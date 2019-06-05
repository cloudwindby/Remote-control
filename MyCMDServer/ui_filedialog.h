/********************************************************************************
** Form generated from reading UI file 'filedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEDIALOG_H
#define UI_FILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>

QT_BEGIN_NAMESPACE

class Ui_filedialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QListWidget *clientFile;
    QLabel *label_2;
    QListWidget *serverFile;

    void setupUi(QDialog *filedialog)
    {
        if (filedialog->objectName().isEmpty())
            filedialog->setObjectName(QStringLiteral("filedialog"));
        filedialog->resize(640, 480);
        gridLayout = new QGridLayout(filedialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(filedialog);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        clientFile = new QListWidget(filedialog);
        clientFile->setObjectName(QStringLiteral("clientFile"));
        clientFile->setContextMenuPolicy(Qt::CustomContextMenu);

        gridLayout->addWidget(clientFile, 1, 0, 1, 1);

        label_2 = new QLabel(filedialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        serverFile = new QListWidget(filedialog);
        serverFile->setObjectName(QStringLiteral("serverFile"));

        gridLayout->addWidget(serverFile, 3, 0, 1, 1);


        retranslateUi(filedialog);
        QObject::connect(clientFile, SIGNAL(customContextMenuRequested(QPoint)), filedialog, SLOT(on_customContextMenuRequested(QPoint)));

        QMetaObject::connectSlotsByName(filedialog);
    } // setupUi

    void retranslateUi(QDialog *filedialog)
    {
        filedialog->setWindowTitle(QApplication::translate("filedialog", "Dialog", nullptr));
        label->setText(QApplication::translate("filedialog", "client:", nullptr));
        label_2->setText(QApplication::translate("filedialog", "server:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class filedialog: public Ui_filedialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEDIALOG_H
