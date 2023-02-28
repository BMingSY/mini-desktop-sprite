#include "chat.h"
#include "ui_chat.h"

Chat::Chat(TcpClient* tcp,QWidget *parent,User u) :
    QMainWindow(parent),ui(new Ui::Chat),rid(u)
{
    ui->setupUi(this);

    setFixedSize(this->width(),this->height());
    setWindowTitle("聊天");

    ui->recBox->setReadOnly(true);

    //载入好友列表
    GetFriends * gf = new GetFriends;
    strcpy(gf->user_id,rid.getId().toLatin1().data());
    tcp->SendData(gf);
    ui->friendList->clear();
    std::vector<QString> vqs;
    while(true)
    {
        tcp->RecvData();
        GetFriendsResult * gfr = (GetFriendsResult*)tcp->res;
        if(!gfr->f) break;
        if(strcmp(gfr->friends,rid.getId().toLatin1().data()) == 0)
            continue;
        vqs.push_back(gfr->friends);
    }

    //查询好友信息
    for(auto i : vqs)
    {
        GetUser* gu = new GetUser;
        strcpy(gu->user_id,i.toLatin1().data());
        tcp->SendData(gu);
        tcp->RecvData();
        GetUserResult* gur =(GetUserResult*)tcp->res;
        User sd(i,QString(gur->password),QString(gur->phoneNumber),QString(gur->mailNumber),QString(gur->name),QString(gur->role),gur->coins,gur->favorability);
        vu.push_back(sd);
        idToName[sd.getId()] = sd.getName();
        nameToId[sd.getName()] = sd.getId();
    }

    //显示好友列表
    for(auto i : vu)
        ui->friendList->addItem(i.getName());
    //设置默认好友选项
    ui->friendList->setCurrentRow(0);
    sid = vu[0];

    //设置选中事件
    connect(ui->friendList,&QListWidget::itemSelectionChanged,[=](){
        sid = vu[ui->friendList->currentRow()];
        //readMMP(sid.getId(),rid.getId());
    });

    //邀请好友来
    ui->inviteBtn->setText("邀\n请\n好\n友\n角\n色");
    connect(ui->inviteBtn,&QPushButton::clicked,[=](){
        vu.clear() ;
        emit inviting(vu[ui->friendList->currentRow()]);
        this->hide();
    });

    //********************更新留言板***************

    //群发消息
    connect(ui->sendBtn,&QPushButton::clicked,[=](){
        QString tmp = ui->sendBox->toPlainText();
        if(tmp == "")return ;

        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss ddd");

        _Message *mes = new _Message;

        strcpy(mes->date,current_date.toLatin1().data());
        strcpy(mes->message,tmp.toUtf8().data());
        strcpy(mes->sid,rid.getName().toLatin1().data());

        tcp->SendData(mes);

        ui->sendBox->clear();
    });

    //开新线程接收消息
    MyThread * th = new MyThread(tcp);
    qth = new QThread(this);
    th->moveToThread(qth);

    connect(qth,&QThread::finished,qth,&QThread::deleteLater);
    connect(ui->sendBtn,&QPushButton::clicked,th,&MyThread::dowork);
    connect(th,&MyThread::newMessage,[=](){
        // tcp->needUpdate = 0;
        // ui->onlineNumber->setText(QString::number(tcp->number));

        //刷新当前窗口的消息同时发送信号给上一个窗口
        while(!tcp->qm.empty())
        {
            _Message mas = tcp->qm.front();
            tcp->qm.pop();
            ui->recBox->append(QString(mas.date) + "------------>"+QString(mas.sid));
            ui->recBox->append(mas.message);
            ui->recBox->moveCursor(QTextCursor::EndOfLine );
            emit newMessage(mas);
        }
    });
    qth->start();

    //关闭
    connect(ui->closed,&QPushButton::clicked,[=](){
        emit this->chating();
        this->hide();
    });

    //添加好友
    connect(ui->addFriendBtn,&QPushButton::clicked,[=](){
        QString friend_user_id = ui->addFriendE->text();

        int r = 0, cnt = 0;
        while(r < ui->friendList->count()) if(nameToId[ui->friendList->item(r ++)->text()] == friend_user_id){cnt ++;break;}
        if(cnt)
        {
            QMessageBox::critical(this,"错误","你已经和他是好友了");
            return ;
        }
        GetUser * gu = new GetUser;
        strcpy(gu->user_id,friend_user_id.toLatin1().data());
        tcp->SendData(gu);
        tcp->RecvData();
        GetUserResult * gur = (GetUserResult*)tcp->res;
        if(!gur->f)
            QMessageBox::critical(this,"错误","没有这个用户");
        else
        {
            User sd(friend_user_id,gur->password,gur->phoneNumber,gur->mailNumber,gur->name,gur->role,gur->coins,gur->favorability);
            ui->friendList->addItem(sd.getName());
            vu.push_back(sd);
            idToName[sd.getId()] = sd.getName();
            nameToId[sd.getName()] = sd.getId();
            Cmd*cc = new Cmd;
            strcpy(cc->_cmd,QString("insert into friends(id1,id2) values('%1','%2')").arg(rid.getId()).arg(friend_user_id).toLatin1().data());
            tcp->SendData(cc);

            QMessageBox::information(this,"okkkk","添加完了");

        }
    });

}

void Chat::readMMP(QString sid ,QString rid)
{
    QString fileName = QString("./data/%1_%2.dat").arg(sid).arg(rid);
    ifstream file(fileName.toLatin1().data(),ios::in| ios::binary);
    message ms;

    while(!file.eof())
    {
        file.read(reinterpret_cast<char*>(&ms),sizeof(ms));
        qDebug()<<ms.dt<<"------->"<<ms.sid;
        qDebug()<<ms.content;
    }

    file.close();
}
void Chat::writeMMP(QString sid,QString rid,message ms)
{
    QString fileName = QString("./data/%1_%2.dat").arg(sid).arg(rid);
    //qDebug()<<fileName.toLatin1().data();
    ofstream file(fileName.toLatin1().data(),ios::out| ios::binary|ios::app);

    file.write(reinterpret_cast<char*>(&ms),sizeof(ms));

    file.close();
}

Chat::~Chat()
{
    qth->quit();
    qth->wait();
    delete ui;
}


