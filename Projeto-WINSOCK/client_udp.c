#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

// Para compilar, executar o comando gcc client_udp.c -o cliente_udp.exe -lws2_32

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in servidor;
    char mensagem[1024];
    int tamanhoServidor;

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

    printf("Digite a mensagem para enviar: ");
    fgets(mensagem, sizeof(mensagem), stdin);

    if (sendto(sock, mensagem, strlen(mensagem), 0, (struct sockaddr*)&servidor, sizeof(servidor)) == SOCKET_ERROR) {
        printf("Erro ao enviar: %d\n", WSAGetLastError());
        return 1;
    }

    printf("Mensagem enviada com sucesso!\n");
    getchar(); // Espera um enter para fechar a aplicação. Pra usar tempo, Sleep(1000);

    closesocket(sock);
    WSACleanup();
    return 0;
}
