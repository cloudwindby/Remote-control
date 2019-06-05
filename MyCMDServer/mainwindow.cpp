#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string.h>
#include <QTimer>
#include <QMessageBox>
extern MainWindow* g_pWindow;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    g_pWindow = this;

    //实时显示当前系统时间
    timerUpDate();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpDate()));
    timer->start(1000);
    
    //设置主窗口title
    setWindowTitle("Server");

    //使窗口固定
    setFixedSize(width(), height());

    //读INI文件
    QSettings *IniRead = new QSettings("Server.ini", QSettings::IniFormat);
    //从INI文件获取端口号
    m_Port = QString::number(IniRead->value("SETTING/Port").toUInt());
    //设置端口号
    ui->lineEdit_2->setText(m_Port);

    //设置table为六列
    ui->tableWidget->setColumnCount(6);

    //设置表头信息
    QStringList labels;
    labels << QString(QStringLiteral("IPaddress")) << QString(QStringLiteral("Port"))
           << QString(QStringLiteral("User Name")) << QString(QStringLiteral("Computer Name"))
           << QString(QStringLiteral("Memory Size")) << QString(QStringLiteral("Logical Processor"));
    ui->tableWidget->setHorizontalHeaderLabels(labels);//设置表头
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    ui->tableWidget->setColumnWidth(0,80);//设置第一列的宽度
    ui->tableWidget->setColumnWidth(1,50);//设置第一列的宽度
    ui->tableWidget->setColumnWidth(2,120);//设置第一列的宽度
    ui->tableWidget->setColumnWidth(3,140);//设置第一列的宽度
    ui->tableWidget->setColumnWidth(4,100);//设置第一列的宽度

    //获取当前机器ip列表:(仅ipv4)
    get_localmachine_ip();

    //设置启动状态
    m_isStart = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerUpDate()
{
    //获取当前系统时间
    QDateTime time = QDateTime::currentDateTime();
    //格式化输出 年-月-日 时-分-秒
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    //设置UI显示
    ui->timelabel->setText(str);
}

void MainWindow::timerEvent(QTimerEvent*)
{
    m_Server.CheckClient();
}

//处理获取到的客户端信息
void MainWindow::ShowData(QByteArray data)
{
    char *pBuff = data.data();

    //把获取到的数据强转为clientinfo结构体
    tagClientInfo* clientInfo =  (tagClientInfo*)pBuff;

    //处理内存显示(按GB)
    double dbSize;
    if (clientInfo->lnMemorySize > (1024 * 1024 * 1024))
    {
        dbSize = ((double)(clientInfo->lnMemorySize)) / (1024 * 1024 * 1024);
    }

    //从数据中取出客户端CPU的核心数(逻辑核心,一般情况多线程技术的CPU,逻辑核心是物理核心的2倍)
    QString qstrCPUCount = QString::number(clientInfo->dwCPUCount);

    //从数据中取出客户端的内存大小
    QString qstrMemorySize = QString::number(dbSize, 'f', 2);

    //取出客户端用户名
    QString qstrUserName = QString::fromWCharArray(clientInfo->szUserName);

    //取出客户端计算机名
    QString qstrComputerName(clientInfo->szComPuterName);
    qstrMemorySize += "GB";

    //插入table中
    QTableWidgetItem * pTableItemCPUCount = new QTableWidgetItem();
    QTableWidgetItem * pTableItemMemorySize = new QTableWidgetItem();
    QTableWidgetItem * pTableItemUserName = new QTableWidgetItem();
    QTableWidgetItem * pTableItemComputerName = new QTableWidgetItem();
    pTableItemCPUCount->setText(qstrCPUCount);
    pTableItemCPUCount->setTextAlignment(Qt::AlignCenter);
    pTableItemMemorySize->setText(qstrMemorySize);
    pTableItemMemorySize->setTextAlignment(Qt::AlignCenter);
    pTableItemUserName->setText(qstrUserName);
    pTableItemUserName->setTextAlignment(Qt::AlignCenter);
    pTableItemComputerName->setText(qstrComputerName);
    pTableItemComputerName->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(m_nRow, 2, pTableItemUserName);
    ui->tableWidget->setItem(m_nRow, 3, pTableItemComputerName);
    ui->tableWidget->setItem(m_nRow, 4, pTableItemMemorySize);
    ui->tableWidget->setItem(m_nRow, 5, pTableItemCPUCount);

}

