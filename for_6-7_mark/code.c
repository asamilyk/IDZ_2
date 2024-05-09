#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
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

void check_pin(Pin *pin, int index) {
    // Проверка случайной булавки
    usleep(rand() % 1000); // Имитация времени проверки
    pin->is_curved = (rand() % 2 == 0) ? 0 : 1; // 50% шанс сделать булавку не кривой
    printf("Worker %d checked pin.\n", index);
}

void sharpen_pin(Pin *pin, int index) {
    // Заточка случайной булавки
    usleep(rand() % 1000); // Имитация времени заточки
    pin->is_sharpened = 1;
    printf("Worker %d sharpened pin.\n", index);
}

void quality_control(Pin *pin, int index) {
    // Проверка качества случайной булавки
    usleep(rand() % 1000); // Имитация времени контроля качества
    pin->is_checked = 1;
    printf("Worker %d quality controlled pin.\n", index);
}

int main() {
    // Создание и инициализация именованного семафора
    sem_t *sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

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

    // Создание рабочих процессов для каждого участка
    pid_t pid;
    srand(time(NULL));

    // Создание процессов для проверки
    for (int i = 0; i < 3; ++i) {
        pid = fork();
        if (pid == 0) { // Child process
            for (int j = i; j < SHARED_MEMORY_SIZE / sizeof(Pin); j += 3) {
                sem_wait(sem); // Ожидание доступа к разделяемой памяти
                check_pin(&pins[j], i); // Проверка булавки
                sem_post(sem); // Освобождение доступа к разделяемой памяти
            }
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Создание процессов для заточки
    for (int i = 0; i < 5; ++i) {
        pid = fork();
        if (pid == 0) { // Child process
            for (int j = i; j < SHARED_MEMORY_SIZE / sizeof(Pin); j += 5) {
                sem_wait(sem); // Ожидание доступа к разделяемой памяти
                if (!pins[j].is_curved) {
                    sharpen_pin(&pins[j], i); // Заточка булавки, если она не кривая
                }
                sem_post(sem); // Освобождение доступа к разделяемой памяти
            }
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Создание процессов для контроля качества
    for (int i = 0; i < 2; ++i) {
        pid = fork();
        if (pid == 0) { // Child process
            for (int j = i; j < SHARED_MEMORY_SIZE / sizeof(Pin); j += 2) {
                sem_wait(sem); // Ожидание доступа к разделяемой памяти
                if (pins[j].is_sharpened) {
                    quality_control(&pins[j], i); // Контроль качества заточенной булавки
                }
                sem_post(sem); // Освобождение доступа к разделяемой памяти
            }
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Ожидание завершения всех дочерних процессов
    for (int i = 0; i < 10; ++i) {
        wait(NULL);
    }
// Отсоединение разделяемой памяти
    if (shmdt(pins) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Удаление разделяемой памяти
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    // Закрытие и удаление именованного семафора
    if (sem_close(sem) == -1) {
        perror("sem_close");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink(SEMAPHORE_NAME) == -1) {
        perror("sem_unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}
