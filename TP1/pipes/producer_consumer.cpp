#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "../lib/MyLib.h"

using namespace std;

void consumer();
void producer();

int my_pipe[2];

int main()
{
  if (pipe(my_pipe))
  {
    cout << "Ocorreu um erro inicializando o pipe" << endl;
    return 1;
  }
  int pid = fork();
  if (pid == 0)
  {
    consumer();
  }
  else if (pid > 0)
  {
    producer();
  }
  else
  {
    cout << "Erro ao realizar fork" << endl;
    return 1;
  }
  return 0;
}

void producer()
{
  int n;
  cout << "Insira quantos números serão enviados no pipe:" << endl;
  cin >> n;

  // N0 = 1
  int start_n = 1;
  char buffer[30];
  for (; n > 0; n--)
  {
    // A cada iteração, seguimos o algoritmo N = N-1 + Delta,
    // sendo Delta um valor aleatório entre 1 e 100.
    start_n = start_n + rand() % 100 + 1;
    // Armazenamos esse valor num buffer como char.
    sprintf(buffer, "%d", start_n);
    // Escrevemos o buffer no pipe.
    write(my_pipe[1], &buffer, sizeof(char) * 30);
  }
  // Escrevemos um 0 no pipe para indicar que os processos terminarão 
  sprintf(buffer, "%d", 0);
  write(my_pipe[1], &buffer, sizeof(char) * 30);
}

void consumer()
{
  char buffer[30];
  while (true)
  {
    read(my_pipe[0], &buffer, sizeof(char) * 30);
    int number = atoi(buffer);
    if (number == 0)
      break;
    cout << number << (isPrime(number) ? "" : " não") << " é primo." << endl;
  }
}