#include "processdialog.h"
#include "ui_processdialog.h"
#include <qDebug>

processdialog::processdialog(SOCKET s,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::processdialog)
{
    ui->setupUi(this);
    m_ClientSocket = s;
    setWindowTitle("Process");
    setMinimumSize(600,400);
    resize(width(),height());
    setWindowFlags(Qt::WindowCloseButtonHint);
    QStringList labels;
    labels << QStringLiteral("进程名称")
        << QStringLiteral("进程ID")
        << QStringLiteral("平台")
        << QStringLiteral("运行权限")
        << QStringLiteral("可执行文件路径");
                
        ui->processtable->setHorizontalHeaderLabels(labels);//设置表头
        ui->processtable->setSortingEnabled(true);//设置排序
        ui->processtable->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
        ui->processtable->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
        ui->processtable->setColumnWidth(0, 160);//设置第一列的宽度
        ui->processtable->setColumnWidth(1, 60);//设置第一列的宽度
        ui->processtable->setColumnWidth(2, 50);//设置第一列的宽度
        ui->processtable->setColumnWidth(3, 80);//设置第一列的宽度
        ui->processtable->setColumnWidth(4, 300);//设置第一列的宽度
        m_menu = QSharedPointer<QMenu>(new QMenu());
        QAction* pTerminate = m_menu->addAction(QStringLiteral("terminate"));
        connect(pTerminate,SIGNAL(triggered()),this,SLOT(slot_terminate()));
}

processdialog::~processdialog()
{
    delete ui;
}

void processdialog::ShowData(QByteArray data)
{
    ui->processtable->clear();
    QStringList labels;
    labels << QStringLiteral("进程名称")
        << QStringLiteral("进程ID")
        << QStringLiteral("平台")
        << QStringLiteral("运行权限")
        << QStringLiteral("可执行文件路径");
    ui->processtable->setHorizontalHeaderLabels(labels);//设置表头
    char* pdata = data.data();
    //memcpy(pdata,data.data(), data.size());

    int nCount = data.size() / sizeof(tagProcessInfo);
    ui->processtable->setRowCount(nCount);
    for (int i = 0; i < nCount; i++)
    {
        QString qsExeFileName = QString::fromWCharArray(((tagProcessInfo*)pdata)->szProcessName);
        QString qsExePath = QString::fromWCharArray(((tagProcessInfo*)pdata)->szProcessPathName);
        QString qsProcessID = QString::number(((tagProcessInfo*)pdata)->szProcessID, 10);
        QString qsPlatform;
        QString qsISAdmin;
        if (((tagProcessInfo*)pdata)->bAccess)
        {
            qsISAdmin = QStringLiteral("允许访问");
        }
        else
        {
            qsISAdmin = QStringLiteral("拒绝访问");
        }
        if (((tagProcessInfo*)pdata)->bWOW64)
        {
            qsPlatform = "32";
        }
        else
        {
            qsPlatform = "64";
        }
        ui->processtable->setItem(i, 0, new QTableWidgetItem(qsExeFileName));
        ui->processtable->setItem(i, 1, new QTableWidgetItem(qsProcessID));
        ui->processtable->setItem(i, 2, new QTableWidgetItem(qsPlatform));
        ui->processtable->setItem(i, 3, new QTableWidgetItem(qsISAdmin));
        ui->processtable->setItem(i, 4, new QTableWidgetItem(qsExePath));
        pdata += sizeof(tagProcessInfo);
    }
    
}

void processdialog::on_processtable_customContextMenuRequested(const QPoint &pos)
{
    if(ui->processtable->rowCount() < 1)
    {
        return;
    }
    else if(ui->processtable->itemAt(pos) != 0)
    {
        m_menu->exec(QCursor::pos());
    }
}

void processdialog::slot_terminate()
{
    int nIndex = ui->processtable->currentRow();
    QTableWidgetItem * pItem = ui->processtable->item(nIndex, 1);
    SendData(m_ClientSocket,SERVER_TERMINATE_PROCESS, pItem->text().toStdString().c_str(), pItem->text().toStdString().size());
}
