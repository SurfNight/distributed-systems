#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>

using namespace std;

void consumer();
void producer();

int my_pipe[2];

bool isPrime(int number)
{
  for (int i = 2; i <= sqrt(number); i++)
  {
    if (number % i == 0)
      return false;
  }
  return true;
}

int main(int argc, char *argv[])
{
  if (pipe(my_pipe))
  {
    cout << "Ocorreu erro" << endl;
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
    cout << "Erro no fork" << endl;
  }
}

void producer()
{
  int n;
  cout << "Insira o número de números a serem enviados no pipe:" << endl;
  cin >> n;

  int start_n = 1;
  char buffer[20];
  for (; n > 0; n--)
  {
    int delta = rand() % 100 + 1;
    start_n = start_n + delta;
    sprintf(buffer, "%d", start_n);
    write(my_pipe[1], &buffer, sizeof(char) * 20);
  }
  sprintf(buffer, "%d", 0);
  write(my_pipe[1], &buffer, sizeof(char) * 20);
}

void consumer()
{
  char buffer[20];
  while (true)
  {
    read(my_pipe[0], &buffer, sizeof(char) * 20);
    int number = atoi(buffer);
    if (number == 0)
      break;
    cout << "O Número " << number << " é primo? " << (isPrime(number) ? "Sim" : "Não") << endl;
  }
}