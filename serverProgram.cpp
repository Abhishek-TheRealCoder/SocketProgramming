#include <iostream>
#include <winsock2.h>
#include "clientHandler.h"

using namespace std;

#pragma comment(lib, "ws2_32.lib")
#define PORT 7000

struct sockaddr_in server;
// internally this structure looks like this:
// struct sockaddr_in
// {
//     short sin_family;
//     unsigned short sin_port;
//     struct in_addr sin_addr;
//     char sin_zero[8];
// };

fd_set fr, fw, fe; // file descriptor sets for select() system call

int main() {
   
    // Initalize WSA variable
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

                                                             // This line asks the Operating System: ->
    int nsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Please create a communication endpoint for network communication . The OS then creates a socket and returns an integer ID called a socket descriptor.
                                        // socket(...)  -> This is a system call provided by the OS.
                                        // It creates a socket inside kernel memory.
                                        // AF_INET -> Address family for IPv4 -> This means we want to use IPv4 addresses for our socket. Without this, OS won’t know what addressing system to use.
                                        // SOCK_STREAM -> This means we want to use TCP (Transmission Control Protocol) for our socket. TCP is a connection-oriented protocol that provides reliable data transmission.
                                        // IPPROTO_TCP -> This specifies the protocol to be used with the socket. In this case, we are explicitly specifying TCP as the protocol. This is optional when using SOCK_STREAM, as it defaults to TCP, but it can be included for clarity.   
                                        // The OS creates a socket and returns an integer ID called a socket descriptor. This descriptor is used in subsequent system calls to refer to this socket. If the socket creation fails, it returns -1 (or INVALID_SOCKET in Windows). The socket descriptor is used for operations like binding, listening, accepting connections, sending and receiving data, etc.
                                        // You use this ID later for:
                                        // bind()
                                        // listen()
                                        // accept()
                                        // recv()
                                        // send()
                                        // close()
    if(nsocket < 0) {   
        cerr << "Error creating socket" << endl;
        return 1;
    }
    else
    {
        cerr << "Socket created successfully with socketDescriptor id " << nsocket << endl;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);       // host to network byte order i.e convert the port number to big-endian format 
    server.sin_addr.s_addr = INADDR_ANY; // Bind this server to all network interfaces available on this laptop i.e bind to all interfaces (Listen for connections on every network interface available on this machine)
    cerr << "Binding socket at ip address " << inet_ntoa(server.sin_addr) << " and port " << PORT << endl;

    memset(&(server.sin_zero), 0, 8); // zero the rest of the struct to avoid any garbage values that might cause issues in some implementations. This is a common practice to ensure that the unused bytes do not contain random data that could potentially lead to unexpected behavior.

    int nRet = bind(nsocket, (sockaddr*)&server, sizeof(server)); // This line tells the OS: “Attach this socket to this IP address and port number.”
                                                                // Before bind():
                                                                // Your socket exists, but it has NO address.
                                                                // Like:
                                                                // Phone created
                                                                // but no SIM number assigned yet
                                                                // After bind():
                                                                // Your socket is now associated with a specific IP address and port number.
                                                                // Like:
                                                                // Socket gets attached to:
                                                                // IP + PORT
                                                                // Example:
                                                                // 192.168.1.7 : 7000
    // (sockaddr*)&server
    // (This contains the address information: IP address , Port ,Address family)
    // Your server structure contains:
    // server.sin_family = AF_INET;
    // server.sin_port = htons(7000);
    // server.sin_addr.s_addr = INADDR_ANY;
    //Why cast to (sockaddr*) ? => Because bind() expects generic socket address type: sockaddr*
    // But server is: sockaddr_in
    // So we convert pointer type:
    // (sockaddr*)&server
    // Meaning:
    // “Treat this IPv4 structure as generic socket.”
    // therefore bind in simple “Reserve this IP and port for my server socket.”

    if(nRet < 0) {
        cerr << "Error binding socket" << endl;
        return 1;
    }
    else
    {
        cerr << "Socket binded successfully to the port" << endl;
    }

    // Listen to incoming connections on this socket. The second argument (5) is the backlog, which specifies the maximum number of pending connections that can be queued up before the server starts rejecting new connection attempts. In this case, it allows up to 5 pending connections in the queue. If more than 5 clients try to connect at the same time, additional connection attempts may be rejected until there is space in the queue.
    nRet = listen(nsocket, 5);

    if(nRet < 0) {
        cerr << "Error listening for connections" << endl;
        return 1;
    }
    else
    {
        cerr << "Server is now listening for incoming connections..." << endl;
    }

    int n = 100;
    for(int index = 0; index < 99; index++)
    {
        FD_SET(n, &fr);
        n++;
    }

    cerr << endl << fr.fd_count << endl; 

    for(int index = 0; index < 64  
        ; index++)
    {
        cerr << fr.fd_array[index] << " ";
    }
    
    // keep waiting for new requests and proceed as per the request 

    return 0;
}