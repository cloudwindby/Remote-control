#ifndef MACRO_H
#define MACRO_H

#include <WinSock2.h>
enum COMMAND_TYPE
{
    //server command
    SERVER_CREATE_CMD = 50,
    SERVER_SEND_COMMAND,

    //server screen
    SERVER_CREATE_SCREEN,
    SERVER_SCREEN_DATA,

    //server process
    SERVER_CREATE_PROCESS,
    SERVER_PROCESS_DATA,
    SERVER_TERMINATE_PROCESS,

    //server file
    SERVER_CREATE_FILE,
    SERVER_FILE_INIT,
    SERVER_FILE_DATA,
    SERVER_GET_FILEHEADER,
    SERVER_GET_FILE,

    //client command
    CLIENT_CMD_REPLY,
    CLIENT_CMD_DATA,

    //client screen
    CLIENT_SCREEN_REPLY,
    CLIENT_SCREEN_DATA,

    //client process
    CLIENT_PROCESS_REPLY,
    CLIENT_PROCESS_DATA,

    //client file
    CLIENT_FILE_REPLY,
    CLIENT_FILE_INIT,
    CLIENT_FILE_DATA,
    CLIENT_SEND_FILEHEADER,
    CLIENT_SEND_FILE,

    //INFO
    CLIENT_SENDCLIENTINFO,
    SERVER_GETCLIENTINFO,
    CLIENTINFO,

    CLIENT_HEART,
    SERVER_HEART_REPLY
};

enum DIALOG_TYPE
{
    CMD_DIALOG = 0,
    SCREEN_DIALOG,
    PROCESS_DIALOG,
    FILE_DIALOG,
    FILE_DATA,
    FILE_DIALOG_INIT,
    FILE_HEADER,
    FILE_DOWNLOAD,
    HEART_REPLY
};

//set struct alignment
#pragma pack(push,1)
struct tagPacket
{
    unsigned char ucode;
    size_t nLength;
};
#pragma pack(pop)


// struct for IOCP
enum IOTYPE
{
    IO_READ = 20,
    IO_WRITE,
    IO_CLOSE,
    IO_ACCEPT,
};

#pragma pack(push,1)
struct tagProcessInfo
{
    int szProcessID;
    BOOL bWOW64;
    BOOL bAccess;
    wchar_t szProcessName[256];
    wchar_t szProcessPathName[256];
};
#pragma pack(pop)

#pragma pack(push,1)
struct tagClientInfo
{
    char szComPuterName[256];
    wchar_t szUserName[256];
    DWORDLONG lnMemorySize;
    DWORD dwCPUCount;
};
#pragma pack(pop)

enum fileType
{
    FILE_DRIVER = 30,
    FILE_FOLDER,
    FILE_FILE,
	FILE_BACK
};

#pragma pack(push,1)
struct tagFile
{
    char szFileName[256];
    fileType nType;
};
#pragma pack(pop)

typedef struct 
{
    char szFileName[256];
    unsigned int nLength;
}FileHeader;


struct MyOverlapped
{
    MyOverlapped(IOTYPE type)
    {
        nType = type;
        memset(&ol, 0, sizeof(OVERLAPPED));
    }

    IOTYPE nType;
    OVERLAPPED ol;
    char pRecvBuf[8192];
};

//receive data at specified length
inline bool RecvData(SOCKET s,char* pBuffer,int nLength)
{

    int nCurrentSize = 0;
    int nTotalSize = 0;
    while(nTotalSize < nLength)
    {
        nCurrentSize = recv(s,(char*)pBuffer + nTotalSize,nLength - nTotalSize,0);
        if(nCurrentSize == 0 || nCurrentSize == SOCKET_ERROR)
        {
            return false;
        }
        else
        {
            nTotalSize += nCurrentSize;
        }
    }
    return true;
}

inline bool SendData(SOCKET s,int nType,const char* pBuffer,size_t nLength)
{

    size_t nTotalSize = 0;
    nTotalSize = sizeof(tagPacket) + nLength;
    char* pSendBuffer = new char[nTotalSize];
    memset(pSendBuffer,0,nTotalSize);
    if(pSendBuffer == nullptr)
    {
        return false;
    }

    tagPacket* pPKT = reinterpret_cast<tagPacket*>(pSendBuffer);
    pPKT->ucode = (unsigned char)nType;
    pPKT->nLength = nLength;

    if(nLength > 0)
    {
        memcpy(pSendBuffer+sizeof(tagPacket),pBuffer,nLength);
    }

    send(s,pSendBuffer,nTotalSize,0);
    if (pSendBuffer != nullptr)
    {
        delete [] pSendBuffer;
        pSendBuffer = nullptr;
    }

    return true;
}






#endif
