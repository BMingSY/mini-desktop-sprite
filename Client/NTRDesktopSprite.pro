QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chat.cpp \
    cwidgetae.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    musicwindow.cpp \
    mythread.cpp \
    personwidget.cpp \
    registered.cpp \
    tcpclient.cpp \
    toolwindow.cpp \
    user.cpp

HEADERS += \
    chat.h \
    cwidgetae.h \
    login.h \
    mainwindow.h \
    musicwindow.h \
    mythread.h \
    personwidget.h \
    registered.h \
    tcpclient.h \
    toolwindow.h \
    user.h

FORMS += \
    chat.ui \
    login.ui \
    mainwindow.ui \
    musicwindow.ui \
    registered.ui \
    toolwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc \
    res.qrc

SUBDIRS += \
    ntr.pro \
    ntr.pro

DISTFILES += \
    NTRDesktopSprite.pro.user \
    ntr.pro.user

RC_FILE=logo/logo.rc

LIBS += -lWs2_32
