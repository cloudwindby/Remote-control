#ifndef SCREENDIALOG_H
#define SCREENDIALOG_H
#include <QImage>
#include <QPixmap>
#include "../common/macro.h"
#include <QPainter>
#include <QDialog>
#include <QFileDialog>

namespace Ui {
class ScreenDialog;
}

class ScreenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenDialog(SOCKET s,QWidget *parent = nullptr);
    ~ScreenDialog();

    void ShowData(QByteArray);
    virtual void paintEvent(QPaintEvent*);
    virtual void closeEvent(QCloseEvent *);
    void SetSendData(bool);

private slots:
    void on_pushButton_clicked();

private:
    Ui::ScreenDialog *ui;
    QImage m_ImgFromClient;
    QImage m_ScaledImg;
    QPixmap m_Pixmap;
    bool m_bDraw;
    bool m_bSendData;
    SOCKET m_ClientSocket;
};

#endif // SCREENDIALOG_H
