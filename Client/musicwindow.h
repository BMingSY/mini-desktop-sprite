#ifndef MUSICWINDOW_H
#define MUSICWINDOW_H

#include <QMainWindow>
#include "personwidget.h"
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class musicWindow; }
QT_END_NAMESPACE

class musicWindow : public QMainWindow
{
    Q_OBJECT

public:
    musicWindow(TcpClient*tcp,User u,int _v,QWidget *parent = nullptr);
    ~musicWindow();

    void showMusicWindow() ;

    void timerEvent(QTimerEvent *event) ;

    PersonWidget *per;
private:
    Ui::musicWindow *ui;
    QRandomGenerator * qrand ;
    int val;
    User user;
};
#endif // MUSICWINDOW_H
