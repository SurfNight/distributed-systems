#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <queue>
#include <tuple>

#define PORT 8080
#define MAXLINE 1024
#define F 30

using namespace std;

void terminal();
char *encode(char *, int);

int server_fd;

class mutualExclusion
{
private:
    queue<tuple<int, struct sockaddr_in>> mutexQ;
    bool acquired = false;
    int grantNext()
    {
        acquired = true;
        tuple<int, struct sockaddr_in> next = mutexQ.front();
        mutexQ.pop();
        char *msg = encode("GRANTED", get<0>(next));
        int addrlen = sizeof(get<1>(next));
        sendto(server_fd, (const char *)msg, sizeof(char) * F,
               MSG_CONFIRM, (const struct sockaddr *)&get<1>(next),
               addrlen);
    }

public:
    mutualExclusion();
    int request(int p_id, struct sockaddr_in p_ip)
    {
        mutexQ.push(make_tuple(p_id, p_ip));
        if (!acquired)
        {
            grantNext();
        }
    }
    int release()
    {
        if (mutexQ.empty())
        {
            acquired = false;
        }
        else
        {
            grantNext();
        }
    }
};

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
            printQueue();
        }
        else if (command == 2)
        {
            attendedCount();
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
    mutualExclusion mmutex;
    char buffer[F];
    struct sockaddr_in addr, cliaddr;
    int addrlen = sizeof(addr);

    // Criando Socket UDP com IPv4
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
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

    int n;

    int addrlen = sizeof(cliaddr); //len is value/resuslt
    while (true)
    {
        n = recvfrom(server_fd, (char *)buffer, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *)&cliaddr,
                     (socklen_t *)&addrlen);

        decode(buffer, &msg, &process);

        if (msg == "REQUEST")
        {
            mmutex.request(process, cliaddr);
        }
        else if (msg == "RELEASE")
        {
            mmutex.release();
        }
        else
        {
            cout << "Invalid message\n";
        }
    }
    return 0;
}