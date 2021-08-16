#include <sys/signalfd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "../../lib/MyLib.h"

#define PORT 8080

using namespace std;

int main()
{
    char buffer[30];
    int server_fd, new_socket, valread;
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    
    // Criando Socket TCP com IPv4
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        cout << "Error criando o socket" << endl;
        return 1;
    }

    // Setamos as opções do socket
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    // bindamos o socket a porta definida anteriormente
    if (bind(server_fd, (struct sockaddr *)&addr,
             sizeof(addr)) < 0)
    {
        cout << "Erro realizando bind" << endl;
        return 1;
    }

    // Inicializando listening do socket
    if (listen(server_fd, 1) < 0)
    {
        cout << "Erro inicializando listen" << endl;
        return 1;
    }
    cout << "Server listening na porta " << PORT << endl;
    new_socket = accept(server_fd, (struct sockaddr *)&addr, (socklen_t *)&addrlen);
    if (new_socket < 0)
    {
        cout << "Erro realizando accept de nova conexão" << endl;
        return 1;
    }

    while (true)
    {
        valread = read(new_socket, buffer, sizeof(char) * 30);
        int number = atoi(buffer);
        if (number == 0)
            break;
        cout << "Received " << number << "..." << endl;
        char response[30];
        sprintf(response, "%i%s", number, (isPrime(number) ? " é primo." : " não é primo."));
        send(new_socket, response, sizeof(char) * 30, 0);
    }
    // Fechando socket
    close(server_fd);
    return 0;
}
