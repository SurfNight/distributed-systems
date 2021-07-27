#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

bool keep_running = true;

void sig_handler(int signo)
{
    cout << "Received signal " << signo << endl;
    if (signo == SIGABRT)
    {
        keep_running = false;
    }
}

int main(int argc, char *argv[])
{
    int type;
    cout << "Enter 1 for busy wait and anything else for blocking wait:" << endl;
    cin >> type;
    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);
    signal(SIGABRT, sig_handler);
    cout << "My pid is: " << getpid() << endl;
    if (type == 1)
    {
        while (keep_running)
            ;
    }
    else
    {
        while (keep_running)
            pause();
    }
    return 0;
}
