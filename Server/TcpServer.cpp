//TcpServer.cpp
#include "TcpServer.h"

TcpServer::TcpServer()
{
    //链接数据库
    mysql_init(&mysql);

    if (mysql_real_connect(&mysql, "47.97.105.213", "kws", "111111", "desktop_sprite", 3306, nullptr, CLIENT_FOUND_ROWS))
        std::cout << "链接成功" << std::endl;
    else
        std::cout << "连接失败" << std::endl;

    _sock = INVALID_SOCKET;
}
TcpServer::~TcpServer()
{
    CloseSocket();
}
int TcpServer::InitSocket()
{
    //创建socket
    if (INVALID_SOCKET != _sock)
    {
        printf("<Socket=%d>关闭连接\n", _sock);
        CloseSocket(); //如果之前有连接 就关闭连接
    }
    _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == _sock)
        return 0; // 0为socket创建错误
    return 1;
}
int TcpServer::Bind(const char *ip, unsigned short port)
{
    //如果为无效套接字 则初始化
    if (INVALID_SOCKET == _sock)
        InitSocket();

    //绑定网络端口和IP地址
    sockaddr_in _myaddr = {};
    _myaddr.sin_family = AF_INET;                // IPV4
    _myaddr.sin_port = htons(port);              //端口
    if (ip)                                      // ip为空则监听所有网卡
        _myaddr.sin_addr.s_addr = inet_addr(ip); // IP
    else
        _myaddr.sin_addr.s_addr = INADDR_ANY; // IP

    if (SOCKET_ERROR ==
        bind(_sock, (sockaddr *)&_myaddr, sizeof(sockaddr_in))) // socket (强制转换)sockaddr结构体 结构体大小
    {
        printf("绑定失败\n");
        return 0;
    }
    else
    {
        printf("绑定成功\n绑定端口为%d\n", port);
        return 1;
    }
}
int TcpServer::Listen(int n)
{
    //如果为无效套接字 则提示
    if (INVALID_SOCKET == _sock)
    {
        printf("请先初始化套接字并绑定IP端口\n");
        return 0;
    }
    //监听网络端口
    if (SOCKET_ERROR == listen(_sock, n)) //最大连接队列
    {
        printf("监听失败\n");
        return 0;
    }
    else
    {
        printf("监听成功\n");
        return 1;
    }
}
int TcpServer::Accept()
{
    //等待接收客户端连接
    sockaddr_in _clientAddr = {};         //新建sockadd结构体接收客户端数据
    int _addr_len = sizeof(sockaddr_in);  //获取sockadd结构体长度
    SOCKET _temp_socket = INVALID_SOCKET; //声明客户端套接字

    _temp_socket =
        accept(_sock, (sockaddr *)&_clientAddr, (socklen_t *)&_addr_len); //自身套接字 客户端结构体 结构体大小
    if (INVALID_SOCKET == _temp_socket)                                   //接收失败
    {
        printf("<Socket=%d>错误,接受到无效客户端SOCKET\n", _temp_socket);
        return 0;
    }

    printf("新客户端加入\nIP地址为:%s \n", inet_ntoa(_clientAddr.sin_addr));

    //群发通知在这
    _MessageNumber * mn = new _MessageNumber;
    mn->number = _clients.size();
    for(int i = 0 ;i < _clients.size() ; i++)
        send(_clients[i],(const char*)mn,mn->data_length,0);	

    //将新的客户端加入动态数组
    _clients.push_back(_temp_socket);
    return 1;
}
void TcpServer::CloseSocket()
{
    if (INVALID_SOCKET != _sock)
    {
        //关闭客户端socket
        for (int n = 0; n < _clients.size(); ++n)
            close(_clients[n]);
        //关闭socket/LINUX
        close(_sock);
        _sock = INVALID_SOCKET;
    }
    mysql_close(&mysql);
}
bool TcpServer::OnRun()
{
    if (IsRun())
    {
        //std::cout << "正在监听" << std::endl;

        fd_set _fdRead; //建立集合
        fd_set _fdWrite;
        fd_set _fdExcept;
        FD_ZERO(&_fdRead); //清空集合
        FD_ZERO(&_fdWrite);
        FD_ZERO(&_fdExcept);
        FD_SET(_sock, &_fdRead); //放入集合
        FD_SET(_sock, &_fdWrite);
        FD_SET(_sock, &_fdExcept);
        timeval _t = {2, 0};     // select最大响应时间
        SOCKET _maxSock = _sock; //最大socket
        //把连接的客户端 放入read集合
        for (int n = _clients.size() - 1; n >= 0; --n)
        {
            FD_SET(_clients[n], &_fdRead);
            if (_maxSock < _clients[n])
                _maxSock = _clients[n];
        }
        // select函数筛选select
        int _ret = select(_maxSock + 1, &_fdRead, &_fdWrite, &_fdExcept, &_t);
        if (_ret < 0)
        {
            printf("select任务结束\n");
            CloseSocket();
            return false;
        }
        if (FD_ISSET(_sock, &_fdRead)) //获取是否有新socket连接
        {
            FD_CLR(_sock, &_fdRead); //清理
            Accept();                //连接
        }
        //遍历所有socket 查看是否有待处理事件
        for (int n = 0; n < _clients.size(); ++n)
        {
            if (FD_ISSET(_clients[n], &_fdRead))
            {
                if (-1 == RecvData(_clients[n])) //处理请求 客户端退出的话
                {
                    std::vector<SOCKET>::iterator iter = _clients.begin() + n; //找到退出客户端的地址
                    if (iter != _clients.end())                                //如果是合理值
                        _clients.erase(iter);                                  //移除
                
                    _MessageNumber * mn = new _MessageNumber;
                    mn->number = _clients.size();
                    for(int i = 0 ;i < _clients.size() ; i++)
                    send(_clients[n],(const char*)mn,mn->data_length,0);	
                
                }
            }
        }
        return true;
    }
    return false;
}
bool TcpServer::IsRun()
{
    return _sock != INVALID_SOCKET;
}
int TcpServer::SendData(DataHeader *_head, SOCKET _temp_socket)
{
    if (IsRun() && _head)
    {
        send(_temp_socket, (const char *)_head, _head->data_length, 0);
        return 1;
    }
    return 0;
}
int TcpServer::RecvData(SOCKET _temp_socket) //处理数据
{
    //缓冲区
    char buffer[4096] = {};
    //接收客户端发送的数据
    int _buf_len = recv(_temp_socket, buffer, sizeof(DataHeader), 0);
    DataHeader *_head = (DataHeader *)buffer;
    if (_buf_len <= 0)
    {
        printf("客户端已退出\n");
        return -1;
    }
    recv(_temp_socket,buffer+sizeof(DataHeader),_head->data_length - sizeof(DataHeader),0);
    
    cal(_head, _temp_socket);
    return 0;
}

