// MyLib.cpp: implementation of the MyLib
// Function that receives an integer and returns if it is prime or not
#include <math.h>
#include "MyLib.h"
#include <string>
#include <string.h>

using namespace std;

bool isPrime(int number)
{
  for (int i = 2; i <= sqrt(number); i++)
  {
    if (number % i == 0)
      return false;
  }
  return true;
}
char *encode(int msg, int id)
{
  char *encoded = new char[F]{0};
  string temp = to_string(msg) + '|' + to_string(id) + '|';
  temp.copy(encoded, temp.length(), 0);
  return encoded;
}
void decode(char *buffer, int *message, int *id)
{
  *message = stoi(strtok(buffer, "|"));
  *id = stoi(strtok(NULL, "|"));
}