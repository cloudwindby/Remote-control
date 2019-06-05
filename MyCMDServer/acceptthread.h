#ifndef ACCEPTTHREAD_H
#define ACCEPTTHREAD_H
#include <QThread>
#include "tcpserver.h"
#include "iocpserver.h"
class AcceptThread : public QThread
{
    Q_OBJECT
public:
    AcceptThread(TCPServer&,iocpserver& IocpServer);
    ~AcceptThread();

    virtual void run();

private:
    TCPServer& m_Server;
    QByteArray m_btArray;
    iocpserver& m_IocpServer;
};

#endif // ACCEPTTHREAD_H
