#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chat.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //tcp connect
    TcpClient* tcp = new TcpClient();
    tcp->Connect("47.97.105.213",8888);

    //设置版本2.1
    const char* version = "2.1";
    //检查版本
    Version* ve = new Version;
    strcpy(ve->version,"qwq");
    tcp->SendData(ve);
    tcp->RecvData();
    ve = (Version*)tcp->res;

    if(strcmp(ve->version,"qwq") == 0)
        QMessageBox::information(this,"提示","网络未连接，请重试");
    else if(strcmp(version,ve->version))
        QMessageBox::information(this,"提示","已有最新版本，请更新为最新版");
    else {
        setWindowIcon(QIcon(":/res/icon.jpg"));
        login(tcp);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::login(TcpClient* tcp)
{
    this->hide();
    lg = new Login(tcp) ;
    lg->show() ;

    connect(lg,&Login::islogin,[=](User u1){
        lg->hide();
        mu = new musicWindow(tcp,u1,1,nullptr) ;
        mu->showMusicWindow() ;
        connect(mu->per->tool->chat,&Chat::inviting,[=](User u2){
            fmu = new musicWindow(tcp,u2,-1,this);
            fmu->showMusicWindow();
        });
    });
}

