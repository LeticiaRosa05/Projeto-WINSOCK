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
    int tamanhoServidor, tamanhoCliente, bytesRecebidos;

    printf("Inicializando o Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Erro ao iniciar Winsock. Código: %d\n", WSAGetLastError());
        return 1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        printf("Erro ao criar socket: %d\n", WSAGetLastError());
        return 1;
    }

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(8888);
    servidor.sin_addr.s_addr = inet_addr("127.0.0.1"); // Aberto em localhost

    printf("Este sendo o processo cliente, ele inicia a comunicação. Ambas as partes podem encerrá-la.\n");
    printf("Digite 'sair' para encerrar o chat.\n");

    while (mensagem != "sair") {
        printf("Digite a mensagem para enviar: ");
        fgets(mensagem, sizeof(mensagem), stdin);

        if (sendto(sock, mensagem, strlen(mensagem), 0, (struct sockaddr*)&servidor, sizeof(servidor)) == SOCKET_ERROR) {
            printf("Erro ao enviar: %d\n", WSAGetLastError());
            return 1;
        }

        tamanhoCliente = sizeof(cliente);
        bytesRecebidos = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliente, &tamanhoCliente);

        if (bytesRecebidos == SOCKET_ERROR) {
            printf("Erro ao receber dados: %d\n", WSAGetLastError);
            return 1;
        }

        buffer[bytesRecebidos] = '\0';

        printf("Mensagem recebida: %s\n", buffer);

        if (strcmp(buffer, "sair\n") == 0 || strcmp(buffer, "sair") == 0) {
            printf("Comunicação encerrada pelo servidor.\n");
            break;
        }
        
    }
    

    printf("\nAperte enter para encerrar a aplicação.");
    getchar(); // Espera um enter para fechar a aplicação. Pra usar tempo, Sleep(1000);

    closesocket(sock);
    WSACleanup();
    return 0;
}
