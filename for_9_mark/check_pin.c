#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <time.h>

#define SHARED_MEMORY_SIZE 1024
#define SEMAPHORE_NAME "/my_semaphore"

typedef struct {
    int is_curved;
    int is_sharpened;
    int is_checked;
} Pin;

int main() {
    // Открытие именованного семафора
    sem_t *sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Создание разделяемой памяти
    int fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    ftruncate(fd, SHARED_MEMORY_SIZE);
    Pin *pins = mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pins == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    // Проверка булавок
    for (int i = 0; i < SHARED_MEMORY_SIZE / sizeof(Pin); ++i) {
        sem_wait(sem); // Ожидание доступа к разделяемой памяти
        printf("Checking pin %d...\n", i);
        pins[i].is_curved = (rand() % 2 == 0) ? 0 : 1; // 50% шанс сделать булавку не кривой
        printf("Pin %d checked.\n", i);
        sem_post(sem); // Освобождение доступа к разделяемой памяти
    }

    // Закрытие именованного семафора
    if (sem_close(sem) == -1) {
        perror("sem_close");
        exit(EXIT_FAILURE);
    }

    // Отсоединение разделяемой памяти
    if (munmap(pins, SHARED_MEMORY_SIZE) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    // Удаление разделяемой памяти
    if (shm_unlink("/my_shm") == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}
