#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>

#define SHM_SIZE 1024
#define SHM_KEY 1234

typedef struct {
    int is_produced;
} Pin;

int main() {
    srand(time(NULL));

    // Получение идентификатора разделяемой памяти
    int shm_id = shmget(SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT);
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

    // Производство булавок
    while (1) {
        sleep(rand() % 2 + 1); // Имитация времени производства
        pins->is_produced = 1;
        printf("Produced a pin.\n");
    }

    return 0;
}
