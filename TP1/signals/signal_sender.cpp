#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <errno.h>

using namespace std;

int main(int argc, char* argv[]){
    int pid, signal;
    cout << "Enter pid and signal" << endl;
    cin >> pid >> signal;
    if(kill(pid,signal) == -1){
        if(errno==ESRCH){
            cout << "PID inexistente" << endl;
        }
        else {
            cout << "Ocorreu um erro" << endl;
        }
        return 1;
    }
    return 0;
}