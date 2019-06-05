/********************************************************************************
** Form generated from reading UI file 'processdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROCESSDIALOG_H
#define UI_PROCESSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_processdialog
{
public:
    QGridLayout *gridLayout;
    QTableWidget *processtable;

    void setupUi(QDialog *processdialog)
    {
        if (processdialog->objectName().isEmpty())
            processdialog->setObjectName(QStringLiteral("processdialog"));
        processdialog->resize(733, 581);
        gridLayout = new QGridLayout(processdialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        processtable = new QTableWidget(processdialog);
        if (processtable->columnCount() < 5)
            processtable->setColumnCount(5);
        processtable->setObjectName(QStringLiteral("processtable"));
        processtable->setMinimumSize(QSize(700, 550));
        processtable->setMaximumSize(QSize(1080, 960));
        processtable->setContextMenuPolicy(Qt::CustomContextMenu);
        processtable->setColumnCount(5);
        processtable->horizontalHeader()->setDefaultSectionSize(150);
        processtable->horizontalHeader()->setStretchLastSection(true);

        gridLayout->addWidget(processtable, 0, 0, 1, 1);


        retranslateUi(processdialog);

        QMetaObject::connectSlotsByName(processdialog);
    } // setupUi

    void retranslateUi(QDialog *processdialog)
    {
        processdialog->setWindowTitle(QApplication::translate("processdialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class processdialog: public Ui_processdialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROCESSDIALOG_H
