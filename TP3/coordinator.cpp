#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <queue>
#include <tuple>
#include <mutex>
#include <thread>
#include <map>
#include "../lib/MyLib.h"

using namespace std;

void terminal();
int listener();

//essa classe é responsável por controlar os acessos concorrentes ao lock
class remoteMutex
{
private:
    mutex mtx;                     //mutex usado para manipular a fila o estado do remoteMutex
    queue<tuple<int, int>> mutexQ; //fila, cada entrada é uma tupla <socket,pid>
    bool acquired = false;         //estado do mutex
    map<int, int> granted_map;     //mapeia quantas vezes cada processo recebeu o lock
    //envia GRANT para o primeiro proceso da fila
    void grantNext()
    {
        acquired = true;
        tuple<int, int> next = mutexQ.front();
        mutexQ.pop();
        char *msg = encode(GRANT, get<0>(next));
        send(get<1>(next), msg, sizeof(char) * F, 0);
        cout << to_string(get<0>(next)) + " GRANT\n";
        if (granted_map.find(get<0>(next)) == granted_map.end()) //se o processo ainda não está no mapa
        {
            granted_map[get<0>(next)] = 1;
        }
        else
        {
            granted_map[get<0>(next)] += 1;
        }
    }

public:
    remoteMutex(){};
    //chamado quando um processo pede o lock
    void request(int p_id, int p_fd)
    {
        mtx.lock();
        cout << to_string(p_id) + " REQUEST\n";
        mutexQ.push(make_tuple(p_id, p_fd));
        if (granted_map.find(p_id) == granted_map.end()) //se o processo ainda não está no mapa
        {
            granted_map[p_id] = 0;
        }
        if (!acquired)
        {
            grantNext(); //se a fila estiver vazia o processo recebe GRANT imediatamente
        }
        mtx.unlock();
    }
    void release(int p_id)
    {
        mtx.lock();
        cout << to_string(p_id) + " RELEASE\n";
        if (mutexQ.empty())
        {
            acquired = false;
        }
        else
        {
            grantNext();
        }
        mtx.unlock();
    }
    void printQueue()
    {
        mtx.lock();
        queue<tuple<int, int>> copy = mutexQ;
        while (!copy.empty())
        {
            cout << get<0>(copy.front()) << " ";
            copy.pop();
        }
        std::cout << std::endl;
        mtx.unlock();
    }
    void printGrantCount()
    {
        mtx.lock();
        for (auto it = granted_map.cbegin(); it != granted_map.cend(); ++it)
        {
            std::cout << it->first << " | " << it->second << "\n";
        }
        mtx.unlock();
    }
};

remoteMutex rmutex;

int main()
{
    thread(listener).detach(); //criamos uma thread para funçao listener()
    terminal();                //a main thread executa funçao terminal
    return 1;
}

void terminal()
{
    while (true)
    {
        int command;
        cin >> command;
        if (command == 1)
        {
            rmutex.printQueue();
        }
        else if (command == 2)
        {
            rmutex.printGrantCount();
        }
        else if (command == 3)
        {
            return;
        }
        else
        {
            cout << "invalid command\n";
        }
    }
}

int listener()
{
    int server_fd;
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
    if (listen(server_fd, 1) < 0)
    {
        cout << "Erro inicializando listen" << endl;
        return 1;
    }
    cout << "Server listening na porta " << PORT << endl;
    while (true)
    {
        int new_socket = accept(server_fd, (struct sockaddr *)&addr, (socklen_t *)&addrlen);
        if (new_socket < 0)
        {
            cout << "Erro realizando accept de nova conexão" << endl;
            return 1;
        }
        //funçao lambda que é executada para tratar cada socket novo
        auto fconnection = [&, new_socket]()
        {
            char buffer[F];
            while (true)
            {
                int valread = read(new_socket, buffer, sizeof(char) * F);
                if (valread <= 0)
                    break;
                int msg, process;
                decode(buffer, &msg, &process);

                if (msg == REQUEST)
                {
                    rmutex.request(process, new_socket);
                }
                else if (msg == RELEASE)
                {
                    rmutex.release(process);
                }
                else
                {
                    cout << "Invalid message\n";
                }
            }
        };
        thread(fconnection).detach();
    }
    return 0;
}
