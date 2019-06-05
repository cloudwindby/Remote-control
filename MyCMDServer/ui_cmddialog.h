/********************************************************************************
** Form generated from reading UI file 'cmddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CMDDIALOG_H
#define UI_CMDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_CMDDialog
{
public:
    QGridLayout *gridLayout;
    QTextBrowser *cmdText;
    QLineEdit *cmdEdit;
    QPushButton *sendcmd;

    void setupUi(QDialog *CMDDialog)
    {
        if (CMDDialog->objectName().isEmpty())
            CMDDialog->setObjectName(QStringLiteral("CMDDialog"));
        CMDDialog->resize(600, 400);
        CMDDialog->setMinimumSize(QSize(600, 400));
        gridLayout = new QGridLayout(CMDDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        cmdText = new QTextBrowser(CMDDialog);
        cmdText->setObjectName(QStringLiteral("cmdText"));
        cmdText->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
        cmdText->setReadOnly(false);

        gridLayout->addWidget(cmdText, 0, 0, 1, 2);

        cmdEdit = new QLineEdit(CMDDialog);
        cmdEdit->setObjectName(QStringLiteral("cmdEdit"));

        gridLayout->addWidget(cmdEdit, 1, 0, 1, 1);

        sendcmd = new QPushButton(CMDDialog);
        sendcmd->setObjectName(QStringLiteral("sendcmd"));

        gridLayout->addWidget(sendcmd, 1, 1, 1, 1);


        retranslateUi(CMDDialog);

        QMetaObject::connectSlotsByName(CMDDialog);
    } // setupUi

    void retranslateUi(QDialog *CMDDialog)
    {
        CMDDialog->setWindowTitle(QApplication::translate("CMDDialog", "Dialog", nullptr));
        sendcmd->setText(QApplication::translate("CMDDialog", "Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CMDDialog: public Ui_CMDDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMDDIALOG_H
