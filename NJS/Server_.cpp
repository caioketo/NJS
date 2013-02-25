#include "Server.h"

namespace Connections
{
    void Server::Startup()
    {
        WSADATA wsa;
        WSAStartup(0x0202, &wsa);
        this->WorkerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        this->EndPnt.sin_addr.s_addr = ADDR_ANY;
        this->EndPnt.sin_family = AF_INET;
        this->EndPnt.sin_port = htons(this->Port);
        this->Enabled = true;
        this->ID = 0;
        bind(this->WorkerSocket, (SOCKADDR*)&this->EndPnt, sizeof(this->EndPnt));
        printf("[AsynServer]Bound..\n");
        listen(this->WorkerSocket, this->Backlog);
                CreateThread(NULL, NULL, &MainThread, this, NULL, NULL);
    }
    void Server::WaitForConnections(Server * Ser)
    {
        WSAEVENT Handler = WSA_INVALID_EVENT;
        while(Ser->Enabled)
        {
            Handler = WSACreateEvent();
            WSAEventSelect(Ser->WorkerSocket, Handler, FD_ACCEPT);
            WaitForSingleObject(Handler, INFINITE);
            SOCKET accptsock = accept(Ser->WorkerSocket, NULL, NULL);
            Client * NewClient = new Client(accptsock, 255, Ser->ID++);
            NewClient->Connected = true;
            printf("[AsynServer]Client connected.\n");
            ClientServer * OurStruct = new ClientServer();
            OurStruct->Server = Ser;
            OurStruct->Client = NewClient;
            CreateThread(NULL, NULL, &DataThread, OurStruct, NULL, NULL);
        }
    }
    void Server::WaitForData(Client * RClient)
    {
        WSAEVENT Tem = WSA_INVALID_EVENT;
        Tem = WSACreateEvent();
        WSAEventSelect(RClient->WorkerSocket, Tem, FD_READ);
        while(RClient->Connected)
        {
            WaitForSingleObject(Tem, INFINITE);
            RClient->RecvSize = recv(RClient->WorkerSocket, RClient->Buffer, 255, NULL);
            if(RClient->RecvSize > 0)
            {
                RClient->Buffer[RClient->RecvSize] = '\0';
                __raise this->ClientRecieved(RClient, RClient->Buffer);
                Sleep(50);
            }
        }
        return;
    }
    DWORD WINAPI MainThread(LPVOID lParam)
    {
        ((Server*)lParam)->WaitForConnections((Server*)lParam);
        return 0;
    }
    DWORD WINAPI DataThread(LPVOID lParam)
    {
        ClientServer * Sta = ((ClientServer*)lParam);
        Sta->Server->WaitForData(Sta->Client);
        return 0;
    }
}