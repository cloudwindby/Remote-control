#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>
#include "../common/macro.h"
#include <QByteArray>
#include <QListWidgetItem>
#include <QMenu>
#include <QSharedPointer>
#include <QFile>

namespace Ui {
class filedialog;
}

class filedialog : public QDialog
{
    Q_OBJECT

public:
    explicit filedialog(SOCKET client,QWidget *parent = nullptr);
    ~filedialog();

    void ShowData(QByteArray data);
    void FileInit(QByteArray data);
    void RecvFileHeader(QByteArray data);
    void RecvFileByClientSend(QByteArray data);

public slots:
    void slot_itemDoubleClicked(QListWidgetItem *);
    void slot_download();
    void on_customContextMenuRequested(const QPoint &pos);

private:
    Ui::filedialog *ui;
    SOCKET m_client;

    QString m_strPath;

    QSharedPointer<QMenu> m_menu;
    QFile m_File;
    FileHeader m_Header;
    unsigned int m_nCurWritten;
};

#endif // FILEDIALOG_H
