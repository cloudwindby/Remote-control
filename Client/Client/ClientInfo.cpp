#include "stdafx.h"
#include "Client.h"
void  OnServerGetClientInfo(SOCKET s, char* pData, int nLength)
{
    tagClientInfo clientInfo = { 0 };

    DWORD dwUserNameLen;
    DWORD dwNameLen;
    GetUserNameW(clientInfo.szUserName, &dwUserNameLen);
    GetComputerNameA(clientInfo.szComPuterName, &dwNameLen);

    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    clientInfo.lnMemorySize = status.ullTotalPhys;

    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    clientInfo.dwCPUCount = siSysInfo.dwNumberOfProcessors;

    SendData(s, CLIENT_SENDCLIENTINFO,(char*)&clientInfo, sizeof(tagClientInfo));
}