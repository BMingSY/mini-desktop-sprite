#ifndef TOOLWINDOW_H
#define TOOLWINDOW_H

#include "chat.h"
#include <QWizardPage>

namespace Ui {
class ToolWindow;
}

class ToolWindow : public QWizardPage
{
    Q_OBJECT

public:
    explicit ToolWindow(int _v, TcpClient*tcp,QWidget *parent = nullptr,User u = User());

    ~ToolWindow();

    void updataHaogan(int haogan) ;
    Ui::ToolWindow *ui;
    Chat * chat;
    User user;
    void hhide();

signals:
    void changemodel(QString role) ;
};

#endif // TOOLWINDOW_H
