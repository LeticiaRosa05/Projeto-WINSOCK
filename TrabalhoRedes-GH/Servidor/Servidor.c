#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345 // Porta na qual o servidor irá escutar.
#define MAX_BUFFER_SIZE  // Tamanho máximo do buffer usado para armazenar mensagens.

int main(int argc, char* argv[]) {
    // Verifica se o número correto de argumentos foi fornecido na linha de comando
    if (argc != 3) {
        printf("Uso: %s <protocolo> <nome_servidor>\n", argv[0]);
        return 1;
    }
    const char* protocolo = argv[1]; // Protocolo especificado (TCP ou UDP)
    const char* nome_servidor = argv[2]; // Nome do servidor

    WSADATA wsaData;
    // Inicialização da biblioteca Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Erro na inicialização do WSA");
        return 1;
    }

    SOCKET serverSocket;

    // Verifica o protocolo especificado (TCP ou UDP)
    if (strcmp(protocolo, "TCP") == 0) {
        // Criação do socket TCP
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    } else if (strcmp(protocolo, "UDP") == 0) {
        // Criação do socket UDP
        serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    } else {
        printf("Protocolo não suportado: %s\n", protocolo);
        WSACleanup();
        return 1;
    }

    if (serverSocket == INVALID_SOCKET) {
        perror("Erro na criação do socket do servidor");
        WSACleanup();
        return 1;
    }

    //Essa parte do código é responsável por configurar a estrutura 'serverAddr', que é usada para
    // especificar os detalhes de como o servidor irá se ligar a uma porta específica e em qual endereço IP ele estará escutando.
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Vincula o socket à porta especificada
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("Erro na ligacao do socket");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (strcmp(protocolo, "TCP") == 0) {
        // Se o protocolo for TCP, o servidor aguarda por conexões
        if (listen(serverSocket, 1) == SOCKET_ERROR) {
            perror("Erro ao aguardar por conexoes");
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        printf("Servidor aguardando conexoes...\n");

        // Aceita a conexão do cliente
        SOCKET clientSocket;
        struct sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);

        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            perror("Erro ao aceitar conexao");
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        // Recebe a mensagem do cliente
        char buffer[MAX_BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));

        if (recv(clientSocket, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
            perror("Erro ao receber mensagem do cliente");
            closesocket(clientSocket);
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        printf("Mensagem recebida do Cliente: %s\n", buffer);

        // Responde ao cliente
        char response[MAX_BUFFER_SIZE];
        sprintf(response, "Ola Cliente! Eu sou %s!", nome_servidor);

        if (send(clientSocket, response, strlen(response), 0) == SOCKET_ERROR) {
            perror("Erro ao enviar mensagem ao cliente");
            closesocket(clientSocket);
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        printf("Resposta enviada ao Cliente: %s\n", response);

        closesocket(clientSocket);
    } else if (strcmp(protocolo, "UDP") == 0) {
        // Se o protocolo for UDP, o servidor aguarda por mensagens em um loop
        printf("Servidor aguardando mensagens...\n");

        char buffer[MAX_BUFFER_SIZE];
        struct sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);

        while (1) {
            // Recebe uma mensagem do cliente UDP
            int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
            if (bytesReceived == SOCKET_ERROR) {
                perror("Erro ao receber mensagem do cliente");
                break;
            }

            printf("Mensagem recebida do Cliente: %s\n", buffer);

            // Responde ao cliente UDP
            char response[MAX_BUFFER_SIZE];
            sprintf(response, "Olá Cliente! Eu sou %s!", nome_servidor);

            if (sendto(serverSocket, response, strlen(response), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
                perror("Erro ao enviar mensagem ao cliente");
                break;
            }

            printf("Resposta enviada ao Cliente: %s\n", response);
        }
    }

    // Fecha o socket do servidor e limpa recursos
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
