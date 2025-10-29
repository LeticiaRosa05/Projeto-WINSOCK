#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

// Para compilar, executar o comando gcc client_udp.c -o cliente_udp.exe -lws2_32

void remover_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in servidor, cliente;
    char buffer[1024];
    char mensagem[1024];
    int tamanhoCliente, bytesRecebidos;
    const char *palavra_chave_sair = "sair";

    printf("Inicializando o Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Erro ao iniciar Winsock. Codigo: %d\n", WSAGetLastError());
        return 1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        printf("Erro ao criar socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(8888);
    servidor.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP do servidor p conexão

    printf("Este sendo o processo cliente, ele inicia a comunicacao. Ambas as partes podem encerra-la.\n");
    printf("Digite '%s' para encerrar o chat.\n", palavra_chave_sair);

    while (1) {
        printf("Digite a mensagem para enviar: ");
        if (fgets(mensagem, sizeof(mensagem), stdin) == NULL) {
            continue;
        }
        remover_newline(mensagem);

        if (strcmp(mensagem, palavra_chave_sair) == 0) {
            sendto(sock, mensagem, strlen(mensagem), 0, (struct sockaddr*)&servidor, sizeof(servidor));
            printf("Comunicacao encerrada pelo Cliente.\n");
            break;
        }

        if (sendto(sock, mensagem, strlen(mensagem), 0, (struct sockaddr*)&servidor, sizeof(servidor)) == SOCKET_ERROR) {
            printf("Erro ao enviar: %d\n", WSAGetLastError());
            break;
        }

        tamanhoCliente = sizeof(cliente);
        bytesRecebidos = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&cliente, &tamanhoCliente);

        if (bytesRecebidos == SOCKET_ERROR) {
            printf("Erro ao receber dados: %d\n", WSAGetLastError);
            break;
        }

        buffer[bytesRecebidos] = '\0';

        if (strcmp(buffer, palavra_chave_sair) == 0) {
            printf("\nServidor: %s\n", buffer);
            printf("Comunicacao encerrada pelo Servidor.\n");
            break;
        }

        printf("\nMensagem recebida: %s\n", buffer);
    }
    

    printf("\nAperte enter para encerrar a aplicacao.");
    getchar(); // Espera um enter para fechar a aplicação. Pra usar tempo, Sleep(1000);

    closesocket(sock);
    WSACleanup();
    return 0;
}
