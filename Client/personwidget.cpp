#include "personwidget.h"
#include <QDir>
#include <QMessageBox>

PersonWidget::PersonWidget(QWidget *parent)
    : QWidget{parent}
{}

PersonWidget::PersonWidget(TcpClient*tcp,QWidget *parent,User u,int _v)
    : QWidget{parent},val(_v),user(u)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    setWindowIcon(QIcon(":/res/icon.jpg"));
    tool = new ToolWindow(val,tcp,this,user) ;
    tool->setHidden(true) ;

    if(val < 0)
        tool->hhide();
    //改变人物
    connect(tool,&ToolWindow::changemodel,[=](QString role){
        init(role) ;
    }) ;
    init(user.getRole()) ;
}

void PersonWidget::init(QString role)
{
    file_url = url + role ;

    this->setWindowFlag(Qt::FramelessWindowHint) ;  // 设置背景
    this->setAttribute(Qt::WA_TranslucentBackground) ;

    QDir tmpDir(file_url) ;
    QFileInfoList listInfo = tmpDir.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot,QDir::Name) ;

    clearList() ;
    for(QFileInfo imgFile : listInfo){  // 将所有符合信息的加入字符串列表
        QString filePath = imgFile.absoluteFilePath() ;
        if(filePath.contains("air")) airList.append(filePath) ;
        else if(filePath.contains("relax")) relaxList.append(filePath) ;
        else if(filePath.contains("shime")) shimeList.append(filePath) ;
        else if(filePath.contains("sit")) siteList.append(filePath) ;
        else if(filePath.contains("special")) specialList.append(filePath) ;
        else if(filePath.contains("stand")) standList.append(filePath) ;
    }

    imgList = &specialList ;
}

void PersonWidget::showPerson()
{
    //stopAE() ;
    totIdx = 0 ;
    this->show() ;
    startAE() ;
}

void PersonWidget::paintEvent(QPaintEvent * e)
{
    Q_UNUSED(e) ;

    if(totIdx > (imgList->size() - 1))  // 图片下标超过了
        return ;

    QPainter painter(this) ;
    painter.drawImage(QPointF(0,0),QImage(imgList->at(totIdx))) ;

    totIdx ++ ;
    totIdx = (totIdx) % imgList->size() ;
}

void PersonWidget::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e) ;
    this->update() ;
}

void PersonWidget::startAE()
{
    personFlashTime = startTimer(200) ;
}

void PersonWidget::stopAE()
{
    if(0 == personFlashTime)
        return ;

    killTimer(personFlashTime);
    personFlashTime = 0;
}

void PersonWidget::air()
{
    this->totIdx = 0;
    imgList = &this->airList ;
}

void PersonWidget::relaxed()
{
    totIdx = 0 ;
    imgList = &this->relaxList ;
}

void PersonWidget::shime()
{
    this->totIdx = 0 ;
    imgList = &this->shimeList ;
}

void PersonWidget::site()
{
    this->totIdx = 0 ;
    imgList = &this->siteList ;
}

void PersonWidget::special()
{
    this->totIdx = 0 ;
    imgList = &this->specialList ;
}

void PersonWidget::stand()
{
    this->totIdx = 0 ;
    imgList = &this->standList ;
}


void PersonWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint EndPos;
    if(event->buttons()==Qt::LeftButton){
        EndPos = event->globalPosition().toPoint() - StartPos;
        this->move(EndPos);
    }
}

void PersonWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton){
        //记录窗口此时的坐标
        this->air() ;
        StartPos = event->globalPosition().toPoint() - this->frameGeometry().topLeft();
    }
}

void PersonWidget::mouseReleaseEvent(QMouseEvent *event)
{
    this->stand() ;
    this->changeHaogan(val) ;
}


void PersonWidget::mouseDoubleClickEvent(QMouseEvent *event)
{

    if(tool->isHidden())
        tool->show() ;
    else
        tool->setHidden(true) ;
}

void PersonWidget::changeHaogan(int val)
{
    user.setFavorability(user.getFavorability() + val);

    if(user.getFavorability() > 100)
        user.setFavorability(100);
    if(user.getFavorability() < 0)
        user.setFavorability(0);

    tool->updataHaogan(user.getFavorability()) ;
}

void PersonWidget::clearList()
{
    airList.clear() ;
    relaxList.clear() ;
    shimeList.clear() ;
    siteList.clear();
    specialList.clear() ;
    standList.clear() ;
}
