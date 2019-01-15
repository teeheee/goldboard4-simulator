#ifndef _SERVER_H_
#define _SERVER_H_


#include <string.h>
#ifdef WIN32
  #include <winsock2.h>
//  #include <Ws2tcpip.h>
#else
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
  #include <unistd.h> /* Needed for close() */
#endif

#include "json11.h"

class tcp_server
{

public:
    tcp_server(int);
    virtual ~tcp_server();
    int start_listening();
    int receive_json(Json* packet);
    int send_json(Json* packet);

protected:
    int port;
    int sSock; // Server Socket
    int cSock; // Server Socket

private:
    int socket_init(void);
    int socket_quit(void);
    int socket_close(int sock);
    int accept_connection();


};

#endif
