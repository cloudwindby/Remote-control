/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QPushButton *startserver;
    QPushButton *processbutton;
    QPushButton *exitbutton;
    QPushButton *filebutton;
    QPushButton *screenbutton;
    QPushButton *buiderbutton;
    QPushButton *aboutbutton;
    QLabel *addlabel;
    QComboBox *comboBox;
    QLabel *label;
    QTableWidget *tableWidget;
    QLineEdit *lineEdit_2;
    QLabel *statslabel;
    QLabel *timelabel;
    QLabel *portlabel;
    QPushButton *stopserver;
    QPushButton *cmdbutton;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(768, 421);
        MainWindow->setMinimumSize(QSize(768, 421));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        startserver = new QPushButton(centralWidget);
        startserver->setObjectName(QStringLiteral("startserver"));
        startserver->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(startserver->sizePolicy().hasHeightForWidth());
        startserver->setSizePolicy(sizePolicy);
        startserver->setMaximumSize(QSize(16777215, 16777215));
        startserver->setLayoutDirection(Qt::LeftToRight);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon/recourse/start.png"), QSize(), QIcon::Normal, QIcon::Off);
        startserver->setIcon(icon);
        startserver->setIconSize(QSize(48, 32));

        gridLayout->addWidget(startserver, 6, 18, 3, 1, Qt::AlignHCenter);

        processbutton = new QPushButton(centralWidget);
        processbutton->setObjectName(QStringLiteral("processbutton"));
        processbutton->setMinimumSize(QSize(72, 72));
        processbutton->setMaximumSize(QSize(82, 75));
        processbutton->setStyleSheet(QStringLiteral(""));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icon/recourse/process.png"), QSize(), QIcon::Normal, QIcon::Off);
        processbutton->setIcon(icon1);
        processbutton->setIconSize(QSize(72, 72));

        gridLayout->addWidget(processbutton, 0, 2, 1, 1);

        exitbutton = new QPushButton(centralWidget);
        exitbutton->setObjectName(QStringLiteral("exitbutton"));
        exitbutton->setMinimumSize(QSize(72, 72));
        exitbutton->setMaximumSize(QSize(82, 75));
        exitbutton->setStyleSheet(QStringLiteral(""));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icon/recourse/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        exitbutton->setIcon(icon2);
        exitbutton->setIconSize(QSize(72, 72));

        gridLayout->addWidget(exitbutton, 0, 6, 1, 1);

        filebutton = new QPushButton(centralWidget);
        filebutton->setObjectName(QStringLiteral("filebutton"));
        filebutton->setMinimumSize(QSize(72, 72));
        filebutton->setMaximumSize(QSize(82, 75));
        filebutton->setStyleSheet(QStringLiteral(""));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icon/recourse/file.png"), QSize(), QIcon::Normal, QIcon::Off);
        filebutton->setIcon(icon3);
        filebutton->setIconSize(QSize(72, 72));

        gridLayout->addWidget(filebutton, 0, 3, 1, 1);

        screenbutton = new QPushButton(centralWidget);
        screenbutton->setObjectName(QStringLiteral("screenbutton"));
        screenbutton->setMinimumSize(QSize(72, 72));
        screenbutton->setMaximumSize(QSize(82, 75));
        screenbutton->setStyleSheet(QStringLiteral(""));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icon/recourse/screen.png"), QSize(), QIcon::Normal, QIcon::Off);
        screenbutton->setIcon(icon4);
        screenbutton->setIconSize(QSize(72, 72));

        gridLayout->addWidget(screenbutton, 0, 1, 1, 1);

        buiderbutton = new QPushButton(centralWidget);
        buiderbutton->setObjectName(QStringLiteral("buiderbutton"));
        buiderbutton->setMinimumSize(QSize(72, 72));
        buiderbutton->setMaximumSize(QSize(82, 75));
        buiderbutton->setStyleSheet(QStringLiteral(""));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icon/recourse/builder.png"), QSize(), QIcon::Normal, QIcon::Off);
        buiderbutton->setIcon(icon5);
        buiderbutton->setIconSize(QSize(72, 72));

        gridLayout->addWidget(buiderbutton, 0, 4, 1, 1);

        aboutbutton = new QPushButton(centralWidget);
        aboutbutton->setObjectName(QStringLiteral("aboutbutton"));
        aboutbutton->setMinimumSize(QSize(72, 72));
        aboutbutton->setMaximumSize(QSize(82, 75));
        aboutbutton->setStyleSheet(QStringLiteral(""));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icon/recourse/about.png"), QSize(), QIcon::Normal, QIcon::Off);
        aboutbutton->setIcon(icon6);
        aboutbutton->setIconSize(QSize(72, 72));

        gridLayout->addWidget(aboutbutton, 0, 5, 1, 1);

        addlabel = new QLabel(centralWidget);
        addlabel->setObjectName(QStringLiteral("addlabel"));

        gridLayout->addWidget(addlabel, 0, 18, 1, 1);

        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        gridLayout->addWidget(comboBox, 1, 18, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 5, 18, 1, 1);

        tableWidget = new QTableWidget(centralWidget);
        if (tableWidget->columnCount() < 6)
            tableWidget->setColumnCount(6);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setMinimumSize(QSize(600, 300));
        tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        tableWidget->setFrameShadow(QFrame::Sunken);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setTextElideMode(Qt::ElideMiddle);
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(6);
        tableWidget->horizontalHeader()->setVisible(true);
        tableWidget->horizontalHeader()->setDefaultSectionSize(100);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setCascadingSectionResizes(false);
        tableWidget->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget->verticalHeader()->setStretchLastSection(false);

        gridLayout->addWidget(tableWidget, 2, 0, 10, 10);

        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setMaxLength(5);

        gridLayout->addWidget(lineEdit_2, 4, 18, 1, 1);

        statslabel = new QLabel(centralWidget);
        statslabel->setObjectName(QStringLiteral("statslabel"));
        statslabel->setMinimumSize(QSize(0, 0));
        statslabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(statslabel, 10, 18, 1, 1);

        timelabel = new QLabel(centralWidget);
        timelabel->setObjectName(QStringLiteral("timelabel"));
        timelabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(timelabel, 11, 18, 1, 1);

        portlabel = new QLabel(centralWidget);
        portlabel->setObjectName(QStringLiteral("portlabel"));

        gridLayout->addWidget(portlabel, 3, 18, 1, 1);

        stopserver = new QPushButton(centralWidget);
        stopserver->setObjectName(QStringLiteral("stopserver"));
        stopserver->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icon/recourse/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopserver->setIcon(icon7);
        stopserver->setIconSize(QSize(48, 32));

        gridLayout->addWidget(stopserver, 9, 18, 1, 1, Qt::AlignHCenter);

        cmdbutton = new QPushButton(centralWidget);
        cmdbutton->setObjectName(QStringLiteral("cmdbutton"));
        cmdbutton->setMinimumSize(QSize(72, 72));
        cmdbutton->setMaximumSize(QSize(82, 75));
        cmdbutton->setTabletTracking(false);
        cmdbutton->setStyleSheet(QStringLiteral(""));
        cmdbutton->setInputMethodHints(Qt::ImhNone);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icon/recourse/cmd.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdbutton->setIcon(icon8);
        cmdbutton->setIconSize(QSize(72, 72));

        gridLayout->addWidget(cmdbutton, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(exitbutton, SIGNAL(clicked()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        startserver->setText(QApplication::translate("MainWindow", "\345\274\200\345\220\257\346\234\215\345\212\241\345\231\250", nullptr));
#ifndef QT_NO_TOOLTIP
        processbutton->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>\346\230\276\347\244\272\350\277\233\347\250\213\344\277\241\346\201\257</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        processbutton->setText(QString());
#ifndef QT_NO_TOOLTIP
        exitbutton->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>\351\200\200\345\207\272\347\250\213\345\272\217</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        exitbutton->setText(QString());
#ifndef QT_NO_TOOLTIP
        filebutton->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>\346\230\276\347\244\272\346\226\207\344\273\266\344\277\241\346\201\257</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        filebutton->setText(QString());
#ifndef QT_NO_TOOLTIP
        screenbutton->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>\347\233\221\346\216\247\345\261\217\345\271\225</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        screenbutton->setText(QString());
#ifndef QT_NO_TOOLTIP
        buiderbutton->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>\347\224\237\346\210\220\345\256\242\346\210\267\347\253\257</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        buiderbutton->setText(QString());
#ifndef QT_NO_TOOLTIP
        aboutbutton->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>\345\205\263\344\272\216\346\234\254\347\250\213\345\272\217</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        aboutbutton->setText(QString());
        addlabel->setText(QApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250\346\211\200\346\234\211IP(IPV4):", nullptr));
        label->setText(QString());
        lineEdit_2->setInputMask(QApplication::translate("MainWindow", "00000", nullptr));
        lineEdit_2->setText(QApplication::translate("MainWindow", "10086", nullptr));
        statslabel->setText(QApplication::translate("MainWindow", "Server not Start", nullptr));
        timelabel->setText(QString());
        portlabel->setText(QApplication::translate("MainWindow", "\347\253\257\345\217\243:", nullptr));
        stopserver->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\346\234\215\345\212\241\345\231\250", nullptr));
#ifndef QT_NO_TOOLTIP
        cmdbutton->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>\345\217\221\351\200\201CMD\345\221\275\344\273\244</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        cmdbutton->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        cmdbutton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
