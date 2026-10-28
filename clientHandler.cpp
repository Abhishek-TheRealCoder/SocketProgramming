#include <iostream>
#include <winsock2.h>

using namespace std;

struct sockaddr_in client;

void handleClient(int nsocket)
{
    int clientSize = sizeof(client);

    int clientSocket = accept(nsocket, (sockaddr*)&client, &clientSize);
    
    if(clientSocket < 0)
    {
        cerr << "Client connection failed" << endl;
        return;
    }

    cerr << "Client connected successfully!" << endl;

    cerr << "Client connected successfully!" << endl;

    cerr << "Client IP   : " << inet_ntoa(client.sin_addr) << endl;
    cerr << "Client Port : " << ntohs(client.sin_port) << endl;

    char buffer[1024];

    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    while (true)
    {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0)
            break;

        buffer[bytesReceived] = '\0';

        cout << "Message: " << buffer << endl;
    }
    // if(bytesReceived < 0)
    // {
    //     cerr << "Error receiving data" << endl;
    // }
    // else
    // {
    //     buffer[bytesReceived] = '\0';
    //     cerr << "Message from client: " << buffer << endl;
    // }
}