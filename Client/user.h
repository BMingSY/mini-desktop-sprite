#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();
    //账号 密码 电话 邮箱 昵称 角色 积分 好感度
    User(QString,QString,QString,QString,QString,QString,int,int);
    ~User();


    const QString &getId() const;
    void setId(const QString &newId);

    const QString &getPassword() const;
    void setPassword(const QString &newPassword);

    const QString &getPhoneNumber() const;
    void setPhoneNumber(const QString &newPhoneNumber);

    const QString &getMailNumber() const;
    void setMailNumber(const QString &newMailNumber);

    const QString &getName() const;
    void setName(const QString &newName);

    int getCoins() const;
    void setCoins(int newCoins);

    const QString &getRole() const;
    void setRole(const QString &newRole);

    int getFavorability() const;
    void setFavorability(int newFavorability);

private:
    QString id,password,phoneNumber,mailNumber,name,role;
    int coins,favorability;
};



#endif // USER_H
