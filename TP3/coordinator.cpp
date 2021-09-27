#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <queue>
#include <tuple>
#include <mutex>
#include <map>
#include "../lib/MyLib.h"

using namespace std;

void terminal();
int listener();

class remoteMutex
{
private:
    queue<tuple<int, int>> mutexQ;
    mutex mtx;
    bool acquired = false;
    map<int, int> granted_map;
    void grantNext()
    {
        acquired = true;
        tuple<int, int> next = mutexQ.front();
        mutexQ.pop();
        char *msg = encode(GRANT, get<0>(next));
        send(get<1>(next), msg, sizeof(char) * F, 0);
        if (granted_map.find(get<0>(next)) == granted_map.end())
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
    void request(int p_id, int p_fd)
    {
        mtx.lock();
        mutexQ.push(make_tuple(p_id, p_fd));
        if (granted_map.find(p_id) == granted_map.end())
        {
            granted_map[p_id] = 0;
        }
        if (!acquired)
        {
            grantNext();
        }
        mtx.unlock();
    }
    void release()
    {
        mtx.lock();
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
            std::cout << it->first << " " << it->second << "\n";
        }
        mtx.unlock();
    }
};

remoteMutex rmutex;

int main()
{
    pid_t terminal_pid = fork();

    if (terminal_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (terminal_pid == 0)
    {
        listener();
    }
    terminal();
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
            exit(1);
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
    char buffer[F];
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
        pid_t connection_pid = fork();
        if (connection_pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (connection_pid == 0)
        {
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
                    rmutex.release();
                }
                else
                {
                    cout << "Invalid message\n";
                }
            }
        }
    }
    return 0;
}
