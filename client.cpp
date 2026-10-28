#include <iostream>
#include <winsock2.h>
#include <cstring>

using namespace std;

// int main() {

//     WSADATA wsa;
//     WSAStartup(MAKEWORD(2,2), &wsa);

//     SOCKET sock;

//     sock = socket(AF_INET, SOCK_STREAM, 0);

//     sockaddr_in server;

//     server.sin_family = AF_INET;
//     server.sin_port = htons(8080);
//     server.sin_addr.s_addr = inet_addr("127.0.0.1");

//     connect(sock, (sockaddr*)&server, sizeof(server));

//     char msg[] = "Hello from client";

//     send(sock, msg, strlen(msg), 0);

//     cout << "Message sent" << endl;

//     closesocket(sock);

//     WSACleanup();

//     return 0;
// }


// #include <iostream>
// #include <winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PORT 7000

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Laptop IP
    server.sin_addr.s_addr = inet_addr("192.168.1.45");

    int ret = connect(clientSocket, (sockaddr*)&server, sizeof(server));

    if(ret < 0)
    {
        cerr << "Connection failed" << endl;
        return 1;
    }

    cerr << "Connected to server" << endl;

    char msg[] = "Hello from client";

    send(clientSocket, msg, strlen(msg), 0);

    return 0;
}