QString MainWindow::get_localmachine_ip()
{
    //获取本机ip地址 (仅ipv4)
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    ui->comboBox->setMaxCount(ipAddressesList.size());
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        //当前协议为ipv4时,才插入comboBox显示
        if (ipAddressesList.at(i).protocol() == QAbstractSocket::IPv4Protocol)
        {
            ui->comboBox->insertItem(i, ipAddressesList.at(i).toString());
        }
    }
    return ipAddress;
}

void MainWindow::on_startserver_clicked()
{
    //点击服务器启动按钮的槽

    //读取INI文件获取端口号,和构造里不同,这里端口号可以自定义,开启后获取到的端口号是新的.
    QSettings *IniRead = new QSettings("Server.ini", QSettings::IniFormat);
    m_Port = QString::number(IniRead->value("SETTING/Port").toUInt());

    //设置右键菜单
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_menu = QSharedPointer<QMenu>(new QMenu());
    QAction* pCmdAction = m_menu->addAction(QStringLiteral("Command"));
    QAction* pProcessAction = m_menu->addAction(QStringLiteral("Process"));
    QAction* pScreenAction = m_menu->addAction(QStringLiteral("Screen"));
    QAction* pFileAction = m_menu->addAction(QStringLiteral("File"));

    //关联右键菜单响应
    connect(pCmdAction, SIGNAL(triggered()), this, SLOT(on_cmdbutton_clicked()));
    connect(pProcessAction, SIGNAL(triggered()), this, SLOT(on_processbutton_clicked()));
    connect(pScreenAction, SIGNAL(triggered()), this, SLOT(on_screenbutton_clicked()));
    connect(pFileAction, SIGNAL(triggered()), this, SLOT(on_filebutton_clicked()));

    if (!m_isStart)
    {
        
        //如果线程没有开启
        if (m_pThread == nullptr)
        {
            //初始化,绑定,监听并开启线程等待链接
            ui->label->clear();
            m_Server.InitServer();
            m_Server.CreateBind(m_Port);
            m_Server.LisentConnect();
            m_pThread = QSharedPointer<AcceptThread>(new AcceptThread(m_Server, m_IocpServer));
            m_pThread->start();
            m_isStart = true;
            ui->statslabel->setText(QStringLiteral("Server is Start"));
        }
        //如果线程开启,其他设置不需要改变
        else if(m_pThread->isRunning())
        {
            m_isStart = true;
            ui->statslabel->setText(QStringLiteral("Server is Start"));
        }
        startTimer(15 * 1000);
    }
}

//右键响应
void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    if (ui->tableWidget->rowCount() < 1)
    {
        return;
    }
    else if (ui->tableWidget->itemAt(pos) != 0)
    {
        m_menu->exec(QCursor::pos());
    }

}

//上线信息
void MainWindow::slot_ClientOnline(QSharedPointer<ClientSession> pClientSession)
{
    //获取端口和ip
    QString qstrAddr = inet_ntoa(pClientSession->clientAddr.sin_addr);
    QString qstrPort = QString::number(ntohs(pClientSession->clientAddr.sin_port));

    //插入table中
    m_nRow = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(m_nRow);

    QTableWidgetItem * pTableItemAddr = new QTableWidgetItem();
    QTableWidgetItem * pTableItemPort = new QTableWidgetItem();

    pTableItemAddr->setText(qstrAddr);
    pTableItemAddr->setTextAlignment(Qt::AlignCenter);
    pTableItemAddr->setData(Qt::UserRole, pClientSession->m_sClient);

    pTableItemPort->setText(qstrPort);
    //居中显示
    pTableItemPort->setTextAlignment(Qt::AlignCenter);

    //发送数据获取其他客户端信息
    m_IocpServer.SendData(pClientSession, SERVER_GETCLIENTINFO, NULL, 0);
    ui->tableWidget->setItem(m_nRow, 0, pTableItemAddr);
    ui->tableWidget->setItem(m_nRow, 1, pTableItemPort);
}

