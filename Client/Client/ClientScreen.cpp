#include "stdafx.h"
#include "Client.h"
#include "../../common/zlib.h"

SOCKET sClient;
HANDLE hMyCmdWrite;
void OnServerScreen(SOCKET s, char* pData, int nLength) 
{
    //发送命令，告诉服务器收到数据可以创建screen
    tagPacket pkt;
    pkt.ucode = CLIENT_SCREEN_REPLY;
    pkt.nLength = 0;
    send(sClient,
        (char*)&pkt,
        sizeof(tagPacket),
        0);
}

void OnServerScreenData(SOCKET s, char* pData, int nLength)
{
    //获取屏幕的数据，并转成bmp格式

    //获取桌面窗口句柄
    HWND hDesktop = GetDesktopWindow();
    RECT rc = { 0 };
    HDC hDesktopDC = NULL;
    HDC hTmpDC = NULL;
    HDC hMemDC = NULL;
    BOOL bRet = FALSE;
    HBITMAP  hBitmap = NULL;
    HBITMAP hBitmapOld = NULL;
    int nWidth = 0;
    int nHeight = 0;
    int nSize = 0;
    char* pBuf = NULL;

    if (hDesktop == NULL) 
    {
        goto SAFE_EXIT;
    }

    //获取桌面窗口DC
    hDesktopDC = GetDC(hDesktop);
    if (hDesktopDC == NULL) 
    {
        goto SAFE_EXIT;
    }

    //创建兼容DC
    hMemDC = CreateCompatibleDC(hDesktopDC);
    if (hMemDC == NULL) 
    {
        goto SAFE_EXIT;
    }

    //获取宽和高
    bRet = GetWindowRect(hDesktop, &rc);
    if (!bRet) 
    {
        goto SAFE_EXIT;
    }

    nWidth = rc.right - rc.left;
    nHeight = rc.bottom - rc.top;

    // 创建位图

    //位图信息头
    BITMAPINFOHEADER bmpInfoHeader;
    //位图文件头
    BITMAPFILEHEADER bmpfileHeader;
    BITMAPINFO bmpInfo;
    void *p;

    //RGB数据的大小
    nSize = 3 * nWidth * nHeight;
    //位图的总大小
    pBuf = new char[nSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
    if (pBuf == NULL)
    {
        goto SAFE_EXIT;
    }

    DWORD dwSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nSize;
    DWORD dwOffSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);

    //24位位图
    bmpInfoHeader.biBitCount = 24;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biCompression = BI_RGB;
    bmpInfoHeader.biWidth = nWidth;
    bmpInfoHeader.biHeight = nHeight;
    bmpInfoHeader.biSizeImage = 0;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;
    bmpInfoHeader.biXPelsPerMeter = nWidth;
    bmpInfoHeader.biYPelsPerMeter = nHeight;
    
    //位图的头要为4D42
    bmpfileHeader.bfType = 0x4D42;
    bmpfileHeader.bfSize = dwSize;
    bmpfileHeader.bfReserved1 = 0;
    bmpfileHeader.bfReserved2 = 0;
    bmpfileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    

    bmpInfo.bmiHeader = bmpInfoHeader;
    //CreateDIBSection函数创建应用程序可以直接写入的DIB。该函数提供一个指向位图位值位置的指针。可以为文件映射对象提供句柄，函数将使用该句柄创建位图，也可以让系统为位图分配内存。
    hBitmap = CreateDIBSection(hDesktopDC, &bmpInfo, DIB_RGB_COLORS, &p, NULL, 0);
    
    //SelectObject函数选择进入指定设备上下文(DC)的对象。新对象替换了相同类型的前一个对象。
    hBitmapOld = (HBITMAP)SelectObject(hMemDC, hBitmap);
    if (hBitmapOld == NULL) 
    {
        goto SAFE_EXIT;
    }

    //StretchBlt函数将一个位图从源矩形复制到目标矩形，如果需要，拉伸或压缩该位图以适应目标矩形的尺寸。系统根据当前在目标设备上下文中设置的拉伸模式拉伸或压缩位图。
    StretchBlt(hMemDC, 0, 0, nWidth, nHeight, hDesktopDC, 0, 0, nWidth, nHeight, SRCCOPY);
    hTmpDC = GetDC(NULL);
    if (hTmpDC == NULL) 
    {
        goto SAFE_EXIT;
    }
    //SelectObject函数选择进入指定设备上下文(DC)的对象。新对象替换了相同类型的前一个对象。
    hBitmap = (HBITMAP)SelectObject(hMemDC, hBitmapOld);

    //把位图头写入位图buf
    bmpInfoHeader.biBitCount = 24;
    memcpy(pBuf,&bmpfileHeader,sizeof(bmpfileHeader));
    memcpy(pBuf + sizeof(bmpfileHeader),&bmpInfoHeader,sizeof(bmpInfoHeader));

    //GetDIBits函数检索指定兼容位图的位，并使用指定的格式将它们作为DIB复制到缓冲区中。
    int iError = GetDIBits(hTmpDC, hBitmap, 0, nHeight, pBuf + dwOffSize, (BITMAPINFO *)&bmpInfoHeader, DIB_RGB_COLORS);
    if (!bRet) 
    {
        goto SAFE_EXIT;
    }

    //获取数据，组包发送

    uLong tlen = dwSize;
    char* buf = NULL;
    uLong blen = 0;

    /* 计算缓冲区大小，并为其分配内存 */
    blen = compressBound(tlen); /* 压缩后的长度是不会超过blen的 */
    if ((buf = (char*)malloc(sizeof(char) * blen + 4)) == NULL)
    {
        printf("no enough memory!\n");
    }

    /* 压缩 */
    if (compress((Bytef*)buf, &blen, (Bytef*)pBuf, tlen) != Z_OK)
    {
        printf("compress failed!\n");
    }

    char tmp[8] = { 0 };
    itoa(dwSize, tmp,10);
    char *tmpbuff = (char*)malloc(sizeof(char) * blen + 8);

    //拷贝压缩数据
    memcpy(tmpbuff, buf, blen);
    //拷贝原始数据的大小到数据末尾
    memcpy(tmpbuff + blen, tmp,8);
    //发包
    SendData(s, CLIENT_SCREEN_DATA, tmpbuff, blen+8);

    //释放资源
SAFE_EXIT:
    if (pBuf != NULL) 
    {
        delete[] pBuf;
        pBuf = nullptr;
    }

    if (buf != NULL)
    {
        delete[] buf;
        buf = nullptr;
    }

    if (tmpbuff != NULL)
    {
        delete[] tmpbuff;
        tmpbuff = nullptr;
    }

    if (hBitmap != NULL)
    {
        DeleteObject(hBitmap);
    }

    if (hBitmapOld != NULL) 
    {
        DeleteObject(hBitmapOld);
    }

    if (hTmpDC != NULL) 
    {
        DeleteObject(hTmpDC);
    }

    if (hMemDC != NULL) 
    {
        DeleteDC(hMemDC);
    }

    if (hDesktopDC != NULL)
    {
        ReleaseDC(hDesktop, hDesktopDC);
    }
}