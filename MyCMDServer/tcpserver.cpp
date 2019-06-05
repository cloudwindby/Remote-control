#include "tcpserver.h"
#include "mainwindow.h"

extern MainWindow* g_pWindow;

TCPServer::TCPServer()
{
    m_ConnectSocket = NULL;
}

TCPServer::~TCPServer()
{
    //QMap<SOCKET, QSharedPointer<ClientSession>>::iterator it = m_ClientSessionMap.begin();
    //release all sockets
    ClearClient();
}

bool TCPServer::InitServer()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        return false;
    }

    return true;
}

bool TCPServer::CreateBind(QString Port)
{
    //create SOCKET : ipv4,tcp
    m_ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_ConnectSocket == SOCKET_ERROR)
    {
        return false;
    }

    //init ip addr and ip port
    sockaddr_in ServerName;
    //ipv4
    ServerName.sin_family = AF_INET;
    //0.0.0.0
    ServerName.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    //port
    ServerName.sin_port = htons(Port.toUShort());

    int ServerNameSize = sizeof(ServerName);
    //bind
    int nRet = bind(m_ConnectSocket, reinterpret_cast<sockaddr*>(&ServerName), ServerNameSize);
    if (nRet == SOCKET_ERROR)
    {
        return false;
    }
    return true;
}

bool TCPServer::LisentConnect()
{
    //listen socket connect
    int nRet = listen(m_ConnectSocket, SOMAXCONN);
    if (nRet == SOCKET_ERROR)
    {
        return false;
    }
    return true;
}

bool TCPServer::AcceptConnect(iocpserver* pIocpServer)
{
    sockaddr_in RecvSocket = { 0 };
    int RecvSize = sizeof(RecvSocket);
    RecvSocket.sin_family = AF_INET;
    char szBuffer[20] = { 0 };
    //accept connect

    SOCKET sClient = accept(m_ConnectSocket, reinterpret_cast<sockaddr*>(&RecvSocket), &RecvSize);
    if (sClient == INVALID_SOCKET && WSAGetLastError() != WSAEWOULDBLOCK)
    {
        return false;
    }


    m_client = QSharedPointer<ClientSession>(new ClientSession(sClient));
    m_client->clientAddr = RecvSocket;
    AddClient(sClient, m_client);

    emit sig_ClientOnline(m_client);
    if (pIocpServer != NULL)
    {
        pIocpServer->Bind(sClient);

        //Í¶µÝÒ»¸öRecv
        pIocpServer->PostRecv(sClient);
    }
    // m_client->m_pRecvThread->start();

    return true;
}

//close connect
bool TCPServer::CloseConnect(SOCKET s)
{
    QMutexLocker locker(&m_MapMutex);
    closesocket(s);
    m_ClientSessionMap.remove(s);
    return true;
}

SOCKET TCPServer::GetSocket()
{
    return m_ClientSessionMap.begin()->data()->m_sClient;
}

//give map a key,and return a value
QSharedPointer<ClientSession> TCPServer::GetClient(SOCKET s)
{
    return m_ClientSessionMap.find(s).value();
}

bool TCPServer::ClearClient()
{
    for (QSharedPointer<ClientSession> value : m_ClientSessionMap)
    {
        closesocket(value.data()->m_sClient);
    }
    m_ClientSessionMap.clear();

    return true;
}

bool TCPServer::AddClient(SOCKET s, QSharedPointer<ClientSession> client)
{
    QMutexLocker locker(&m_MapMutex);
    m_ClientSessionMap.insert(s, client);
    return true;
}

bool TCPServer::CheckClient()
{
  
        QMutexLocker locker(&m_MapMutex);
        QMap<SOCKET, QSharedPointer<ClientSession>>::iterator it = m_ClientSessionMap.begin();
        for (; it != m_ClientSessionMap.end(); it++)
        {
            qDebug() << "currentTime:" << QTime::currentTime().msecsSinceStartOfDay() << endl;
            qDebug() <<  "lastTime:" <<it.value()->m_lastTime << endl;
            if ((QTime::currentTime().msecsSinceStartOfDay() - it.value()->m_lastTime) > (25 * 1000))
            {
                closesocket(it.key());
            }
        }
    qDebug() << "CheckClient" << endl;
    return true;
}
