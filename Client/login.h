#ifndef LOGIN_H
#define LOGIN_H

#include "registered.h"
#include <QWidget>
#include <QString>
#include <fstream>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(TcpClient*,QWidget *parent = nullptr);
    ~Login();

signals:
    void islogin(User user);
private:
    Ui::Login *ui;
    registered *re;
    //User user;
};

#endif // LOGIN_H
