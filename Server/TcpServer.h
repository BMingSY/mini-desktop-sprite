#ifndef _TcpServer_hpp_
#define _TcpServer_hpp_

#include <arpa/inet.h> //selcet
#include <mysql.h>
#include <unistd.h> //uni std

#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (-1)

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

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

struct Login:public DataHeader{
    Login(){
        this->cmd = CMD_LOGIN;
        this->data_length = sizeof(Login);
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



class TcpServer
{
  public:
    //??????
    TcpServer();
    //??????
    virtual ~TcpServer();
    //?????????socket ??????1?????????
    int InitSocket();
    //??????IP/??????
    int Bind(const char *ip, unsigned short port);
    //????????????
    int Listen(int n);
    //????????????
    int Accept();
    //??????socket
    void CloseSocket();
    //??????????????????????????????
    bool OnRun();
    //?????????????????????
    bool IsRun();
    //????????????
    //int SendData(char *_head, SOCKET _temp_socket);
    int SendData(DataHeader *_head, SOCKET _temp_socket);

    //????????????
    int RecvData(SOCKET _temp_socket);

    //????????????
    void cal(DataHeader *_head, SOCKET _temp_socket);

  private:
    SOCKET _sock;
    std::vector<SOCKET> _clients; //???????????????socket

    MYSQL mysql;
};

#endif

