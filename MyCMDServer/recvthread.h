#ifndef RECVTHREAD_H
#define RECVTHREAD_H
#include <QThread>
#include <QString>
#include <qDebug>
#include "../common/macro.h"

class TCPServer;
class iocpserver;

class recvthread : public QThread
{
    Q_OBJECT
public:
    recvthread(TCPServer& Server, iocpserver* pIocpServer);

    virtual void run();
signals:
    void sig_ShowRecvData(int, int, QByteArray);
    void sig_CreateDialog(int, int);
    void sig_Disconnect(int s);

private:
   //SOCKET m_recv;
   iocpserver* m_pIocpServer;
   TCPServer& m_Server;
};

#endif // RECVTHREAD_H
