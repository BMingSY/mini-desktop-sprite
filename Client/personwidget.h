#ifndef PERSONWIDGET_H
#define PERSONWIDGET_H

#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include "toolwindow.h"

class PersonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PersonWidget(QWidget *parent = nullptr);

    PersonWidget(TcpClient*,QWidget *parent = nullptr,User u = User(),int _val = 1) ;

    void init(QString id) ;  // 初始化路径等信息

    void showPerson() ; // 开始展示

    void paintEvent(QPaintEvent * e) ;  // 绘画图片

    void timerEvent(QTimerEvent *e);   // 这个反复循环的事件去干什么

    void startAE() ;                   // 设置循环事件的单位步长

    void stopAE();                    // 停止去循环

    void air() ;                     // air 动作

    void relaxed() ;                 // relaxed 动作

    void shime() ;                  // shime动作

    void site()  ;                // site动作

    void special() ;                // special动作

    void stand() ;                  // stand动作

    void setName(const QString &newName);

    void mouseMoveEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event) ;

    void changeHaogan(int val) ;

    void clearList() ;

    ToolWindow * tool ;
private:
    QString url = ":/res/img/" ;
    int val ;

    User user;

    QString file_url ;
    QStringList airList,relaxList,shimeList,siteList,specialList,standList;
    QStringList *imgList ;
    int totIdx,personFlashTime ;
    QPoint StartPos ;  // 记录窗口坐标，为了实现可以拖动

};

#endif // PERSONWIDGET_H
