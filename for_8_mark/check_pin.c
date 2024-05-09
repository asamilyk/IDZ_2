#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024
#define SHM_KEY 1234

typedef struct {
    int is_produced;
} Pin;

int main() {
    // Получение идентификатора разделяемой памяти
    int shm_id = shmget(SHM_KEY, SHM_SIZE, 0666);
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

    // Проверка булавок
    while (1) {
        if (pins->is_produced) {
            printf("Checked a pin.\n");
            pins->is_produced = 0; // Сброс состояния булавки после проверки
        }
        sleep(1); // Имитация времени проверки
    }

    return 0;
}
