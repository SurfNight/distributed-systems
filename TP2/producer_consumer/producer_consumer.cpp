#include <unistd.h>
#include <stdlib.h>
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
bool consumed_all = false;

thread **consumer_threads;
thread **producer_threads;

sem_t mutex;
sem_t empty;
sem_t full;

int generateRandomNumber()
{
    // Generate a random number between 1 and 10000000
    return (rand() % 10000000) + 1;
}

void addResource(int number)
{
    // function that receives a number and stores it at the shared memory
    for (int i = 0; i < memory_size; i++)
    {
        // find a free space of the memory to store the number
        if (memory[i] == 0)
        {
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
            cout << "CONSUMER: O número " << number << (isPrime(number) ? "" : " não") << " é primo. " << consumed + 1 << endl;
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
    while (!consumed_all)
    {
        // generate a resource
        int number = generateRandomNumber();
        sem_wait(&empty);
        sem_wait(&mutex);
        // add resource to shared memory
        addResource(number);
        sem_post(&mutex);
        sem_post(&full);
    }
}

void consumer()
{
    while (!consumed_all)
    {
        int number;
        sem_wait(&full);
        sem_wait(&mutex);
        consume();
        if (consumed == 10000)
        {
            consumed_all = true;
        }
        sem_post(&mutex);
        sem_post(&empty);
    }
}

void createProducerThreads(int num_threads)
{
    producer_threads = new thread *[num_threads];
    // create num_threads threads that will produce numbers
    for (int i = 0; i < num_threads; i++)
    {
        producer_threads[i] = new thread(&producer); //creates a thread given a function pointer
    }
}

void createConsumerThreads(int num_threads)
{
    consumer_threads = new thread *[num_threads];
    // create num_threads threads that will produce numbers
    for (int i = 0; i < num_threads; i++)
    {
        consumer_threads[i] = new thread(&consumer); //creates a thread given a function pointer
    }
}

void joinConsumerThreads(int Nc)
{
    for (int i = 0; i < Nc; i++)
    {
        consumer_threads[i]->join();
    }
}
void joinProducerThreads(int Np)
{
    for (int i = 0; i < Np; i++)
    {
        producer_threads[i]->join();
    }
}

int main()
{
    srand(time(NULL));
    int Np, Nc;
    cout << "Insira o tamanho da memória, o número de produtores e o número de consumidores" << endl;
    cin >> memory_size >> Np >> Nc;
    memory = new int[memory_size]{0}; //creates a zeroed array of size "memory_size"
    sem_init(&mutex, 1, 1);
    sem_init(&full, 1, memory_size); //sets the semaphore "full" to its maximum size
    sem_init(&empty, 1, 0);
    // creating threads
    createConsumerThreads(Nc);
    createProducerThreads(Np);
    joinConsumerThreads(Nc);
    joinProducerThreads(Np);
}
