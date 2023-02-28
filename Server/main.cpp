//服务端的 main函数
#include "TcpServer.h"
#include <iostream>
using namespace std;


int main()
{
    //建立tcp对象
    TcpServer *tcp1 = new TcpServer();
    //建立一个socket
    tcp1->InitSocket();
    //绑定端口和IP
    tcp1->Bind(nullptr, 8888);
    //监听
    tcp1->Listen(5);
    //循环
    while (tcp1->IsRun())
        tcp1->OnRun();
    //关闭
    tcp1->CloseSocket();

    return 0;
}
