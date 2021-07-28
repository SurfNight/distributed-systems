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
#define PORT 8080

using namespace std;

char *isPrime(int number)
{
    for (int i = 2; i <= sqrt(number); i++)
    {
        if (number % i == 0)
            return " não é primo.";
    }
    return " é primo.";
}

int main(int argc, char *argv[])
{
    char buffer[50];
    int server_fd, new_socket, valread;
    struct sockaddr_in addr;
    int opt = 1;
    int addrlen = sizeof(addr);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        cout << "error creating socket" << endl;
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    // bind socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&addr,
             sizeof(addr)) < 0)
    {
        cout << "bind failed" << endl;
        return 1;
    }
    if (listen(server_fd, 1) < 0)
    {
        cout << "listen failed" << endl;
        return 1;
    }
    cout << "Server is listening on port " << PORT << endl;
    new_socket = accept(server_fd, (struct sockaddr *)&addr, (socklen_t *)&addrlen);
    if (new_socket < 0)
    {
        cout << "accept failed" << endl;
        return 1;
    }

    while (true)
    {
        valread = read(new_socket, buffer, sizeof(char) * 50);
        int number = atoi(buffer);
        if (number == 0)
            break;
        cout << "Received " << number << "..." << endl;
        char text[50];
        sprintf(text, "O número %i%s", number, isPrime(number));
        send(new_socket, text, sizeof(char) * 50, 0);
    }
    close(server_fd);
    return 0;
}
