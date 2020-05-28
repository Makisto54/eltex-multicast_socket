#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 

int main(int argc, char *argv[]) 
{
    int sock;
    socklen_t size;
    char buf[1024];
    struct ip_mreq mr;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    size = sizeof(struct sockaddr_in);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        fprintf(stderr, "Incorrect server socket\n");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(0xAABB);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(sock, (struct sockaddr *)& server_address, size) == -1)
    {
        fprintf(stderr, "Incorrect server bind\n");
        exit(1);
    }
    
    mr.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
    mr.imr_interface.s_addr = INADDR_ANY;

    if(setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1)
    {
        fprintf(stderr, "Incorrect server setsockopt\n");
        exit(1);
    }

    if(recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)& client_address, &size) == -1)
    {
        fprintf(stderr, "Incorrect server recv\n");
        exit(1);
    }

    shutdown(sock, SHUT_RDWR);
    puts(buf);

    return 0;
}
