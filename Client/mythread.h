#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "tcpclient.h"
#include <QObject>

class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(TcpClient *,QObject *parent = nullptr);

public slots:
    void dowork();
signals:
    void newMessage();
private:
    TcpClient* tcp;
};

#endif // MYTHREAD_H
