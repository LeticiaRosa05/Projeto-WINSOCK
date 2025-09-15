#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define MAX_BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;

    // Criar um socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("Erro ao criar o socket UDP");
        exit(EXIT_FAILURE);
    }

    // Configurar o endereço do servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Vincular o socket ao endereço do servidor
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Erro ao vincular o socket");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER_SIZE];
    socklen_t client_len = sizeof(client_addr);

    // Esperar por mensagens do cliente
    ssize_t recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
    if (recv_len == -1)
    {
        perror("Erro na recepção");
        exit(EXIT_FAILURE);
    }

    buffer[recv_len] = '\0';
    printf("Cliente diz: %s\n", buffer);

    // Responder ao cliente
    char response[] = "Olá Cliente! Eu sou o servidor.";
    sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&client_addr, client_len);

    close(sockfd);

    return 0;
}
