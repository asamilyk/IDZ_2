#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>

#define SHARED_MEMORY_SIZE 1024
#define SEMAPHORE_KEY 1234

typedef struct {
    int is_curved;
    int is_sharpened;
    int is_checked;
} Pin;

int main() {
    // Создание семафора
    int sem_id = semget(SEMAPHORE_KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Инициализация семафора
    semctl(sem_id, 0, SETVAL, 1);

    // Создание разделяемой памяти
    int shm_id = shmget(IPC_PRIVATE, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Прикрепление разделяемой памяти
    Pin *pins = (Pin *)shmat(shm_id, NULL, 0);
    if (pins == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    // Проверка булавок
    for (int i = 0; i < SHARED_MEMORY_SIZE / sizeof(Pin); ++i) {
        sem_wait(sem_id); // Ожидание доступа к разделяемой памяти
        pins[i].is_curved = (rand() % 2 == 0) ? 0 : 1; // 50% шанс сделать булавку не кривой
        printf("Pin %d checked.\n", i);
        sem_post(sem_id); // Освобождение доступа к разделяемой памяти
    }

    // Отсоединение разделяемой памяти
    if (shmdt(pins) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}