//设置端口
void MainWindow::on_lineEdit_2_returnPressed()
{
    QString qstrPort = ui->lineEdit_2->text();
    //端口不能为空
    if (!qstrPort.isEmpty())
    {
        ui->label->setText(QStringLiteral("Port set is OK"));
        QSettings *IniWrite = new QSettings("Server.ini", QSettings::IniFormat);
        IniWrite->setValue("SETTING/Port", qstrPort.toUInt());
    }
    else
    {
        ui->label->setText(QStringLiteral("Input is Invalid"));
    }
}

//响应创建对话框
void MainWindow::slot_CreateDialog(int s, int nType)
{
    QSharedPointer<ClientSession> client = m_Server.GetClient(s);

    switch (nType)
    {

    case CMD_DIALOG:
    {
        if (client->m_CMDDialog.isNull())
        {
            client->m_CMDDialog = QSharedPointer<CMDDialog>(new CMDDialog(s,this));
            client->m_CMDDialog->show();
        }
        else
        {
            client->m_CMDDialog->show();
        }

    }
        break;

    case SCREEN_DIALOG:
    {
        if (client->m_ScreenDialog.isNull())
        {
            client->m_ScreenDialog = QSharedPointer<ScreenDialog>(new ScreenDialog(s));
            client->m_ScreenDialog->show();
            m_IocpServer.SendData(client, SERVER_SCREEN_DATA, NULL, 0);
        }
        else
        {
            client->m_ScreenDialog->show();
        }
    }
        break;

    case PROCESS_DIALOG:
    {
        if (client->m_ProcessDialog.isNull())
        {
            client->m_ProcessDialog = QSharedPointer<processdialog>(new processdialog(s));
            client->m_ProcessDialog->show();
            m_IocpServer.SendData(client, SERVER_PROCESS_DATA, NULL, 0);
        }
        else
        {
            client->m_ProcessDialog->show();
        }
    }
        break;

    case FILE_DIALOG:
    {
        if (client->m_FileDialog.isNull())
        {
            client->m_FileDialog = QSharedPointer<filedialog>(new filedialog(s));
            client->m_FileDialog->show();
            m_IocpServer.SendData(client, SERVER_FILE_INIT, NULL, 0);
        }
        else
        {
            client->m_FileDialog->show();
        }
    }
        break;

    default:
        break;
    }
}

//接收数据并交给相应的窗口去处理
void MainWindow::slot_ShowRecvData(int s, int nType, QByteArray data)
{
    QSharedPointer<ClientSession> client = m_Server.GetClient(s);
    if (nType == CMD_DIALOG)
    {
        client->m_CMDDialog->ShowData(data);
    }

    if (nType == SCREEN_DIALOG)
    {
        client->m_ScreenDialog->ShowData(data);
    }

    if (nType == PROCESS_DIALOG)
    {
        client->m_ProcessDialog->ShowData(data);
    }

    if (nType == FILE_DATA)
    {
        client->m_FileDialog->ShowData(data);
    }
    if (nType == FILE_DIALOG_INIT)
    {
        client->m_FileDialog->FileInit(data);
    }
    if (nType == FILE_HEADER)
    {
        client->m_FileDialog->RecvFileHeader(data);
    }
    if (nType == FILE_DOWNLOAD)
    {
        client->m_FileDialog->RecvFileByClientSend(data);
    }

    if(nType == CLIENTINFO)
    {
        this->ShowData(data);
    }
}

//断开链接
void MainWindow::slot_Disconnect(int s)
{
    m_Server.CloseConnect(s);
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        if (s == ui->tableWidget->item(i, 0)->data(Qt::UserRole).toInt())
        {
            ui->tableWidget->removeRow(i);
        }
    }
}

//停止按钮
void MainWindow::on_stopserver_clicked()
{
    if(ui->tableWidget->rowCount() > 0)
    {
        m_Server.ClearClient();
        for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            ui->tableWidget->removeRow(i);
        }
    }
    ui->statslabel->setText("Server is Stop");
    m_isStart = false;
}

