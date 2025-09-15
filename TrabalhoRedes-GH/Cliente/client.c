#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1" // Endereço IP do servidor
#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    // Verifica se o número correto de argumentos foi fornecido na linha de comando
    if (argc != 3) {
        printf("Uso: %s <protocolo> <nome_cliente>\n", argv[0]);
        return 1;
    }
    const char* protocolo = argv[1]; // Protocolo especificado (TCP ou UDP)
    const char* nome_cliente = argv[2]; // Nome do cliente

    WSADATA wsaData;
    // Inicialização da biblioteca Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Erro na inicialização do WSA");
        return 1;
    }

    SOCKET clientSocket;

    // Verifica o protocolo especificado (TCP ou UDP)
    if (strcmp(protocolo, "TCP") == 0) {
        // Criação do socket TCP
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    } else if (strcmp(protocolo, "UDP") == 0) {
        // Criação do socket UDP
        clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    } else {
        printf("Protocolo nao suportado: %s\n", protocolo);
        WSACleanup();
        return 1;
    }

    if (clientSocket == INVALID_SOCKET) {
        perror("Erro na criacao do socket do cliente");
        WSACleanup();
        return 1;
    }

    if (strcmp(protocolo, "TCP") == 0) {
        // Configuração do endereço do servidor para TCP
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);
        serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

        // Conexão ao servidor TCP
        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            perror("Erro na conexao ao servidor");
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    }

    char message[MAX_BUFFER_SIZE];
    sprintf(message, "Ola Servidor! Eu sou %s!", nome_cliente);

    if (strcmp(protocolo, "UDP") == 0) {
        // Configuração do endereço do servidor para UDP
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);
        serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

        // Envio de mensagem para o servidor UDP
        if (sendto(clientSocket, message, strlen(message), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            perror("Erro ao enviar mensagem ao servidor");
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    } else {
        // Envio de mensagem para o servidor TCP
        if (send(clientSocket, message, strlen(message), 0) == SOCKET_ERROR) {
            perror("Erro ao enviar mensagem ao servidor");
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    }

    printf("Mensagem enviada ao Servidor: %s\n", message);

    char response[MAX_BUFFER_SIZE];
    memset(response, 0, sizeof(response));

    if (strcmp(protocolo, "UDP") == 0) {
        // Recebe resposta do servidor UDP
        struct sockaddr_in serverResponseAddr;
        int serverResponseAddrLen = sizeof(serverResponseAddr);

        if (recvfrom(clientSocket, response, sizeof(response), 0, (struct sockaddr*)&serverResponseAddr, &serverResponseAddrLen) == SOCKET_ERROR) {
            perror("Erro ao receber resposta do servidor");
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    } else {
        // Recebe resposta do servidor TCP
        if (recv(clientSocket, response, sizeof(response), 0) == SOCKET_ERROR) {
            perror("Erro ao receber resposta do servidor");
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    }

    printf("Resposta recebida do Servidor: %s\n", response);

    // Fecha o socket do cliente e limpa recursos
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
