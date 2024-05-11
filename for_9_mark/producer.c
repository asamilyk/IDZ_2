#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>

#define SHM_SIZE 1024
#define SEM_NAME "/pin_semaphore"

typedef struct {
    int is_produced;
} Pin;

int main() {
    srand(time(NULL));

    // Открытие именованного семафора
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Создание и открытие разделяемой памяти
    int shm_fd = shm_open("/pin_memory", O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    ftruncate(shm_fd, SHM_SIZE);
    Pin *pins = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (pins == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Производство булавок
    while (1) {
        sem_wait(sem); // Ожидание доступа к разделяемой памяти
        if (!pins->is_produced) {
            pins->is_produced = 1;
            printf("Produced a pin.\n");
        }
        sem_post(sem); // Освобождение доступа к разделяемой памяти
        sleep(rand() % 2 + 1); // Имитация времени производства
    }

    // Закрытие семафора
    sem_close(sem);

    return 0;
}
