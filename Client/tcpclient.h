#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#define WIN32_LEAN_AND_MEAN
#include<winSock2.h>
#include<windows.h>

#include <queue>
#include <vector>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

enum{
    CMD_LOGIN,
    CMD_LOGINRESULT,
    CMD_GETUSER,
    CMD_INSERT,
    CMD_GETVERSION,
    CMD_GETFRIENDS,
    CMD_GETFRIENDSRESULT,
    CMD_MESSAGE,
    CMD_MESSAGENUMBER,
};

struct DataHeader{
    short cmd;
    short data_length;
};

struct _Message:public DataHeader{
    _Message(){
        this->cmd = CMD_MESSAGE;
        this->data_length = sizeof(_Message);
    }
    char sid[256];
    char date[256];
    char message[256];
};

struct GetUser:public DataHeader{
    GetUser(){
        this->cmd = CMD_GETUSER;
        this->data_length = sizeof(GetUser);
    }
    char user_id[256];
};

struct GetUserResult:public DataHeader{
    GetUserResult(){
        this->cmd = CMD_GETUSER;
        this->data_length = sizeof(GetUserResult);
    }
    char user_id[256];
    char password[256];
    int coins;
    char phoneNumber[256];
    char mailNumber[256];
    char name[256];
    int favorability;
    char role[256];
    int f;
};

struct LOGIN:public DataHeader{
    LOGIN(){
        this->cmd = CMD_LOGIN;
        this->data_length = sizeof(LOGIN);
    }
    char user_id[256];
    char password[256];
};
struct LoginResult:public DataHeader{
    LoginResult(){
        this->cmd = CMD_LOGINRESULT;
        this->data_length = sizeof(LoginResult);
    }
    int result;
};
struct Version:public DataHeader{
    Version(){
        this->cmd = CMD_GETVERSION;
        this->data_length = sizeof(Version);
    }
    char version[8];
};

struct GetFriends:public DataHeader{
    GetFriends(){
        this->cmd = CMD_GETFRIENDS;
        this->data_length = sizeof(GetFriends);
    }
    char user_id[256];
};
struct GetFriendsResult:public DataHeader{
    GetFriendsResult(){
        this->cmd = CMD_GETFRIENDSRESULT;
        this->data_length = sizeof(GetFriendsResult);
    }
    char friends[256];
    int f;
};

struct Cmd:public DataHeader{
    Cmd(){
        this->cmd = CMD_INSERT;
        this->data_length = sizeof(Cmd);
    }
    char _cmd[256];
};

struct _MessageNumber:public DataHeader{
    _MessageNumber(){
        this->cmd = CMD_MESSAGENUMBER;
        this->data_length = sizeof(_MessageNumber);
    }
    int number;
};


class TcpClient
{
public:
    //构造
    TcpClient();
    //析构
    virtual ~TcpClient();
    //初始化socket 返回1为正常
    int InitSocket();
    //连接服务器  返回1为成功
    int Connect(const char *ip,unsigned short port);
    //关闭socket
    void CloseSocket();
    //查询是否有待处理消息
    bool OnRun();
    //判断是否工作中
    bool IsRun();
    //发送数据
    int SendData(DataHeader *_head);
    //接收数据
    int RecvData();
    //总数据
    DataHeader * res;
    //群聊消息队列
    queue<_Message> qm;
    int number = 1;
    //int needUpdate = 0;
private:
    SOCKET _sock;

};

#endif // TCPCLIENT_H
