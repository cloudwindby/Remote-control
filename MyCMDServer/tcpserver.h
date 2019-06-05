#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <WinSock2.h>
#include <QString>
#include <QMap>
#include <QSharedPointer>
#include "recvthread.h"
#include "cmddialog.h"
#include "processdialog.h"
#include "screendialog.h"
#include "filedialog.h"
#include <QMutex>
#include <QTime>
#include <QMetaType>

class iocpserver;
class AcceptThread;
//clientSession
class ClientSession
{
public:
    ClientSession(){}
    ClientSession(const ClientSession&){}
    ~ClientSession(){}
    ClientSession(SOCKET s)
    {
        m_sClient = s;
        //m_pRecvThread = QSharedPointer<recvthread>(new recvthread(s));
        m_CMDDialog = nullptr;
        m_ScreenDialog = nullptr;
        m_ProcessDialog = nullptr;
        m_FileDialog = nullptr;
        //m_szUserNameBuffer = nullptr;
        m_lnMemorySize = 0;
        m_dwCPUCount = 0;
        m_lastTime = QTime::currentTime().msecsSinceStartOfDay();
        m_btSendArray.clear();
    }
public:
    SOCKET m_sClient;
    sockaddr_in clientAddr;
    //char* m_szPCNameBuffer;
    //wchar_t* m_szUserNameBuffer;
    DWORDLONG m_lnMemorySize;
    DWORD m_dwCPUCount;
    //QSharedPointer<recvthread> m_pRecvThread;
    //QSharedPointer<CMDDialog> m_CMDDialog;
    QSharedPointer<CMDDialog> m_CMDDialog;
    QSharedPointer<ScreenDialog> m_ScreenDialog;
    QSharedPointer<processdialog> m_ProcessDialog;
    QSharedPointer<filedialog> m_FileDialog;
    int m_lastTime;
    QByteArray m_btRecvArray;
    QByteArray m_btSendArray;
    QMutex m_Mutex;
};
Q_DECLARE_METATYPE(ClientSession);

class TCPServer:public QObject
{
    Q_OBJECT
public:
    TCPServer();
    ~TCPServer();

public:
    //init server
    bool InitServer();

    //create socket and bind
    bool CreateBind(QString);

    //lisent client
    bool LisentConnect();

    //accept connect
    bool AcceptConnect(iocpserver* pIocpServer);

    //close connect
    bool CloseConnect(SOCKET);

    SOCKET GetSocket();

    QSharedPointer<ClientSession> GetClient(SOCKET);
    bool ClearClient();

    bool AddClient(SOCKET, QSharedPointer<ClientSession>);
    bool CheckClient();

signals:
    void sig_ClientOnline(QSharedPointer<ClientSession>);
    void sig_Disconnect(int s);
    void sig_ClearClientList(int s);

private:
    //socket of accept
    SOCKET m_ConnectSocket;
    //clientSession data   key to value
    QMap<SOCKET,QSharedPointer<ClientSession>> m_ClientSessionMap;
    QSharedPointer<ClientSession> m_client;
    QMutex m_MapMutex;
};

#endif // TCPSERVER_H
