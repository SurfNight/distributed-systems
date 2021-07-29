#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

void sig_handler(int signo)
{
    cout << "Received signal " << signo << endl;
    if (signo == SIGABRT)
    {
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    int type;
    cout << "Digite 1 para busy wait e 0 para blocking wait:" << endl;
    cin >> type;
    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);
    signal(SIGABRT, sig_handler);
    cout << "PID: " << getpid() << endl;
    if (type == 1)
    {
        while (true);
    }
    else if (type == 0)
    {
        while (true)
            pause();
    }
    else
    {
        cout << "Opção invalida" << endl;
        return 1;
    }
    return 0;
}
