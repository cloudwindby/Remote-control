/********************************************************************************
** Form generated from reading UI file 'screendialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREENDIALOG_H
#define UI_SCREENDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ScreenDialog
{
public:
    QPushButton *pushButton;

    void setupUi(QDialog *ScreenDialog)
    {
        if (ScreenDialog->objectName().isEmpty())
            ScreenDialog->setObjectName(QStringLiteral("ScreenDialog"));
        ScreenDialog->resize(536, 480);
        pushButton = new QPushButton(ScreenDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(560, 250, 80, 20));

        retranslateUi(ScreenDialog);

        QMetaObject::connectSlotsByName(ScreenDialog);
    } // setupUi

    void retranslateUi(QDialog *ScreenDialog)
    {
        ScreenDialog->setWindowTitle(QApplication::translate("ScreenDialog", "Dialog", nullptr));
        pushButton->setText(QApplication::translate("ScreenDialog", "save image", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScreenDialog: public Ui_ScreenDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREENDIALOG_H
