#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Winsock2.h>
#include <thread>
#include "../../common/macro.h"
//SOCKET（套接字）编程 ： 初始化
//UDP是无连接的
//TCP是有连接的

//cmd func
bool CmdThread();

void OnServerCmd(SOCKET s, char* pData, int nLength);

void OnServerCmdData(SOCKET s, char* pData, int nLength);

//screen func
void OnServerScreen(SOCKET s, char* pData, int nLength);

void OnServerScreenData(SOCKET s, char* pData, int nLength);

//process func
void OnServerProcess(SOCKET s, char* pData, int nLength);

void OnServerProcessData(SOCKET s, char* pData, int nLength);

void OnServerTerminateProcess(SOCKET s, char* pData, int nLength);

//file func
void OnServerFile(SOCKET s, char* pData, int nLength);

void OnServerFileInit(SOCKET s, char* pData, int nLength);

void OnServerFileData(SOCKET s, char* pData, int nLength);

void OnServerFileHeader(SOCKET s, char* pData, int nLength);

void OnServerFileDownLoad(SOCKET s, char* pData, int nLength);

//client info func
void  OnServerGetClientInfo(SOCKET s, char* pData, int nLength);