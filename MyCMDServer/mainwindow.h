#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "tcpserver.h"
#include "acceptthread.h"
#include <QMainWindow>
#include <QSharedPointer>
#include <QMenu>
#include <QSettings>
#include <QNetworkInterface>
#include "QtNetwork/QHostAddress"
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //获取本机ip
    QString get_localmachine_ip();

    virtual void timerEvent(QTimerEvent *event);

    //获取本机时间
    void ShowData(QByteArray);

signals:
    void sig_SendData(QString strInfo);

    //槽
private slots:

    //开启服务
    void on_startserver_clicked();
    //关闭服务
    void on_stopserver_clicked();

    //右键
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    //上线信息
    void slot_ClientOnline(QSharedPointer<ClientSession> ClientSession);

    //设置端口
    void on_lineEdit_2_returnPressed();

    //创建窗口
    void slot_CreateDialog(int,int);

    //处理数据
    void slot_ShowRecvData(int, int, QByteArray);

    //断开链接
    void slot_Disconnect(int s);

    
    //发送CMD命令
    void slot_cmd_clicked(SOCKET, QString);

    //本地时间更新
    void timerUpDate();

    //CMD按钮
    void on_cmdbutton_clicked();

    //屏幕按钮
    void on_screenbutton_clicked();

    //进程按钮
    void on_processbutton_clicked();

    //文件按钮
    void on_filebutton_clicked();

    //创建按钮
    void on_buiderbutton_clicked();

    //关于按钮
    void on_aboutbutton_clicked();

private:
    Ui::MainWindow *ui;

    //AcceptThread
    QSharedPointer<AcceptThread> m_pThread;

    bool m_isStart;
    QSharedPointer<QMenu> m_menu;
    TCPServer m_Server;

    //设置端口
    QString m_Port;
    iocpserver m_IocpServer;
    int m_nRow;
};

#endif // MAINWINDOW_H



