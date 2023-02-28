#include "user.h"

User::User(){}
User::User(QString _id, QString _pa, QString _pn, QString _en, QString _na, QString _ro, int _co, int _fa):
    id(_id),password(_pa),phoneNumber(_pn),mailNumber(_en),name(_na),role(_ro),coins(_co),favorability(_fa){}
User::~User(){}

const QString &User::getId() const
{
    return id;
}

void User::setId(const QString &newId)
{
    id = newId;
}

const QString &User::getPassword() const
{
    return password;
}

void User::setPassword(const QString &newPassword)
{
    password = newPassword;
}

const QString &User::getPhoneNumber() const
{
    return phoneNumber;
}

void User::setPhoneNumber(const QString &newPhoneNumber)
{
    phoneNumber = newPhoneNumber;
}

const QString &User::getMailNumber() const
{
    return mailNumber;
}

void User::setMailNumber(const QString &newMailNumber)
{
    mailNumber = newMailNumber;
}

const QString &User::getName() const
{
    return name;
}

void User::setName(const QString &newName)
{
    name = newName;
}

int User::getCoins() const
{
    return coins;
}

void User::setCoins(int newCoins)
{
    coins = newCoins;
}

const QString &User::getRole() const
{
    return role;
}

void User::setRole(const QString &newRole)
{
    role = newRole;
}

int User::getFavorability() const
{
    return favorability;
}

void User::setFavorability(int newFavorability)
{
    favorability = newFavorability;
}
