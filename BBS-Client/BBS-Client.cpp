// BBS-Client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <cstdio>
#include <cstdint>
#include <iostream>
#include <string>

#include "Crypto.h"
#include "SocketW.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

using WinSock::SocketW;

int main()
{
    SocketW client = SocketW();
    client.init();
    client.connect("127.0.0.1", 11451);

    while (true) {
        printf("Your Message: ");
        string data;
        cin >> data;
        uint16_t len = data.length();
        client.send((char*)(&len), 2);
        client.send(data.c_str(), len);
    }
    return 0;
}
