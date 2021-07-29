#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

using namespace std;

int main()
{
    int n, valread;
    char host[16];
    cout << "Insira o IPv4 do servidor:" << endl;
    cin >> host;
    cout << "Insira quantos números serão enviados ao servidor:" << endl;
    cin >> n;
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "Erro criando o socket." << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0)
    {
        cout << "Erro no endereço IPv4." << endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Erro ao conectar no socket do servidor.");
        return -1;
    }

    // N0 = 1
    int start_n = 1;
    char buffer[30];
    for (; n > 0; n--)
    {
        // A cada iteração, seguimos o algoritmo N = N-1 + Delta,
        // sendo Delta um valor aleatório entre 1 e 100.
        start_n = start_n + rand() % 100 + 1;
        // Armazenamos esse valor num buffer como char.
        sprintf(buffer, "%d", start_n);
        cout << "Sending " << start_n << "..." << endl;
        // Enviamos o buffer para o socket.
        send(sock, buffer, sizeof(char) * 30, 0);
        valread = read(sock, buffer, 30);
        cout << "[CONSUMER]: " << buffer << endl;
    }
    // Enviamos um 0 para o socket para indicar que os processos terminarão
    sprintf(buffer, "%d", 0);
    send(sock, buffer, sizeof(char) * 30, 0);
    return 0;
}