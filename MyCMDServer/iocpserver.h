#ifndef IOCPSERVER_H
#define IOCPSERVER_H
#include "recvthread.h"
#include "tcpserver.h"

class iocpserver
{
public:
    iocpserver();
    ~iocpserver();

    bool Init(TCPServer& Server);

    bool Bind(SOCKET s);

    bool PostRecv(SOCKET s);

    bool SendData(QSharedPointer<ClientSession> pClientSession, int nType,const char* pBuf, int nLength);

    bool PostSend(QSharedPointer<ClientSession> pClientSession);

    HANDLE GetHandle();
    QSharedPointer<recvthread> m_pThread[5];

private:
    HANDLE m_hIOCP;
    //QSharedPointer<recvthread> m_pThread[5];
};

#endif // IOCPSERVER_H
