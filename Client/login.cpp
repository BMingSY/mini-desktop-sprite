#include "login.h"
#include "ui_login.h"
#include <QDebug>

Login::Login(TcpClient * tcp,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    setFixedSize(this->width(),this->height());
    setWindowIcon(QIcon(":/res/icon.jpg"));

    setWindowTitle("登录");
    //初始化账号密码登录
    ifstream ifs("login.dat");
    if(ifs.is_open())
    {
        //qDebug() <<"------------------------->111111";
        string id , password;
        ifs >> id >> password;
        ui->idE->setText(QString::fromStdString(id));
        ui->passwordE->setText(QString::fromStdString(password));
    }
    ifs.close();
    //登录
    connect(ui->enter,&QPushButton::clicked,[=](){

        QString id = ui->idE->text();
        QString password = ui->passwordE->text();
        LOGIN * lo = new LOGIN;
        strcpy(lo->user_id,id.toLatin1().data());
        strcpy(lo->password,password.toLatin1().data());

        tcp->SendData(lo);
        tcp->RecvData();
        LoginResult* lr = (LoginResult*)tcp->res;

        if(lr->result == 2){
            QMessageBox::critical(this,"错误","没有这个账号，请先注册");
            return ;
        }
        if(lr->result == 0)
        {
            QMessageBox::critical(this,"错误","密码错误");
            return ;
        }
        User user;
        GetUser * gu = new GetUser;
        strcpy(gu->user_id,id.toLatin1().data());
        tcp->SendData(gu);
        tcp->RecvData();
        GetUserResult * gur = (GetUserResult*)tcp->res;

        user.setId(gur->user_id);
        user.setPassword(gur->password);
        user.setCoins(gur->coins);
        user.setPhoneNumber(gur->phoneNumber);
        user.setMailNumber(gur->mailNumber);
        user.setName(gur->name);
        user.setFavorability(gur->favorability);
        user.setRole(gur->role);

        //保存账号密码
        ofstream ofs("login.dat");
        if(ofs.is_open())
            ofs<<user.getId().toStdString()<<" "<<user.getPassword().toStdString()<<endl;
        //qDebug()<<user.getId()<<" "<<user.getPassword()<<" "<<user.getRole();
        ofs.close();

        emit this->islogin(user);
    });

    //注册
    connect(ui->registered,&QPushButton::clicked,[=](){
        re = new registered(tcp);
        this->hide();
        re->show();
        connect(re,&registered::regist,[=](){
            re->hide();
            this->show();
        });
    });
    //忘记密码
    connect(ui->forgotPassword,&QPushButton::clicked,[=](){
        QMessageBox::information(this,"namo","还没有这个功能呢，请换个手机号再注册一遍吧。");
    });
}

Login::~Login()
{
    delete ui;
}
