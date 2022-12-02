// client
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#define EHCO_PORT 8080
#define SV_ADDR "192.168.141.134"
#define MSG_LEN 100
int main()
{
    // socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        printf("socket:failure\n");
        return 0;
    }
    printf("socket%d:success\n", sock_fd);

    // server_address
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;        // TCP/IPaddress_family
    serv_addr.sin_port = htons(EHCO_PORT); // listening_port
    // serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
    serv_addr.sin_addr.s_addr = inet_addr(SV_ADDR);
    bzero(&(serv_addr.sin_zero), 8);

    // connect
    if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("connect:failure\n");
        close(sock_fd);
        return 0;
    }
    printf("connect:success\n");

    // send
    char sd_msg[MSG_LEN];
    char rcv_msg[MSG_LEN];
    int rcv_len;
    while (1)
    {
        getchar();
        scanf("%[^\n]", sd_msg);
        send(sock_fd, sd_msg, strlen(sd_msg), 0);
        if (strncmp(sd_msg, "_exit_", 6) == 0)
            break;
        rcv_len = recv(sock_fd, rcv_msg, MSG_LEN, 0);
        rcv_msg[rcv_len] = '\0';
        printf("to server: %s\n%d bytes, from server: %s\n\n", sd_msg, rcv_len, rcv_msg);
    }

    close(sock_fd);
    printf("\n_close_\n");
    return 0;
}
