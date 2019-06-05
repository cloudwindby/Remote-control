#-------------------------------------------------
#
# Project created by QtCreator 2018-11-02T01:25:20
#
#-------------------------------------------------

QT       += core gui \
          network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyCMDServer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    tcpserver.cpp \
    acceptthread.cpp \
    recvthread.cpp \
    cmddialog.cpp \
    screendialog.cpp \
    iocpserver.cpp \
    processdialog.cpp \
    filedialog.cpp

HEADERS += \
        mainwindow.h \
    tcpserver.h \
    acceptthread.h \
    recvthread.h \
    cmddialog.h \
    ../common/macro.h \
    screendialog.h \
    iocpserver.h \
    processdialog.h \
    filedialog.h

FORMS += \
        mainwindow.ui \
    cmddialog.ui \
    screendialog.ui \
    processdialog.ui \
    filedialog.ui

LIBS += -lws2_32 -L../common/ -lzlibstat

RESOURCES += \
    resource.qrc
	
RC_FILE = myicon.rc
