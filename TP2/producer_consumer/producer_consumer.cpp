#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include "time.h"
#include <semaphore.h>
#include <math.h>
#include "../../lib/MyLib.h"

using namespace std;

int memory_size;
clock_t before, after;
int *memory;
int consumed;
int Np, Nc;

// declarando threads
thread **consumer_threads;
thread **producer_threads;

// declarando semáforos
sem_t mutex;
sem_t empty_sem;
sem_t full;

// função que gera um numero entre 1 e 10000000
int generateRandomNumber()
{
    return (rand() % 10000000) + 1;
}

// função que gera um numero aleatorio e coloca na primeira posição zerada da memoria compartilhada
void addResource(int number)
{
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

// função que realiza o consumo do primeiro recurso encontrado na memoria compartilhada
void consume()
{
    int number;
    // consumes the first number != 0 from the shared memory
    for (int i = 0; i < memory_size; i++)
    {
        number = memory[i];
        // find a number to consume (number != 0)
        if (number != 0)
        {
            // explicitamos para o usuario se o numero consumido é ou nao primo
            cout << "CONSUMER: O número " << number << (isPrime(number) ? "" : " não") << " é primo. " << endl;
            // consume number and set its memory location to 0
            memory[i] = 0;
            // add 1 to consumed count
            consumed++;
            break;
        }
    }
}

// função executada pelas threads produtoras
void producer()
{
    while (true)
    {
        // generate a resource
        int number = generateRandomNumber();
        // esperamos o semaforo de empty
        sem_wait(&empty_sem);
        // esperamos o mutex
        sem_wait(&mutex);
        // add resource to shared memory
        addResource(number);
        // liberamos o mutex
        sem_post(&mutex);
        // incrementamos o semaforo de full
        sem_post(&full);
    }
}

// função executada pelas threads consumidoras
void consumer()
{
    // aqui, ficamos em loop infinito
    while (true)
    {
        // esperamos o semaforo full
        sem_wait(&full);
        // esperamos o mutex
        sem_wait(&mutex);
        // realizamos o consumo
        consume();
        // se o contador de consumo chegar a 100000, devemos finalizar o programa
        if (consumed == 100000)
        {
            // computamos o after para termos o tempo de execução do programa
            after = clock();
            double time_spent = ((double)(after - before)) / CLOCKS_PER_SEC;
            // calculamos o tempo de execução do programa e explicitamos isso para o usuário
            cout << memory_size << "|" << Np << "|" << Nc << "|" << time_spent << endl;
            exit(0);
        }
        // liberamos o mutex
        sem_post(&mutex);
        // incrementamos o semaforo de empty
        sem_post(&empty_sem);
    }
}

// função que cria as threads consumidoras dado o número de threads consumidoras
void createProducerThreads(int num_threads)
{
    producer_threads = new thread *[num_threads];
    // create num_threads threads that will produce numbers
    for (int i = 0; i < num_threads; i++)
    {
        // criamos uma thread produtora apontando para a função produtor
        producer_threads[i] = new thread(&producer);
    }
}

// função que cria as threads consumidoras dado o número de threads consumidoras
void createConsumerThreads(int num_threads)
{
    consumer_threads = new thread *[num_threads];
    // create num_threads threads that will produce numbers
    for (int i = 0; i < num_threads; i++)
    {
        // criamos uma thread consumidora apontando para a função consumidor
        consumer_threads[i] = new thread(&consumer); //creates a thread given a function pointer
    }
}

// função que realiza o join das threads consumidoras
void joinConsumerThreads(int Nc)
{
    for (int i = 0; i < Nc; i++)
    {
        consumer_threads[i]->join();
    }
}

// função que realiza o join das threads produtoras
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
    // "Insira o tamanho da memória, o número de produtores e o número de consumidores"
    cin >> memory_size >> Np >> Nc;
    memory = new int[memory_size]{0}; //creates a zeroed array of size "memory_size"
    sem_init(&mutex, 1, 1); //initializes the mutex
    sem_init(&full, 1, memory_size); //sets the semaphore "full" to its maximum size
    sem_init(&empty_sem, 1, 0); //sets the semaphore "empty" to its minimum size
    // marcamos o tempo de início da execução do programa (excluindo esses passo iniciais)
    before = clock();
    // creating threads
    createConsumerThreads(Nc);
    createProducerThreads(Np);
    // joining threads
    joinConsumerThreads(Nc);
    joinProducerThreads(Np);
    return 0;
}
