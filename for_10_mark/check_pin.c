#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <time.h>

#define MAX_PINS 100

typedef struct {
    long mtype;
    int pin_index;
    int is_curved;
} Message;

typedef struct {
    int is_curved;
    int is_sharpened;
    int is_checked;
} Pin;

void check_pins(int sem_id, int msg_id) {
    srand(time(NULL));
    Message message;
    Pin pins[MAX_PINS];

    while (1) {
        msgrcv(msg_id, &message, sizeof(message) - sizeof(long), 1, 0);
        if (message.pin_index == -1)
            break;

        printf("Checking pin %d...\n", message.pin_index);
        pins[message.pin_index].is_curved = (rand() % 2 == 0) ? 0 : 1;
        printf("Pin %d checked.\n", message.pin_index);

        message.mtype = message.pin_index + 2;
        message.is_curved = pins[message.pin_index].is_curved;
        msgsnd(msg_id, &message, sizeof(message) - sizeof(long), 0);
    }
}

int main() {
    int sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    semctl(sem_id, 0, SETVAL, 1);

    int msg_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (msg_id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    check_pins(sem_id, msg_id);

    semctl(sem_id, 0, IPC_RMID, 0);
    msgctl(msg_id, IPC_RMID, NULL);

    return 0;
}
