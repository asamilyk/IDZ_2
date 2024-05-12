#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>

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

void quality_control_pins(int sem_id, int msg_id) {
    Message message;
    Pin pins[MAX_PINS];

    while (1) {
        msgrcv(msg_id, &message, sizeof(message) - sizeof(long), 3, 0);
        if (message.pin_index == -1)
            break;

        printf("Quality controlling pin %d...\n", message.pin_index);
        pins[message.pin_index].is_checked = 1;
        printf("Pin %d quality controlled.\n", message.pin_index);

        message.mtype = 1; // Возврат обратно для следующей проверки
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

    quality_control_pins(sem_id, msg_id);

    semctl(sem_id, 0, IPC_RMID, 0);
    msgctl(msg_id, IPC_RMID, NULL);

    return 0;
}
