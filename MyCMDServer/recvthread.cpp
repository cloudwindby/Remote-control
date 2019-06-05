#include "iocpserver.h"
#include "tcpserver.h"
#include "mainwindow.h"
extern MainWindow* g_pWindow;
recvthread::recvthread(TCPServer& Server, iocpserver* pIocpServer)
    :m_Server(Server)
{
    m_pIocpServer = pIocpServer;
    QObject::connect(this,SIGNAL(sig_CreateDialog(int, int)),
                     g_pWindow,SLOT(slot_CreateDialog(int, int)));

    QObject::connect(this,SIGNAL(sig_ShowRecvData(int, int, QByteArray)),
                     g_pWindow,SLOT(slot_ShowRecvData(int, int, QByteArray)));

    QObject::connect(this,SIGNAL(sig_Disconnect(int)),
                     g_pWindow,SLOT(slot_Disconnect(int)));
}

void recvthread::run()
{
    DWORD dwTransferBytes = 0;
    ULONG_PTR dwCompleteKey = 0;
    LPOVERLAPPED lpOl = NULL;
    BOOL bRet = false;
    SOCKET s = NULL;

    while (!isInterruptionRequested())
    {
        //从消息队列中取出消息(完成的消息)
        bRet = GetQueuedCompletionStatus(m_pIocpServer->GetHandle(),    //socket
                                         &dwTransferBytes,
                                         &dwCompleteKey,
                                         &lpOl,
                                         INFINITE);

        if (!bRet)
        {
            break;
        }

        //处理消息
        s = (SOCKET)dwCompleteKey;
        MyOverlapped* pOl = CONTAINING_RECORD(lpOl, MyOverlapped, ol);

        QSharedPointer<ClientSession> clientSession =  m_Server.GetClient(s);

        if (pOl->nType == IO_READ)
        {

            //将收到的数据放到client的缓冲区
            clientSession->m_btRecvArray.append(pOl->pRecvBuf, dwTransferBytes);

            //qDebug() << m_btArray << endl;
            //检查缓冲区是否满足一个包的大小，如果满足，则处理。
            while (clientSession->m_btRecvArray.size() >= 5)
            {
                tagPacket* pkt = (tagPacket*)clientSession->m_btRecvArray.data();

                int nCode = pkt->ucode;
                int nLength = pkt->nLength;
                char* pData = NULL;
                if (clientSession->m_btRecvArray.size() - 5 >= nLength)
                {
                    //说明已经完整的收到了一个包，可以开始处理了
                    if (nLength > 0)
                    {
                        pData = new char[nLength];
                        memcpy(pData, clientSession->m_btRecvArray.data() + 5, nLength);
                    }

                    clientSession->m_btRecvArray.remove(0, nLength + 5);

                    switch (nCode)
                    {

                    //create cmd dialog
                    case CLIENT_CMD_REPLY:
                    {
                        emit sig_CreateDialog(s,CMD_DIALOG);
                    }
                        break;

                        //show cmd data
                    case CLIENT_CMD_DATA:
                    {
                        emit sig_ShowRecvData(s,CMD_DIALOG,QByteArray(pData,nLength));
                    }
                        break;

                    case CLIENT_SCREEN_REPLY:
                    {
                        qDebug() << "CLIENT_SCREEN_REPLY" << endl;
                        emit sig_CreateDialog(s,SCREEN_DIALOG);
                    }
                        break;

                    case CLIENT_SCREEN_DATA:
                    {
                        qDebug() << "CLIENT_SCREEN_DATA" << endl;
                        emit sig_ShowRecvData(s,SCREEN_DIALOG,QByteArray(pData,nLength));
                    }
                        break;

                    case CLIENT_PROCESS_REPLY:
                    {
                        emit sig_CreateDialog(s,PROCESS_DIALOG);
                    }
                        break;

                    case CLIENT_PROCESS_DATA:
                    {
                        qDebug() << "CLIENT_SCREEN_DATA" << endl;
                        emit sig_ShowRecvData(s,PROCESS_DIALOG,QByteArray(pData,nLength));
                    }
                        break;

                    case CLIENT_FILE_REPLY:
                    {
                        emit sig_CreateDialog(s,FILE_DIALOG);
                    }
                        break;

                    case CLIENT_FILE_INIT:
                    {
                        //显示数据
                        emit sig_ShowRecvData(s, FILE_DIALOG_INIT, QByteArray(pData, nLength));
                    }
                        break;

                    case CLIENT_FILE_DATA:
                    {
                        //show data
                        emit sig_ShowRecvData(s, FILE_DATA, QByteArray(pData, nLength));
                    }
                        break;
                    case CLIENT_SEND_FILEHEADER:
                    {
                        //get data
                        emit sig_ShowRecvData(s, FILE_HEADER, QByteArray(pData, nLength));
                    }
                        break;
                    case CLIENT_SEND_FILE:
                    {
                        //get data
                        emit sig_ShowRecvData(s, FILE_DOWNLOAD, QByteArray(pData, nLength));
                    }
                        break;
                    case CLIENT_SENDCLIENTINFO:
                    {
                        emit sig_ShowRecvData(s,CLIENTINFO,QByteArray(pData,nLength));
                    }
                        break;
                    case CLIENT_HEART:
                    {
                        QTime t = QTime::currentTime();
                        clientSession->m_lastTime = t.msecsSinceStartOfDay();
                    }
                        break;

                    default:
                    {

                    }
                        break;

                    }

                    if (pData != nullptr)
                    {
                        delete[] pData;
                        pData = nullptr;
                    }

                }//if (clientSession->m_btRecvArray.size() - 5 >= nLength)
                else
                {
                    break;
                }

            }
            
            m_pIocpServer->PostRecv(s);
        }
        else if (pOl->nType == IO_WRITE)
        {
            Q_ASSERT(clientSession->m_btSendArray.size() >= dwTransferBytes);

            clientSession->m_Mutex.lock();
            clientSession->m_btSendArray.remove(0, dwTransferBytes);
            clientSession->m_Mutex.unlock();
            if (clientSession->m_btSendArray.size() > 0)
            {
                m_pIocpServer->PostSend(clientSession);
            }
        }

        if (pOl != NULL)
        {
            delete pOl;
            pOl = NULL;
        }
    }//end while

    emit sig_Disconnect(s);
}
