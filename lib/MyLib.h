#pragma once

#define PORT 8080
#define F 30

enum
{
    REQUEST = 1,
    GRANT,
    RELEASE
};

bool isPrime(int number);
char *encode(int, int);
void decode(char *, int *, int *);