//发送CMD命令(从CMD窗口中转过来)
void MainWindow::slot_cmd_clicked(SOCKET sClient, QString str)
{
    QSharedPointer<ClientSession> clientSession = m_Server.GetClient(sClient);
    m_IocpServer.SendData(clientSession, SERVER_SEND_COMMAND, str.toLocal8Bit().toStdString().c_str(), str.toLocal8Bit().toStdString().length());
}

//响应CMD按钮
void MainWindow::on_cmdbutton_clicked()
{
    int nIndex = ui->tableWidget->currentRow();
    if(nIndex != -1)
    {
        //拿item
        QTableWidgetItem * pItem = ui->tableWidget->item(nIndex, 0);
        if(pItem != nullptr)
        {
            //从item中获取到自定义的数据(当前为socket)
            SOCKET sClient = pItem->data(Qt::UserRole).toInt();

            //根据socket获取到当前的ClientSession
            QSharedPointer<ClientSession> client = m_Server.GetClient(sClient);

            //为空则创建,否则只显示
            if (client->m_CMDDialog == nullptr)
            {
                qDebug() << "slot_ShowCmd" << endl;
                m_IocpServer.SendData(client, SERVER_CREATE_CMD, NULL, 0);
            }
            else
            {
                client->m_CMDDialog->show();
            }
        }
    }
}

//响应屏幕按钮
void MainWindow::on_screenbutton_clicked()
{
    int nIndex = ui->tableWidget->currentRow();
    if(nIndex != -1)
    {
        QTableWidgetItem * pItem = ui->tableWidget->item(nIndex, 0);
        if(pItem != nullptr)
        {

            SOCKET sClient = pItem->data(Qt::UserRole).toInt();

            QSharedPointer<ClientSession> client = m_Server.GetClient(sClient);
            if (client->m_ScreenDialog == nullptr)
            {
                m_IocpServer.SendData(client, SERVER_CREATE_SCREEN, NULL, 0);
            }
            else
            {
                client->m_ScreenDialog->show();
                client->m_ScreenDialog->SetSendData(true);
                m_IocpServer.SendData(client, SERVER_SCREEN_DATA, NULL, 0);
            }
        }
    }
}

//响应进程按钮
void MainWindow::on_processbutton_clicked()
{
    int nIndex = ui->tableWidget->currentRow();
    if(nIndex != -1)
    {
        QTableWidgetItem * pItem = ui->tableWidget->item(nIndex, 0);
        if(pItem != nullptr)
        {

            SOCKET sClient = pItem->data(Qt::UserRole).toInt();

            QSharedPointer<ClientSession> client = m_Server.GetClient(sClient);
            if (client->m_ProcessDialog == nullptr)
            {
                qDebug() << "SERVER_CREATE_PROCESS" << endl;
                m_IocpServer.SendData(client, SERVER_CREATE_PROCESS, NULL, 0);
            }
            else
            {
                client->m_ProcessDialog->show();
            }
        }
    }
}

//响应文件按钮
void MainWindow::on_filebutton_clicked()
{
    int nIndex = ui->tableWidget->currentRow();
    if(nIndex != -1)
    {
        QTableWidgetItem * pItem = ui->tableWidget->item(nIndex, 0);
        if(pItem != nullptr)
        {
            SOCKET sClient = pItem->data(Qt::UserRole).toInt();

            QSharedPointer<ClientSession> client = m_Server.GetClient(sClient);
            if (client->m_FileDialog == nullptr)
            {
                m_IocpServer.SendData(client, SERVER_CREATE_FILE, NULL, 0);
            }
            else
            {
                client->m_FileDialog->show();
            }
        }
    }
}

void MainWindow::on_buiderbutton_clicked()
{
    QMessageBox message(QMessageBox::NoIcon, "About", "Not Support Yet!");
    message.exec();
}

//响应关于按钮
void MainWindow::on_aboutbutton_clicked()
{
    QMessageBox message(QMessageBox::NoIcon, "About", "This is a Toy!");
    message.setIconPixmap(QPixmap(":/icon/recourse/toy.png"));
    message.exec();
}
