
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Winsock2.h>
#include "ECTCPSocket.h"

using namespace std;

int main()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        return -1; 
    }

    if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
    {
        WSACleanup();
        return -1;
    }

    ECTCPSocket mysocket;
 
    ECSocketAddress address;
    address.nPort = 5566;
    address.strIP = "127.0.0.1";
    mysocket.BindAddress(&address);

    mysocket.Listen();

    while (true)
    {
        ECSocketAddress conAddress;
        ECTCPSocket conSocket = mysocket.Accept(&conAddress);

        char sendBuf[100] = { 0 };
        sprintf(sendBuf, "Welcome %s to the server program~ \nNow, let's start talking...\n", conAddress.strIP.ToCStr());
        conSocket.Send(sendBuf, strlen(sendBuf) + 1); 

        char * sockConnName = "Client";
        printf("We only can talk five times.\n");
        int n = 5;
        while (n--)
        {
            printf("New left %d times to talk£º\n", n + 1);
            char recvBuf[100] = { 0 };
            conSocket.Recv(recvBuf, 100);
            printf("Con Recv Data:%s\n", recvBuf);

            char talk[100] = {0};
            printf("Please enter what you want to say next(\"quit\"to exit):");
            gets(talk);
            conSocket.Send(talk, strlen(talk) + 1); 
            printf("\n");
        }
        printf("\nEnd talking... \n");
        conSocket.Close();
    }

    printf("\n");
    system("pause");
    return 0;
}