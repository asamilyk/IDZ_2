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

typedef struct {
    int is_curved;
    int is_sharpened;
    int is_checked;
} Pin;

void check_pin(Pin *pin, int worker_id) {
    // Проверка случайной булавки
    usleep(rand() % 1000); // Имитация времени проверки
    pin->is_curved = (rand() % 2 == 0) ? 0 : 1; // 50% шанс сделать булавку не кривой
    printf("Worker %d checked pin\n", worker_id);
}

void sharpen_pin(Pin *pin, int worker_id) {
    // Заточка случайной булавки
    usleep(rand() % 1000); // Имитация времени заточки
    pin->is_sharpened = 1;
    printf("Worker %d sharpened pin\n", worker_id);
}

void quality_control(Pin *pin, int worker_id) {
    // Проверка качества случайной булавки
    usleep(rand() % 1000); // Имитация времени контроля качества
    pin->is_checked = 1;
    printf("Worker %d checked quality\n", worker_id);
}

int main() {
    // Создание семафора
    int sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Инициализация семафора
    if (semctl(sem_id, 0, SETVAL, 1) == -1) {
        perror("semctl");
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
            int worker_id = i;
            for (int j = i; j < SHARED_MEMORY_SIZE / sizeof(Pin); j += 3) {
                struct sembuf sops = {0, -1, SEM_UNDO};
                semop(sem_id, &sops, 1); // Ожидание доступа к разделяемой памяти
                check_pin(&pins[j], worker_id); // Проверка булавки
                sops.sem_op = 1;
                semop(sem_id, &sops, 1); // Освобождение доступа к разделяемой памяти
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
            int worker_id = i + 3;
            for (int j = i; j < SHARED_MEMORY_SIZE / sizeof(Pin); j += 5) {
                struct sembuf sops = {0, -1, SEM_UNDO};
                semop(sem_id, &sops, 1); // Ожидание доступа к разделяемой памяти
                if (!pins[j].is_curved) {
                    sharpen_pin(&pins[j], worker_id); // Заточка булавки, если она не кривая
                }
                sops.sem_op = 1;
                semop(sem_id, &sops, 1); // Освобождение доступа к разделяемой памяти
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
            int worker_id = i + 8;
            for (int j = i; j < SHARED_MEMORY_SIZE / sizeof(Pin); j += 2) {
                struct sembuf sops = {0, -1, SEM_UNDO};
                semop(sem_id, &sops, 1); // Ожидание доступа к разделяемой памяти
                if (pins[j].is_sharpened) {
                    quality_control(&pins[j], worker_id); // Контроль качества заточенной булавки
                }
                sops.sem_op = 1;
                semop(sem_id, &sops, 1); // Освобождение доступа к разделяемой памяти
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
    if (shmctl(shm_id, IPC_RMID, NULL) == -

            1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    // Удаление семафора
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}
