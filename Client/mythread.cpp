#include "mythread.h"
#include <QDebug>

MyThread::MyThread(TcpClient* tt,QObject *parent)
    : QObject{parent}
{
    tcp = tt;
    qDebug()<<"启动准备干活！！！！！！！！！";
}
void MyThread::dowork()
{
    while(true)
    {
        qDebug()<<"干活了";
        tcp->RecvData();
        if(!tcp->qm.empty() ) emit newMessage();
        Sleep(1000);
    }
}
