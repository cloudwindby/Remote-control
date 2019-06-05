#include "cmddialog.h"
#include "ui_cmddialog.h"

CMDDialog::CMDDialog(SOCKET sClient,QWidget *parent) :
    QDialog (parent),
    ui(new Ui::CMDDialog)
{
    ui->setupUi(this);
    QObject::connect(this, SIGNAL(on_cmd_clicked(SOCKET, QString)), parent, SLOT(slot_cmd_clicked(SOCKET, QString)));
    m_ClientSocket = sClient;
    setWindowTitle("CMD");
    setMinimumSize(600,400);

    QPalette pal;
    pal = ui->cmdText->palette();
    pal.setColor(QPalette::Base, QColor(0,0,0));//改变背景色
    pal.setColor(QPalette::Text, QColor(255,255,255));//改变背景色
    ui->cmdText->setPalette(pal);
}

CMDDialog::~CMDDialog()
{
    delete ui;
}

void CMDDialog::ShowData(QByteArray data)
{
    QTextCodec* qTextCodec = QTextCodec::codecForName("gbk");
    QString strCMD = qTextCodec->toUnicode(data.data(), data.length());
    ui->cmdText->append(strCMD);
}

void CMDDialog::on_sendcmd_clicked()
{
    QString strCMD = ui->cmdEdit->text();
    //注意命令行后要加\n代表一个命令输入结束
    strCMD += '\n';

    ui->cmdEdit->clear();
    //数据转发到mainwindow里处理
    emit on_cmd_clicked(m_ClientSocket, strCMD);
}
