// MyLib.cpp: implementation of the MyLib
// Function that receives an integer and returns if it is prime or not
#include <math.h>
#include "MyLib.h"

bool isPrime(int number)
{
  for (int i = 2; i <= sqrt(number); i++)
  {
    if (number % i == 0)
      return false;
  }
  return true;
}
