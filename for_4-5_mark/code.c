#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <pthread.h>

#define KEY 1234
#define SHM_SIZE 10

int shmid;
int *shared_memory;
int sem_id;

// Структура для семафора
struct sembuf acquire = {0, -1, SEM_UNDO};
struct sembuf release = {0, 1, SEM_UNDO};

// Функция для создания семафора
int create_semaphore() {
    int sem_id = semget(KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    return sem_id;
}

// Функция для инициализации разделяемой памяти
void initialize_shared_memory() {
    shmid = shmget(KEY, SHM_SIZE * sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Инициализация значений в разделяемой памяти
    for (int i = 0; i < SHM_SIZE; ++i) {
        shared_memory[i] = 0; 
    }
}

// Функция для проверки кривизны булавки
void *check_curvature(void *arg) {
    int worker_id = *((int *) arg);
    while (1) {
        sleep(rand() % 3); // случайное время проверки
        semop(sem_id, &acquire, 1);
        for (int i = 0; i < SHM_SIZE; ++i) {
            if (shared_memory[i] == 0) {
                printf("Worker %d checked pin %d\n", worker_id, i);
                shared_memory[i] = 1; // булавка обработана
                semop(sem_id, &release, 1);
                break;
            }
        }
        semop(sem_id, &release, 1);
    }
    return NULL;
}

int main() {
    // Инициализация семафора и разделяемой памяти
    sem_id = create_semaphore();
    initialize_shared_memory();

    // Создание рабочих для проверки кривизны булавок
    pthread_t workers[3];
    int worker_ids[3] = {1, 2, 3};
    for (int i = 0; i < 3; ++i) {
        pthread_create(&workers[i], NULL, check_curvature, &worker_ids[i]);
    }

    // Ожидание завершения рабочих
    for (int i = 0; i < 3; ++i) {
        pthread_join(workers[i], NULL);
    }

    // Освобождение ресурсов
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    
    return 0;
}
