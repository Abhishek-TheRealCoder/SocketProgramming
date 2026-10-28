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
    // “Treat this IPv4 structure as generic socket address.”
    // therefore bind in simple “Reserve this IP and port for my server socket.”

    if(nRet < 0) {
        cerr << "Error binding socket" << endl;
        return 1;
    }
    else
    {
        cerr << "Socket binded successfully to the port" << endl;
    }


    nRet = listen(nsocket, 5);

    if(nRet < 0)
    {
        cerr << "Listen failed" << endl;
        return 1;
    }

    cerr << "Server is listening on port " << PORT << endl;
    
    while (true)
    {
        handleClient(nsocket);
    }
    // handleClient(nsocket);
    return 0;
}

// NOTE->
// Suppose your laptop has this IP on Wireless LAN adapter Wi-Fi::
// 192.168.1.7

// Now you write:
// server.sin_addr.s_addr = INADDR_ANY;
// This tells the OS:
// “I do not care WHICH IP of my laptop gets the connection. Accept all.”
// So OS internally uses:
// 0.0.0.0

// 0.0.0.0 is NOT your laptop IP.

// It is only a special symbol meaning:
// ALL IPs OF THIS MACHINE
// your server listens on ALL network interfaces of your laptop.
// That includes IPs from:
// Wi-Fi adapter
// Ethernet adapter
// VMware VMnet8
// VMware VMnet1
// VPN adapters

// localhost (127.0.0.1)
// Think of your house analogy:

// Your house has:

// Front door
// Back door
// Side door

// Now you tell security guard:

// Allow entry from ANY door

// Internally he notes:

// ALL DOORS

// That “ALL DOORS” is like:

// 0.0.0.0

// But people still enter through real doors.

// Similarly:

// People connect through:

// 192.168.1.7:7000

// But server internally says:

// I am listening on ALL addresses → 0.0.0.0

// So:

// What you SEE	Meaning
// 0.0.0.0	listen on every IP
// 192.168.1.7	actual laptop IP used by clients

// Your phone connects to:

// 192.168.1.7:7000

// NOT to:

// 0.0.0.0:7000

// =====================================================

// So if your laptop has:

// WiFi       -> 192.168.1.45
// VMnet8     -> 192.168.220.1
// VMnet1     -> 192.168.56.1
// localhost  -> 127.0.0.1

// then your server is listening on: 

// 192.168.1.45:7000
// 192.168.220.1:7000
// 192.168.56.1:7000
// 127.0.0.1:7000

// ALL at the same time.

// That is why OS shows:

// 0.0.0.0

// ====================================================

// Why does sin_zero exist?

// For memory alignment and compatibility with old socket structures.
// You normally do NOT use it for anything.
// It is just unused extra bytes.
// Set all 8 bytes to zero to avoid any garbage values that might cause issues in some implementations.