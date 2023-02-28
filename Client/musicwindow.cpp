#include "musicwindow.h"
#include "ui_musicwindow.h"
#include "QPushButton"

musicWindow::musicWindow(TcpClient*tcp,User u,int _v,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::musicWindow),val(_v),user(u)
{
    setWindowIcon(QIcon(":/res/icon.jpg"));
    ui->setupUi(this);
    per = new PersonWidget(tcp,nullptr,user,val) ;
    qrand = new QRandomGenerator() ;
    startTimer(5000) ;
}

musicWindow::~musicWindow()
{
    delete ui;
}

void musicWindow::showMusicWindow()
{
    per->showPerson() ;
}

void musicWindow::timerEvent(QTimerEvent *event)
{
    int totidx = qrand->bounded(0,4) ;

    if(totidx == 0) per->relaxed() ;
    else if(totidx == 1) per->site() ;
    else if(totidx == 2) per->special() ;
    else if(totidx == 3) per->stand() ;
}