void TcpServer::cal(DataHeader *_head, SOCKET _temp_socket)
{
    //在此处理相应数据
    switch(_head->cmd){
        case CMD_GETUSER :
        {
            GetUser* gu = (GetUser*)_head;
            char query[512];
            sprintf(query,"select * from user where user_id='%s'",gu->user_id);
            if(mysql_query(&mysql,query)){
                std::cout << "mysql query user error" << std::endl;
                break;
            }
            MYSQL_RES * res = mysql_store_result(&mysql);
            int n = mysql_num_rows(res),m = mysql_num_fields(res);
            GetUserResult *gur = new GetUserResult() ;
            for(int i = 0 ; i < n ; i ++ ){
                MYSQL_ROW row = mysql_fetch_row(res);
                memcpy(gur->user_id,row[0],sizeof(row[0]));
                memcpy(gur->password,row[1],sizeof(row[1]));
                gur->coins = strtol(row[4],NULL,10);
                memcpy(gur->phoneNumber,row[5],sizeof(row[5]));
                memcpy(gur->mailNumber,row[6],sizeof(row[6]));
                memcpy(gur->name,row[7],sizeof(row[7]));
                gur->favorability = strtol(row[8],NULL,10);
                memcpy(gur->role,row[9],sizeof(row[9]));
            }
            if(n == 0) gur->f = 0;
            else gur->f = 1; 
            //返回数据结果
            SendData(gur , _temp_socket);
            break;
        }

        case CMD_INSERT :
        {
            Cmd* cmd = (Cmd*)_head;
            if(mysql_query(&mysql,cmd->_cmd)){
                std::cout << "mysql insert error" << std::endl;
            }
            break;
        }

        case CMD_LOGIN :
        {
            Login* login = (Login*)_head;
            char query[512];
            sprintf(query,"select * from user where user_id='%s'",login->user_id);
            if(mysql_query(&mysql,query)){
                std::cout << "mysql query user error" << std::endl;
                break;
            }
            MYSQL_RES * res = mysql_store_result(&mysql);
            int n = mysql_num_rows(res),m = mysql_num_fields(res);
            LoginResult *loginResult = new LoginResult() ;
            for(int i = 0 ; i < n ; i++){
                MYSQL_ROW row = mysql_fetch_row(res);
                // std::cout << row[1] <<" "<< login->password<<std::endl;
                if(strcmp(row[1],login->password) == 0){
                    // 比对成功
                    loginResult->result = 1;
                }else loginResult->result = 0;
            }
            if(n == 0) loginResult->result = 2;
            else if(n >= 2) {
                std::cout << "error:mysql user id query return more than 1 row" << std::endl;
            }
            //返回数据结果
            SendData(loginResult , _temp_socket);
            break;
        }

        case CMD_GETVERSION :
        {
            Version* version = (Version*)_head;
            char query[32];
            
            sprintf(query,"select * from version");
            if(mysql_query(&mysql,query)){
                std::cout << "mysql query version error" << std::endl;
                break;
            }

            MYSQL_RES * res = mysql_store_result(&mysql);
            int n = mysql_num_rows(res),m = mysql_num_fields(res);
            for(int i = 0 ; i < n ; i++){
                MYSQL_ROW row = mysql_fetch_row(res);
                memcpy(version->version,row[0],sizeof(row[0])); 
            }
            if(n != 1) {
                std::cout << "error:mysql version query return not equal 1 row" << std::endl;
            }
            //返回数据结果
            SendData(version , _temp_socket);
            break;
        } 
        
        case CMD_GETFRIENDS :
        {
            GetFriends* gf = (GetFriends*)_head;
            char query[512];
            sprintf(query,"select * from friends where id1='%s'",gf->user_id);
            if(mysql_query(&mysql,query)){
                std::cout << "mysql query friends error" << std::endl;
                break;
            }
            MYSQL_RES * res = mysql_store_result(&mysql);
            int n = mysql_num_rows(res),m = mysql_num_fields(res);
            
            for(int i = 0 ; i < n ; i ++ ){
                GetFriendsResult *gfr = new GetFriendsResult() ;
                MYSQL_ROW row = mysql_fetch_row(res);
                memcpy(gfr->friends,row[1],sizeof(row[1]));
                gfr -> f = 1;
                SendData(gfr , _temp_socket);
            }
            GetFriendsResult *gff = new GetFriendsResult() ;
            if(n == 0) gff->f = 0;
       
            //返回数据结果
            SendData(gff , _temp_socket);
            break;
        }

        case CMD_MESSAGE :
        {
            break;
        }

    }
}

