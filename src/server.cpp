#include "server.h"
#include "json11.h"
#include <stdio.h>

#ifdef WIN32
  #include <winsock2.h>
//  #include <Ws2tcpip.h>
#else
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
  #include <unistd.h> /* Needed for close() */
#endif

tcp_server::tcp_server( int port){
  printf("init server");
    tcp_server::port = port;
}

tcp_server::~tcp_server(){
    socket_quit();
}

int tcp_server::socket_close(int sock){
  int status = 0;
  #ifdef _WIN32
    status = shutdown(sock, SD_BOTH);
    if (status == 0) { status = closesocket(sock); }
  #else
    status = shutdown(sock, SHUT_RDWR);
    if (status == 0) { status = close(sock); }
  #endif
  return status;
}

int tcp_server::socket_init(void){
  #ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(1,1), &wsa_data);
  #else
    return 0;
  #endif
}

int tcp_server::socket_quit(void){
  #ifdef _WIN32
    return WSACleanup();
  #else
    return 0;
  #endif
}

int tcp_server::start_listening(){
    if (socket_init() != 0){
        printf("Server: The Winsock dll not found!n\r\n");
        socket_quit();
        return 0;
    }

    sSock = socket(AF_INET, SOCK_STREAM, 0);
    if(sSock == -1){
        printf("Server: Error initializing socket!n\r\n");
        socket_quit();
        return 0;
    }

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(port);
    service.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sSock, (sockaddr*)&service, sizeof(service)) == -1){
        printf("Server: Error binding socket to portn\r\n");
        socket_quit();
        return 0;
    }

    if(listen(sSock, 10) == -1){
        printf("listen(): Error listening on socket \r\n");
        socket_quit();
        return 0;
    }
    return accept_connection();
}

int tcp_server::accept_connection()
{
    sockaddr_in from;
    int fromlen = sizeof(from);
    printf("waiting for connection...\r\n");
    cSock = accept(sSock, (sockaddr*)&from, &fromlen);
    printf("accepted connection\r\n");
    socket_close(sSock);
    return cSock;
}

int tcp_server::receive_json(json11::Json* packet){
  std::string data = "";
  while(1)
  {
     char buf[1024];
     int numBytesRead = recv(cSock, buf, sizeof(buf), 0);
     if (numBytesRead > 0)
     {
        for (int i=0; i<numBytesRead; i++)
        {
           char c = buf[i];
           if (c == '\n')
           {
              if (data.length() > 0)
              {
                 std::string err_comment;
                 packet->parse(data, err_comment);
                 return 0;
              }
           }
           else
              data += c;
        }
     }
     else
     {
        printf("Socket closed or socket error!\n");
        return -1;
     }
  }

}

int tcp_server::send_json(json11::Json* packet){
  std::string data = packet->dump();
  send(cSock, data.c_str(), data.length(), 0);
  return 0;
}
