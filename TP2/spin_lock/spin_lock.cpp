#include <atomic>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include "time.h"

using namespace std;

class SpinLock
{
private:
    atomic_flag lock = ATOMIC_FLAG_INIT;

public:
    void acquire()
    {
        while (lock.test_and_set())
        {
            /*test_and_set seta lock para true e retorna seu valor anterior, 
            se lock for true quando acquire for chamado
            esse loop irá rodar até que o valor de lock seja false 
            (ou seja, até ocorrer release)*/
        }
    }
    void release()
    {
        lock.clear();
    }
};

char *vetor;
int accumulator = 0;
thread **summer_threads;
SpinLock my_spinlock;

int generateRandomNumber()
{
    // Generate a random number between 100 and -100
    return (rand() % 200) - 100;
}

// função somadora - summer
void summer(char *parcela, int size)
{
    /* parcela indica o primeiro elemento do vetor da parcela 
    que deve ser somado por summer, size indica o número de elementos a serem somados*/
    int parcial_sum = 0;
    for (int i = 0; i < size; i++)
    {
        parcial_sum += (int)parcela[i];
    }
    my_spinlock.acquire();
    accumulator += parcial_sum; //somamos a soma parcial ao acumulador global
    my_spinlock.release();
}

void fill_random(char *array, int size)
{
    //preenche o array com números aleátorios
    for (int i = 0; i < size; i++)
    {
        array[i] = (char)generateRandomNumber();
    }
}

void createSummerThreads(int num_threads, char *vetor, int vetor_size)
{
    int parcela = vetor_size / num_threads; //tamanhos de cada parcela
    summer_threads = new thread *[num_threads];
    // create num_threads threads
    for (int i = 0; i < num_threads - 1; i++) //i vai de 0 a num_threads - 2
    {
        summer_threads[i] = new thread(summer, &vetor[i * parcela], parcela); //creates a thread given a function pointer
    }
    //a ultima thread receberá uma parcela com o resto somado ao tamanho
    int parcela_com_resto = parcela + vetor_size % num_threads;
    summer_threads[num_threads - 1] = new thread(summer, &vetor[parcela * (num_threads - 1)], parcela_com_resto);
}

void joinSummerThreads(int num_threads)
{
    for (int i = 0; i < num_threads; i++)
    {
        summer_threads[i]->join();
    }
}

int main()
{
    srand(time(NULL));
    int K, N;
    // "Insira o número de threads e o tamanho do vetor"
    cin >> K >> N;
    vetor = new char[N]; //creates a zeroed array of size "N"
    fill_random(vetor, N);
    clock_t before, after;
    before = clock();
    createSummerThreads(K, vetor, N);
    joinSummerThreads(K);
    after = clock();
    double time_spent = ((double)(after - before)) / CLOCKS_PER_SEC;
    cout << K << "|" << N << "|" << time_spent << endl;
    return 0;
}