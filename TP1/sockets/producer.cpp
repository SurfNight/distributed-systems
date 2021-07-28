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

int main(int argc, char *argv[])
{
    int n;
    char host[16];
    cout << "Insira o IPV4 do servidor:" << endl;
    cin >> host;
    cout << "Insira o número de números a serem enviados ao servidor:" << endl;
    cin >> n;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    int start_n = 1;
    char buffer[50];
    for (; n > 0; n--)
    {
        int delta = rand() % 100 + 1;
        start_n = start_n + delta;
        sprintf(buffer, "%d", start_n);
        cout << "Sent " << start_n << "..." << endl;
        send(sock, buffer, sizeof(char) * 50, 0);
        valread = read(sock, buffer, 50);
        cout << "[CONSUMER]: " << buffer << endl;
    }
    sprintf(buffer, "%d", 0);
    send(sock, buffer, sizeof(char) * 50, 0);
    return 0;
}