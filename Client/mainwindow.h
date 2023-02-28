#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpclient.h"
#include "musicwindow.h"
#include "login.h"
#include "personwidget.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>
#include <QPushButton>
#include <QPixmap>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void login(TcpClient *tcp) ;

private:
    Ui::MainWindow *ui;
    Login * lg ;
    musicWindow *mu,*fmu ;

};
#endif // MAINWINDOW_H
