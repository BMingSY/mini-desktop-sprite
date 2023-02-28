#include "mainwindow.h"
#include "login.h"
#include "chat.h"
#include "registered.h"
#include "tcpclient.h"
#include <QDebug>
#include <QApplication>

#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //mainwindow Test
    new MainWindow();

    return a.exec();
}

