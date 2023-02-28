#ifndef CHAT_H
#define CHAT_H

#include "user.h"
#include "mythread.h"
#include "tcpclient.h"

#include <QObject>
#include <map>
#include <QFile>
#include <queue>
#include <QTimer>
#include <vector>
#include <QThread>
#include <fstream>
#include <QDebug>
#include <QKeyEvent>
#include <QDateTime>
#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
class Chat;
}

struct message{
    QString dt;
    QString sid;
    QString content;
};

class Chat : public QMainWindow
{
    Q_OBJECT

public:
    explicit Chat(TcpClient*,QWidget *parent = nullptr,User u = User());

    ~Chat();
signals:
    void chating();
    void inviting(User);
    void newMessage(_Message);
private:
    Ui::Chat *ui;

    //user
    User rid, sid;
    //好友列表
    std::vector<User> vu;
    //id->key 找name ; name->key 找id
    std::map<QString,QString> idToName,nameToId;
    //sid rid
    void readMMP(QString,QString);
    void writeMMP(QString,QString,message);
    //接收消息的线程
    QThread * qth;
};

#endif // CHAT_H
