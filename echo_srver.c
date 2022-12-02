// server
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
#define MAX_CLIENT_NUM 10
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
    serv_addr.sin_addr.s_addr = inet_addr(SV_ADDR); // server_address
    bzero(&(serv_addr.sin_zero), 8);

    // bind
    if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0)
    {
        printf("bind:failure\n");
        close(sock_fd);
        return 0;
    }
    printf("bind:success\n");
    // loop
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int clientfd;
    while (1)
    {
        // listen
        if (listen(sock_fd, MAX_CLIENT_NUM) != 0)
        {
            printf("listen:failure\n");
            close(sock_fd);
            return 0;
        }
        printf("listen:success\n");
        // accept
        clientfd = accept(sock_fd, (struct sockaddr *)&client_addr, &len);
        if (clientfd <= 0)
        {
            printf("accept:failure\n");
            close(sock_fd);
            return 0;
        }
        printf("accept:success\n");
        if (fork() == 0)
        {
            // son is born
            char rcv_msg[MSG_LEN];
            int rcv_len;
            while ((rcv_len = recv(clientfd, rcv_msg, MSG_LEN, 0)) > 0)
            {
                rcv_msg[rcv_len] = '\0';
                // if (strncmp(rcv_msg, "_exit_", 6) == 0)
                //     break;
                printf("process%d, \n%d bytes, from client: %s\n\n", getpid(), rcv_len, rcv_msg);
                fflush(stdout);
                send(clientfd, rcv_msg, rcv_len, 0);
            }
            close(clientfd);
            close(sock_fd);
            return 0;
            // son is dead
        }
        // father_loop
        close(clientfd);
    }
    printf("\n_close_\n");
    return 0;
}