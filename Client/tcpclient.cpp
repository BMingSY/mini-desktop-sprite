#include "tcpclient.h"
#include <QDebug>

TcpClient::TcpClient()
{
    _sock = INVALID_SOCKET;
}
TcpClient::~TcpClient()
{
    // 关闭socket
    CloseSocket();
}
int TcpClient::InitSocket()
{
    // 启动windows socket 2,x环境
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    if (0 != WSAStartup(ver, &dat))
        return -1; // 环境错误
    // 创建socket
    if (INVALID_SOCKET != _sock)
    {
        printf("<Socket=%d>关闭连接\n", _sock);
        CloseSocket(); // 如果之前有连接 就关闭连接
    }
    _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (INVALID_SOCKET == _sock)
        return 0; // socket创建失败
    return 1;
}
int TcpClient::Connect(const char *ip, unsigned short port)
{
    // 如果为无效套接字 则初始化
    if (INVALID_SOCKET == _sock)
        InitSocket();
    // 连接服务器
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;                 // IPV4
    _sin.sin_port = htons(port);               // 端口号
    _sin.sin_addr.S_un.S_addr = inet_addr(ip); // IP
    if (SOCKET_ERROR == connect(_sock, (sockaddr *)&_sin, sizeof(sockaddr_in)))
        return 0; // 连接失败

    return 1; // 连接成功
}
void TcpClient::CloseSocket()
{
    if (INVALID_SOCKET != _sock)
    {
        // 关闭socket
        closesocket(_sock);
        // 清除windows socket 环境
        WSACleanup();
        _sock = INVALID_SOCKET;
    }
}
bool TcpClient::OnRun()
{
    if (IsRun()) // 如果有连接则监听事件
    {
        fd_set _fdRead;          // 建立集合
        FD_ZERO(&_fdRead);       // 清空集合
        FD_SET(_sock, &_fdRead); // 放入集合
        timeval _t = {1, 0};     // select最大响应时间
        // 新建seclect
        int _ret = select(_sock + 1, &_fdRead, NULL, NULL, &_t);
        if (_ret < 0)
        {
            printf("seclect任务结束\n");
            return false;
        }
        if (FD_ISSET(_sock, &_fdRead)) // 获取是否有可读socket
        {
            FD_CLR(_sock, &_fdRead); // 清理计数器
            if (-1 == RecvData())
            {
                CloseSocket();
                return false;
            }
        }
        return true;
    }
    return false;
}
bool TcpClient::IsRun()
{
    return _sock != INVALID_SOCKET;
}
int TcpClient::SendData(DataHeader *_head)
{
    if (IsRun() && _head)
    {
        send(_sock, (const char *)_head, _head->data_length, 0);
        return 1;
    }
    return 0;
}

int TcpClient::RecvData() // 处理数据
{
    // 缓冲区
    char buffer[4096] = {};
    // 接收客户端发送的数据
    int _buf_len = recv(_sock, buffer, sizeof(DataHeader), 0);
    DataHeader *_head = (DataHeader *)buffer;
    if (_buf_len <= 0)
    {
        printf("与服务器断开连接,任务结束\n");
        return -1;
    }

    recv(_sock, buffer + sizeof(DataHeader), _head->data_length - sizeof(DataHeader), 0);

    res = _head;

    if (res->cmd == CMD_MESSAGE)
    {
        _Message *ms = (_Message *)res;
        _Message me;
        strcpy(me.date, ms->date);
        strcpy(me.sid, ms->sid);
        strcpy(me.message, ms->message);
        qm.push(me);
        res = nullptr;
    }
    //    if(res->cmd == CMD_MESSAGENUMBER)
    //    {
    //        _MessageNumber * mn = (_MessageNumber*)res;
    //        number = mn->number;
    //        res = nullptr;
    //        needUpdate = 1;
    //    }

    return 0;
}
