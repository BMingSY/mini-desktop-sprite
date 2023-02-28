#include "toolwindow.h"
#include "ui_toolwindow.h"
#include <QPushButton>
#include <QDebug>
#include <windows.h>
#include <locale>
#include <tchar.h>
#include <QComboBox>
#include <shellapi.h>

ToolWindow::ToolWindow(int _v,TcpClient*tcp,QWidget *parent,User u) :
    QWizardPage(parent),ui(new Ui::ToolWindow),user(u)
{
    setWindowIcon(QIcon(":/res/icon.jpg"));
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint) ;
    this->setAttribute(Qt::WA_TranslucentBackground) ;

    ui->progressBar->setValue(user.getFavorability()) ;

    //关闭按钮
    connect(ui->closetool,&QPushButton::clicked,parent,[=](){
        //程序退出更新数据库中的人物好感
        Cmd * cc = new Cmd;
        strcpy(cc->_cmd,QString("update user set favorability='%1' where user_id = '%2'").arg(user.getFavorability()).arg(user.getId()).toLatin1().data());
        tcp->SendData(cc);
        parent->close() ;
    });

    // 通过样式去除边框;
    ui->closetool->setFont(QFont("songti",30,30)) ;
    ui->closetool->setStyleSheet("border:none;color: red");
    ui->jietu->hide() ;
    ui->paint->hide() ;

    if(_v < 0) return ;

    chat = new Chat(tcp,this,user) ;
    ui->chatMessage->show();

    //消息框
    connect(chat,&Chat::newMessage,[=](_Message mas){
        ui->chatMessage->setStyleSheet("color: red");
        ui->chatMessage->setText(QString(mas.sid)+" : "+QString(mas.message));
    });

    ui->comboBox->hide() ;
    //添加可选内容
    ui->comboBox->addItem("amiya") ;
    ui->comboBox->addItem("Lancet-2") ;
    ui->comboBox->addItem("chen");
    ui->comboBox->addItem("aiyafala");
    ui->comboBox->addItem("ansaier");
    ui->comboBox->addItem("dekesasi");
    ui->comboBox->addItem("yunxing");
    ui->comboBox->addItem("hemo");
    ui->comboBox->addItem("hei");
    ui->comboBox->addItem("hong");
    ui->comboBox->addItem("jiexika");
    ui->comboBox->addItem("xingxiong");
    ui->comboBox->addItem("nengtianshi");
    ui->comboBox->addItem("mositima");
    ui->comboBox->addItem("yifulite");
    ui->comboBox->addItem("kong");
    ui->comboBox->addItem("lapulande");
    ui->comboBox->addItem("maizhelun");
    ui->comboBox->addItem("saileiya");
    ui->comboBox->addItem("sikadi");
    ui->comboBox->setCurrentText(user.getRole());

    //好友按钮
    connect(ui->friends,&QPushButton::clicked,[=](){
        this->hide();
        chat->show();
        connect(chat,&Chat::chating,[=](){
            this->show();
        });
    });


    //更换角色按钮
    connect(ui->change,&QPushButton::clicked,ui->comboBox,[&](){
        ui->comboBox->show() ;
    });

    //下拉框
    connect(ui->comboBox,&QComboBox::currentIndexChanged,[=](){
        emit changemodel(ui->comboBox->currentText()) ;
        Cmd * cc = new Cmd;
        strcpy(cc->_cmd,QString("update user set role='%1' where user_id = '%2'").arg(ui->comboBox->currentText()).arg(user.getId()).toLatin1().data());
        tcp->SendData(cc);
        ui->comboBox->hide();
    });

    // 工具按钮事件
    connect(ui->tools,&QPushButton::clicked,[=](){
        ui->jietu->show();
        ui->paint->show() ;
    }) ;

    // 截图按钮事件
    connect(ui->jietu,&QPushButton::clicked,[=](){
        ShellExecuteA(nullptr, "open", "SnippingTool", nullptr, nullptr, SW_SHOW);
    });

    // 绘图按钮事件
    connect(ui->paint,&QPushButton::clicked,[=](){
        ShellExecuteA(nullptr, "open", "mspaint", nullptr, nullptr, SW_SHOW);
    });


}

void ToolWindow::hhide()
{
    ui->change->hide();
    ui->friends->hide();
    ui->tools->hide();
    ui->comboBox->hide();
    ui->jietu->hide();
    ui->paint->hide();

}
ToolWindow::~ToolWindow()
{
    delete ui;
}

void ToolWindow::updataHaogan(int haogan)
{
    user.setFavorability(haogan);
    ui->progressBar->setValue(haogan) ;
}
