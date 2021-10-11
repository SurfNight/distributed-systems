#include <string>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <netinet/in.h>
#include <ctime>
#include "../lib/MyLib.h"
#include <stdlib.h>
#include <chrono>


using namespace std;

class remoteMutex
{
private:
    int sock;
public:
    remoteMutex(char *coord_ip)
    {
        int valread;
        struct sockaddr_in serv_addr;

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            cout << "Erro criando o socket." << endl;
            return;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        if (inet_pton(AF_INET, coord_ip, &serv_addr.sin_addr) <= 0)
        {
            cout << "Erro no endereço IPv4." << endl;
            return;
        }

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("Erro ao conectar no socket do servidor.");
            return;
        }
    }
    int acquire()
    {
        char *buffer = encode(REQUEST, getpid());
        send(sock, buffer, sizeof(char) * 30, 0);
        int valread = read(sock, buffer, sizeof(char) * 30);
        delete buffer;
        if (valread == 0) {
            return 0;
        }

        return 1;
    }

    int release()
    {
        char *buffer = encode(RELEASE, getpid());
        send(sock, buffer, sizeof(char) * 30, 0);
        delete buffer;
        return 1;
    }
    ~remoteMutex(){};
};

int main(int argc, char *argv[])
{
    int k = atoi(argv[1]);
    int r = atoi(argv[2]);
    remoteMutex rm(argv[3]);
    ofstream myfile;
    for (int i = 0; i < r; i++)
    {
        if (!rm.acquire())
            return -1;
        myfile.open("resultado.txt", ios::app);
        auto current_time = chrono::high_resolution_clock::now();
        myfile << chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count()  << "|" << getpid() << "\n";
        myfile.close();
        rm.release();
        sleep(k);
    }
}