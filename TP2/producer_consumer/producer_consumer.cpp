#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <semaphore.h>
#include <math.h>
#include "../../lib/MyLib.h"

using namespace std;
int memory_size;
int *memory;
int consumed;

sem_t mutex;
sem_t empty_semaphore;
sem_t full;

int generateRandomNumber() {
    // Generate a random number between 1 and 10000000
    return (rand() % 10000000) + 1;
}

void createThread(void (*function) (void)) {
    // function that creates a thread given a function pointer
    thread t(function);
}

int* createZeroedArray(int size)
{
    // function that creates a zeroed array of size "size"
    int *array = new int[size];
    for (int i = 0; i < size; i++)
    {
        array[i] = 0;
    }
    return array;
}

void fullizeSemaphore()
{
    // function that sets the semaphore "full" to its maximum size
    sem_init(&full,1,memory_size);
}

void addResource(int number)
{   
    // function that receiver a number and stores it at the shared memory
    for (int i = 0; i < memory_size; i++) {
        // find a free space of the memory to store the number
        if (memory[i] == 0) {
            memory[i] = number;
            // leave the loop afterwards
            break;
        }
    }
}

void consume()
{
    int number;
    // function that consumes a random number from the shared memory
    for (int i = 0; i < memory_size; i++)
    {
        number = memory[i];
        // find a number to consume (number != 0)
        if (number != 0)
        {
            cout << "CONSUMER: O número " << number << (isPrime(number) ? "" : " não") << " é primo." << endl;
            // consume number and set its memory location to 0
            memory[i] = 0;
            // add 1 to consumed count
            consumed++;
            break;
        }
    }
}

void producer()
{
    // generate a resource
    int number = generateRandomNumber();
    sem_wait(&empty_semaphore);
    sem_wait(&mutex);
    // add resource to shared memory
    addResource(number);
    sem_post(&mutex);
    sem_post(&full);
}

void consumer()
{   
    int number;
    sem_wait(&full);
    sem_wait(&mutex);
    consume();
    if(consumed == 10000) {
        exit(0);
    }
    sem_post(&mutex);
    sem_post(&empty_semaphore);
}

void createProducerThreads(int num_threads) {
    // create num_threads threads that will produce numbers
    for (int i = 0; i < num_threads; i++) {
        createThread(&producer);
    }
}

void createConsumerThreads(int num_threads) {
    // create num_threads threads that will consume numbers
    for (int i = 0; i < num_threads; i++) {
        createThread(&consumer);
    }
}

int main()
{   
    srand(time(NULL));
    int Np, Nc;
    cout << "Insira o tamanho da memória, o número de produtores e o número de consumidores" << endl;
    cin >> memory_size >> Np >> Nc;
    memory = createZeroedArray(memory_size);
    fullizeSemaphore();
    // creating threads
    createConsumerThreads(Nc);
    createProducerThreads(Np);
    while(true) {
        // waiting for 10000 numbers to be consumed
    }
}