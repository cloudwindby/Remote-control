#include "filedialog.h"
#include "ui_filedialog.h"
#include <QListWidgetItem>

filedialog::filedialog(SOCKET sClient,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::filedialog)
{
    ui->setupUi(this);
    m_client = sClient;
    setWindowTitle("File");
    m_strPath = "\0";
    m_nCurWritten = 0;
    //setWindowFlags(Qt::WindowCloseButtonHint);
    connect(ui->clientFile,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(slot_itemDoubleClicked(QListWidgetItem*)));
    m_menu = QSharedPointer<QMenu>(new QMenu());
    QAction* pDownload = m_menu->addAction(QStringLiteral("download"));
    connect(pDownload, SIGNAL(triggered()), this, SLOT(slot_download()));
}

filedialog::~filedialog()
{
    delete ui;
}

void filedialog::on_customContextMenuRequested(const QPoint &pos)
{
    if (ui->clientFile->count() < 1)
    {
        return;
    }
    else if (ui->clientFile->itemAt(pos) != 0)
    {
        m_menu->exec(QCursor::pos());
    }
}

void filedialog::slot_itemDoubleClicked(QListWidgetItem *item)
{
    int nType = item->data(Qt::UserRole).toInt();
    switch (nType)
    {
    case FILE_DRIVER:
    {
        m_strPath = item->text();
    }
    break;
    case FILE_FOLDER:
    {
        m_strPath = m_strPath + "\\" + item->text();
    }
    break;
    case FILE_BACK:
    {
        QStringList lTmp = m_strPath.split("\\");
        lTmp.removeLast();
        m_strPath = lTmp.join("\\");
    }
    break;
    }
    SendData(m_client, SERVER_FILE_DATA, m_strPath.toStdString().c_str(), m_strPath.toStdString().length());
}

void filedialog::ShowData(QByteArray data)
{
    ui->clientFile->clear();
    char* pBuf = data.data();
    int nCount = *(int *)pBuf;

    tagFile *pFile = new tagFile;
    //get nCount of file struct
    for (int i = 0; i < nCount; i++)
    {
        memcpy(pFile, pBuf + 4 + i * sizeof(tagFile), sizeof(tagFile));

        QString strName(pFile->szFileName);

        //add data into treeWidget
        QListWidgetItem* pItNew = new QListWidgetItem(strName);
        pItNew->setData(Qt::UserRole, pFile->nType);

        ui->clientFile->insertItem(i, pItNew);
    }

    if (pFile != NULL)
    {
        delete pFile;
        pFile = NULL;
    }
    pBuf = NULL;

    //write model into table
    ui->clientFile->show();
}

void filedialog::FileInit(QByteArray data)
{
    char* pBuf = data.data();
    int nCount = *(int *)pBuf;

    tagFile *pFile = new tagFile();

    //get nCount of file struct
    for(int i = 0; i < nCount; i++)
    {
        memcpy(pFile, pBuf + 4 + i * sizeof(tagFile), sizeof(tagFile));

        QString strName(pFile->szFileName);
        strName.chop(1);

        //add data into treeWidget
        QListWidgetItem* pItDriver = new QListWidgetItem(strName);
        pItDriver->setData(Qt::UserRole, pFile->nType);
        ui->clientFile->insertItem(i,pItDriver);
        //m_pTreeWidget->insertTopLevelItem(i, pItDriver);

        //prompt for child
        //emit sig_FileData(m_sClient, strName);
        
    }

    if(pFile != NULL)
    {
        delete pFile;
        pFile = NULL;
    }
    pBuf = NULL;

    //write model into table
    ui->clientFile->show();
}

void filedialog::RecvFileHeader(QByteArray data)
{
    memcpy(&m_Header, data.data(), sizeof(FileHeader));

    m_File.setFileName(m_Header.szFileName);
    if (!m_File.open(QFile::WriteOnly)) 
    {
        return;
    }

    m_nCurWritten = 0;

    // 如果已经完成就退出
    if (m_nCurWritten >= m_Header.nLength) 
    {
        m_File.close();
    }
    else
    {
        SendData(m_client, SERVER_GET_FILE, 0, 0);
    }
}

void filedialog::RecvFileByClientSend(QByteArray data)
{
    if (data.size() > 0) {
        // 增加已写入数据
        m_nCurWritten += data.size();

        // 写数据到文件里
        m_File.write(data.data(), data.size());
        m_File.flush();
        data.clear();
    }
    // 如果已经完成就退出
    if (m_nCurWritten >= m_Header.nLength)
    {
        m_File.close();
    }
    else
    {
        SendData(m_client, SERVER_GET_FILE, 0, 0);
    }
}

void filedialog::slot_download()
{
    QListWidgetItem *pItem = ui->clientFile->currentItem();

    QString strPath = m_strPath + "\\" + pItem->text();
    SendData(m_client, SERVER_GET_FILEHEADER,strPath.toStdString().c_str(), strPath.toStdString().size());
}
