#include "registered.h"
#include "ui_registered.h"

registered::registered(TcpClient*tcp,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registered)
{
    ui->setupUi(this);

    setFixedSize(this->width(),this->height());
    setWindowTitle("注册");
    setWindowIcon(QIcon(":/res/icon.jpg"));

    connect(ui->co,&QPushButton::clicked,[=](){
        QString user_id = ui->phoneNumber->text();
        QString password1 = ui->password_2->text();
        QString password2 = ui->conPassword->text();
        QString mailNumber = ui->mailNumber->text();
        QString name = ui->name->text();

        auto check=[&](QString id){
                for(auto i : id)
                    if(i < '0' || i >'9')return false;
                return true;
        };

        //检测手机号输入是否合法(11位数字)
        if(user_id.size() != 11 || !check(user_id)){
            QMessageBox::critical(this,"错误","手机号非法");
            return ;
        }
        GetUser* gu = new GetUser;
        strcpy(gu->user_id,user_id.toLatin1().data());
        //检测是否已经有此手机号
        tcp->SendData(gu);
        tcp->RecvData();
        GetUserResult * gur = (GetUserResult*)tcp->res;

        if(gur -> f)
            QMessageBox::critical(this, "错误","此手机号已注册");
        else if(user_id == "" || password1 == "" || mailNumber == "" || name == "")
            QMessageBox::critical(this, "错误","请填完所有项");
        else if(password1 != password2)
            QMessageBox::critical(this,"错误","两次密码不一致");
        else {
            Cmd * cc = new Cmd;
            strcpy(cc->_cmd,QString("insert into user(user_id,password,coins,phoneNumber,mailNumber,name,favorability,role) values('%1','%2','0','%3','%4','%5','0','amiya')").arg(user_id).arg(password1).arg(user_id).arg(mailNumber).arg(name).toLatin1().data());
            tcp->SendData(cc);
            strcpy(cc->_cmd, QString("insert into friends(id1,id2) values('%1','admin')").arg(user_id).toLatin1().data());
            tcp->SendData(cc);
            strcpy(cc->_cmd,QString("insert into chat(send_user_id,receive_user_id,content) values('admin','%1','%2')").arg(user_id).arg("欢迎使用这个软件，有什么问题或者建议可以直接给管理员留言哦qwq").toLatin1().data());
            tcp->SendData(cc);
            QMessageBox::information(this,"提示","注册成功！");

           emit this->regist();
        }
    });

}

registered::~registered()
{
    delete ui;
}
