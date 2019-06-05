#include "iocpserver.h"
#include <winsock2.h>
iocpserver::iocpserver()
{

}

iocpserver::~iocpserver()
{
    for (int i = 0; i < 5; i++)
    {
        m_pThread[i]->terminate();
    }
}

HANDLE iocpserver::GetHandle()
{
    return m_hIOCP;
}

bool iocpserver::Init(TCPServer& Server)
{

    m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
                                     NULL,
                                     0,
                                     0);

    if (m_hIOCP == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    //创建线程池
    for (int i = 0; i < 5; i++)
    {
        m_pThread[i] = QSharedPointer<recvthread>(new recvthread(Server,this));
        m_pThread[i]->start();
    }

    return true;
}

bool iocpserver::Bind(SOCKET sClient)
{
    HANDLE dwRet =  CreateIoCompletionPort((HANDLE)sClient,
                                           m_hIOCP,
                                           (ULONG_PTR)sClient,
                                           8);
    if (dwRet == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    return true;
}

bool iocpserver::PostRecv(SOCKET sClient)
{
    //到这里表示关联成功了，投递一个recv请求
    DWORD dwReadedBytes = 0;
    MyOverlapped* pRecvOl = new MyOverlapped(IO_READ);

    WSABUF wsaBuf;
    wsaBuf.buf = pRecvOl->pRecvBuf;
    wsaBuf.len = 8192;

    DWORD flags = 0;

    int nRet = WSARecv(sClient,
                       &wsaBuf,
                       1,
                       &dwReadedBytes,
                       &flags,
                       &pRecvOl->ol,
                       NULL);
    if (nRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
    {
        return false;
    }

    return true;
}

bool iocpserver::SendData(QSharedPointer<ClientSession> spClientSession, int nType, const char *pBuf, int nLength)
{
        char* pSendBuf = new char[sizeof(tagPacket) + nLength];
        if (pSendBuf == NULL)
        {
            return false;
        }


        tagPacket* pkt = (tagPacket*)pSendBuf;
        pkt->ucode = nType;
        pkt->nLength = nLength;

        
        if (nLength > 0)
        {
            memcpy(pSendBuf + sizeof(tagPacket) , pBuf,nLength);
        }

        //需要循环的发送
        spClientSession->m_Mutex.lock();
        //spClientSession->m_btSendArray.clear();
        spClientSession->m_btSendArray.append(pSendBuf, (sizeof(tagPacket) + nLength));
        spClientSession->m_Mutex.unlock();
       
        qDebug() << spClientSession->m_btSendArray.size() << endl;
        if (spClientSession->m_btSendArray.size() == (sizeof(tagPacket) + pkt->nLength))
        {
            PostSend(spClientSession);
        }

        if (pSendBuf != NULL) 
        {
            delete[] pSendBuf;
            pSendBuf = NULL;
        }
        return true;
}

bool iocpserver::PostSend(QSharedPointer<ClientSession> pClientSession)
{
    QMutexLocker locker(&pClientSession->m_Mutex);

        if (pClientSession->m_btSendArray.size() == 0)
        {
            return true;
        }

        WSABUF wsaBuf;
        wsaBuf.buf = pClientSession->m_btSendArray.data();
        wsaBuf.len = pClientSession->m_btSendArray.size();
        DWORD dwSendedBytes = 0;

        MyOverlapped* pWriteOl = new MyOverlapped(IO_WRITE);

        int nRet = WSASend(pClientSession->m_sClient,
            &wsaBuf,
            1,
            &dwSendedBytes,
            0,
            &pWriteOl->ol,
            NULL);

        if (nRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
        {
            return false;
        }

        return true;
}
