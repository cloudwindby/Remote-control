#include "mainwindow.h"
#include <QApplication>
MainWindow* g_pWindow;
int main(int argc, char *argv[])
{
    qRegisterMetaType<QSharedPointer<ClientSession>>("QSharedPointer<ClientSession>");
    qRegisterMetaType<QSharedPointer<ClientSession>>("QSharedPointer<ClientSession>&");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
