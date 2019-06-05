#ifndef CMDDIALOG_H
#define CMDDIALOG_H
#include <QDialog>
#include "../common/macro.h"
#include <QTextCodec>
namespace Ui {
class CMDDialog;
}

class CMDDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDDialog(SOCKET sClient,QWidget *parent = nullptr);
    ~CMDDialog();

    void ShowData(QByteArray);

signals:
    void  on_cmd_clicked(SOCKET, QString);

private slots:
    void on_sendcmd_clicked();

private:
    Ui::CMDDialog *ui;
    SOCKET m_ClientSocket;
};

#endif // CMDDIALOG_H
