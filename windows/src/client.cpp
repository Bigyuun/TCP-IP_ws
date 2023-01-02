// #ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
// #include "WinSock2.h"
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")

#include <math.h>
#include <string.h>
#include <string>
#include <time.h>
#include <chrono>
#include <thread>

#define TCP_BUFFER_SIZE 512

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    SOCKET hSocket;
    SOCKADDR_IN serverAddress;

    short port = 9800;
    char message[] = "";
    std::cout << "Initializing TCP/IP... ";
    // initial WinSock.
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) std::cout << "WSAStartup() error" << std::endl;

    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET) std::cout << "hSocket() error" << std::endl;

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("192.168.7.1");
    // inet_pton(AF_INET, "192.168.7.1", &serverAddress.sin_addr.s_addr);
    serverAddress.sin_port = htons(port);

    std::cout << "Done" << std::endl;
    std::cout << "Connecting to Server...";
    if (connect(hSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) std::cout << "connect() error" << std::endl;
    std::cout << "Done" << std::endl;

    static char recvMsg[TCP_BUFFER_SIZE] = { 0 };
    std::string sendMsg;
    static double joints_vel[7];
    static uint64_t counter = 0;
    static int strlen;
    std::cout << "Enter TCP Loop" << std::endl;
    while (true)
    {
		//strlen = recv(hSocket, recvMsg, TCP_BUFFER_SIZE, 0);
		//if (strlen == -1) std::cout << "read() error" << std::endl;
        //std::string str_recvMsg;
        //str_recvMsg = recvMsg;
		////printf("from server : %s, [%d]\n", str_recvMsg, strlen);
        //std::cout << str_recvMsg << "/" << strlen << std::endl;

        sendMsg = "torquej";
        //sendMsg = "speedj";
        sendMsg += "([";

        joints_vel[0] = 2100 * sin(counter * -0.5);
        joints_vel[1] = 100 * sin(counter * -0.02);
        joints_vel[2] = 3100 * sin(counter * 0.001);
        joints_vel[3] = 100 * sin(counter * -0.001);
        joints_vel[4] = 14200 * sin(counter * 34);
        joints_vel[5] = 100 * sin(counter * -19585e3);
        joints_vel[6] = 100 * sin(counter * 0.04e-5);

        for (int i = 0; i < 7; i++)
        {
            //joints_vel[i] = 100 * sin(counter * 0.001);
            sendMsg += std::to_string(joints_vel[i]);
            sendMsg += "e-2";
            sendMsg += ",";
        }
        sendMsg.pop_back();
        sendMsg += "])"+std::to_string(counter);
        // sendMsg += "])";
        send(hSocket, sendMsg.c_str(), sendMsg.size(), 0);
        //sendMsg = send(hSocket, sendMsg.c_str(), TCP_BUFFER_SIZE, 0);
        printf("client send : %s\n", sendMsg.c_str());
        counter++;
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
    closesocket(hSocket);
    std::cout << "TCP OFF" << std::endl;
    return 0;
}
