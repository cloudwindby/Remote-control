#include "stdafx.h"
#include "Client.h"
#include <Tlhelp32.h>
#include <vector>

void OnServerProcess(SOCKET s, char* pData, int nLength)
{
    //发送命令，告诉服务器收到数据可以创建screen
    tagPacket pkt;
    pkt.ucode = CLIENT_PROCESS_REPLY;
    pkt.nLength = 0;
    send(s,
        (char*)&pkt,
        sizeof(tagPacket),
        0);
}

void OnServerProcessData(SOCKET s, char* pData, int nLength)
{
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;
    tagProcessInfo processinfo = { 0 };
    std::vector<tagProcessInfo> ProcessData;
    wchar_t FileName[MAX_PATH] = { 0 };


    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return;
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);          // clean the snapshot object
        return;
    }
    // Now walk the snapshot of processes, and
    // display information about each process in turn
    do
    {
        BOOL isWow64 = FALSE;
        BOOL bAccess = FALSE;
        // Retrieve the priority class.
        dwPriorityClass = 0;
        hProcess = OpenProcess(/*PROCESS_QUERY_INFORMATION | PROCESS_VM_READ*/PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
        if (hProcess == NULL)
        {
            bAccess = FALSE;
        }
        else
        {
            bAccess = TRUE;
            DWORD size = MAX_PATH;
            memset(FileName, 0, sizeof(FileName));

            //获取进程镜像全路径
            QueryFullProcessImageName(hProcess, 0, FileName, &size);
            IsWow64Process(hProcess,&isWow64);
            dwPriorityClass = GetPriorityClass(hProcess);
            if (!dwPriorityClass)
                CloseHandle(hProcess);
        }
        processinfo.bWOW64 = isWow64;
        processinfo.bAccess = bAccess;
        processinfo.szProcessID = pe32.th32ProcessID;
        memcpy(processinfo.szProcessName, pe32.szExeFile,256);
        memcpy(processinfo.szProcessPathName, FileName, 256);
        ProcessData.push_back(processinfo);
    } while (Process32Next(hProcessSnap, &pe32));

    char *pdata = new char[ProcessData.size() * sizeof(tagProcessInfo)];
    memcpy(pdata, ProcessData.data(), ProcessData.size() * sizeof(tagProcessInfo));

    CloseHandle(hProcessSnap);
    SendData(s, CLIENT_PROCESS_DATA, pdata, ProcessData.size() * sizeof(tagProcessInfo));
    if (pdata != nullptr)
    {
        delete[] pdata;
        pdata = nullptr;
    }
}

void OnServerTerminateProcess(SOCKET s, char * pData, int nLength)
{
    int PID = atoi(pData);

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, PID);
    TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);

    //由于本地测试速度太快,导致服务端的显示有一些小问题.这里用Sleep来处理
    Sleep(1000);

    OnServerProcessData(s,0,0);
}
