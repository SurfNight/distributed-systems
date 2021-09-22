#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

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
}