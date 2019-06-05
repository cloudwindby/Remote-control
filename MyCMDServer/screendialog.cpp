#include "screendialog.h"
#include "ui_screendialog.h"
#include "../../common/zlib.h"

ScreenDialog::ScreenDialog(SOCKET s,QWidget *parent):
    QDialog (parent),
    ui(new Ui::ScreenDialog)
{
    ui->setupUi(this);
    m_bDraw = false;
    m_bSendData = true;
    m_ClientSocket = s;
    setMinimumSize(600,400);
    resize(width(),height());
    setWindowTitle("Screen");
    setWindowFlags(Qt::WindowCloseButtonHint & Qt::WindowMaximizeButtonHint);
}

ScreenDialog::~ScreenDialog()
{
    delete ui;
}

void ScreenDialog::ShowData(QByteArray data)
{
    //数据经过压缩,客户端把压缩前的数据大小保存在数据的末尾8位
    char tmp[8] = { 0 };
    int n = data.size() - 8;
    memcpy(tmp, data.data() + n, 8);
    //把末尾的字符串转为ULONG
    ULONG nsize =  atol(tmp);
    char *pBuf = new char[nsize];
    //解压缩
    if (uncompress((Bytef*)pBuf, &nsize, (Bytef*)data.data(), data.size() - 8) != Z_OK)
    {
        printf("uncompress failed!\n");
    }
    //把解压后的数据重新构建成QByteArray
    QByteArray imgdata(pBuf, nsize);
    //数据转为img
    m_ImgFromClient = QImage::fromData(imgdata);
    if (pBuf != nullptr)
    {
        delete[] pBuf;
        pBuf = nullptr;
    }
 
    m_bDraw = true;
    update();

    if (m_bSendData)
    {
        SendData(m_ClientSocket, SERVER_SCREEN_DATA, NULL, 0);
    }
}

void ScreenDialog::paintEvent(QPaintEvent *)
{
    
    if (m_bDraw)
    {
        QPainter painter(this);

        //根据窗口大小重绘
        m_ScaledImg =  m_ImgFromClient.scaled(width(), height());
        painter.drawImage(0, 0, m_ScaledImg);

        //在窗口上设置一个button
        ui->pushButton->setGeometry(width()-100, height()/2, 80, 20);
    }
}

void ScreenDialog::closeEvent(QCloseEvent *)
{
    m_bSendData = false;
    this->close();
}

void ScreenDialog::SetSendData(bool b)
{
    m_bSendData = b;
}

void ScreenDialog::on_pushButton_clicked()
{
    //保存图片
    QString qstrSaveFileName = QFileDialog::getSaveFileName(this, tr("Save File"),"save.bmp","Images (*.bmp)");
    m_ImgFromClient.save(qstrSaveFileName);
}
