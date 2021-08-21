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
        }
    }
    void release()
    {
        lock.clear();
    }
};

char *memory;
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
    int parcial_sum = 0;
    for (int i = 0; i < size; i++)
    {
        parcial_sum += (int)parcela[i];
    }
    my_spinlock.acquire();
    accumulator += parcial_sum;
    my_spinlock.release();
}

void fill_random(char *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        array[i] = (char)generateRandomNumber();
    }
}

void createSummerThreads(int num_threads, char *memory, int memory_size)
{
    int parcela = memory_size / num_threads;
    summer_threads = new thread *[num_threads];
    // create num_threads threads
    for (int i = 0; i < num_threads - 1; i++)
    {
        summer_threads[i] = new thread(summer, &memory[i * parcela], parcela); //creates a thread given a function pointer
    }
    int parcela_com_resto = parcela + memory_size % num_threads;
    summer_threads[num_threads - 1] = new thread(summer, &memory[parcela * (num_threads - 1)], parcela_com_resto);
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
    // "Insira o número de threads e o tamanho da memória"
    cin >> K >> N;
    memory = new char[N]; //creates a zeroed array of size "N"
    fill_random(memory, N);
    clock_t before, after;
    before = clock();
    createSummerThreads(K, memory, N);
    joinSummerThreads(K);
    after = clock();
    double time_spent = ((double)(after - before)) / CLOCKS_PER_SEC;
    cout << K << "|" << N << "|" << time_spent << endl;
    return 0;
}