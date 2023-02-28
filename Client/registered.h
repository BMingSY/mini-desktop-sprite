#ifndef REGISTERED_H
#define REGISTERED_H

#include "user.h"
#include "tcpclient.h"

#include <QDebug>
#include <QWidget>
#include <QMessageBox>


namespace Ui {
class registered;
}

class registered : public QWidget
{
    Q_OBJECT

public:
    explicit registered(TcpClient*,QWidget *parent = nullptr);
    ~registered();

signals:
    void regist();

private:
    Ui::registered *ui;

};

#endif // REGISTERED_H
