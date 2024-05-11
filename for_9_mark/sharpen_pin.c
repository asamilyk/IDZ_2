#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>

#define SHM_SIZE 1024
#define SEM_NAME "/pin_semaphore"

typedef struct {
    int is_produced;
} Pin;

int main() {
    // Открытие именованного семафора
    sem_t *sem = sem_open(SEM_NAME, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Открытие разделяемой памяти
    int shm_fd = shm_open("/pin_memory", O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    Pin *pins = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (pins == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Заточка булавок
    while (1) {
        sem_wait(sem); // Ожидание доступа к разделяемой памяти
        if (pins->is_produced) {
            printf("Sharpened a pin.\n");
            pins->is_produced = 0; // Сброс состояния булавки после заточки
        }
        sem_post(sem); // Освобождение доступа к разделяемой памяти
        sleep(1); // Имитация времени заточки
    }

    // Закрытие семафора
    sem_close(sem);

    return 0;
}
