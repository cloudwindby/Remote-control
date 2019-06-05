#ifndef PROCESSDIALOG_H
#define PROCESSDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QSharedPointer>
#include "../common/macro.h"

namespace Ui {
class processdialog;
}

class processdialog : public QDialog
{
    Q_OBJECT

public:
    explicit processdialog(SOCKET s,QWidget *parent = nullptr);
    ~processdialog();
    void ShowData(QByteArray);

private slots:
    void on_processtable_customContextMenuRequested(const QPoint &pos);
    void slot_terminate();

private:
    Ui::processdialog *ui;
    QSharedPointer<QMenu> m_menu;
    SOCKET m_ClientSocket;
};

#endif // PROCESSDIALOG_H
