/*
compile: gcc -Wall -Wextra -Wpedantic -DFIFO ex26_receiver.c -o ex26_rec_fifo    OR: gcc -DFIFO ex26_receiver.c -o ex26_rec_fifo
compile: gcc -Wall -Wextra -Wpedantic -DXQUEUE ex26_receiver.c -o ex26_rec_q_x   OR: gcc -DXQUEUE ex26_receiver.c -o ex26_rec_q_x
compile: gcc -Wall -Wextra -Wpedantic -DVQUEUE ex26_receiver.c -o ex26_rec_q_v   OR: gcc -DVQUEUE ex26_receiver.c -o ex26_rec_q_v

_GNU_SOURCE - to use semtimedop
compile: gcc -Wall -Wextra -Wpedantic -DVSMEM -D_GNU_SOURCE ex26_receiver.c -o ex26_rec_sm_v   OR: gcc -DVSMEM -D_GNU_SOURCE ex26_receiver.c -o ex26_rec_sm_v
compile: gcc -Wall -Wextra -Wpedantic -DXSMEM ex26_receiver.c -o ex26_rec_sm_x   OR: gcc -DXSMEM ex26_receiver.c -o ex26_rec_sm_x
*/

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>      // errno
#include <string.h>     // strerror
#include <stdlib.h>
#include <unistd.h>     // close

#ifdef FIFO
#include <sys/stat.h>   // mkfifo
#endif

#ifdef XQUEUE
#include <time.h>
#include <mqueue.h>     // mq_open
#endif

#ifdef VQUEUE
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#endif

#ifdef VSMEM
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>

typedef union SemAttr {
    int val;
    struct semid_ds* buf;
    ushort array[1];
} SemAttr_t;
#endif

#ifdef XSMEM
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#endif


#define LEN_ARRAY(x)    (sizeof(x)/sizeof(x[0]))
#define LEN_STR(x)      (LEN_ARRAY(x) - 1)


int main(void) {
#ifdef FIFO
#define PATH_FIFO   "/tmp/example_fifo"

    // mkfifoat - allows relative path
    // errno 17 -> file exists
    if (mkfifo(PATH_FIFO, 0777) < 0 && errno != 17) {
        printf("Failed to create FIFO: %s\n", strerror(errno));
        return 0;
    }
    int fd = open(PATH_FIFO, O_RDONLY);
    if (fd < 0) {
        printf("Failed to open FIFO: %s\n", strerror(errno));
        return 0;
    }
    char message[56] = { 0 };
    while (1) {
        const int rc = read(fd, message, LEN_STR(message));
        if (rc < 0) {
            printf("Failed to receive message: %s\n", strerror(errno));
        } else if (rc == 0) {
            printf("FIFO is empty\n");
            break;
        } else {
            message[rc] = '\0';
            printf("Received message: %s\n", message);
        }
    }
    close(fd);
#endif


#ifdef XQUEUE
#define PATH_QUEUE   "/example_queue"
    // sender will create a queue. Receiver need to open it
    mqd_t queue = mq_open(PATH_QUEUE, O_RDONLY);
    if (queue < 0) {
        printf("Failed to open queue: %s\n", strerror(errno));
        return 0;
    }
    char message[129] = { 0 };
    while (1) {
        struct timespec ts = { 0 };
        if (clock_gettime(CLOCK_REALTIME, &ts) < 0) {
            printf("Failed to setup time stamp: %s\n", strerror(errno));
        }
        ts.tv_sec += 5;
        unsigned int priority = 0;
        // const int rc = mq_receive(queue, message, LEN_STR(message), &priority);
        const int rc = mq_timedreceive(queue, message, LEN_STR(message), &priority, &ts);
        if (rc < 0) {
            printf("Failed to receive message: %s\n", strerror(errno));
            if (errno == ETIMEDOUT) {
                break;
            }
        } else {
            message[rc] = '\0';
            printf("Received message: %s\n", message);
        }
    }
    mq_close(queue);
#endif


#ifdef VQUEUE
#define QUEUE_KEY   "/tmp/example_key"
#define QUEUE_ID    'Y'
    // verify that file exist before this call
    key_t queue = ftok(QUEUE_KEY, QUEUE_ID);
    if (queue < 0) {
        printf("Failed to get queue key: %s\n", strerror(errno));
        return 0;
    }

    int qid = msgget(queue, 0777);
    if (qid < 0) {
        printf("Failed to get queue: %s\n", strerror(errno));
        return 0;
    }
    char message[56] = { 0 };
    for (int i = 0; i < 10; i++) {
        const int rc = msgrcv(qid, message, LEN_STR(message), 0, 0);
        if (rc < 0) {
            printf("Failed to receive message: %s\n", strerror(errno));
            if (errno == ETIMEDOUT) {
                break;
            }
        } else {
            message[rc] = '\0';
            printf("Received message: %s\n", message);
        }
    }
    if (msgctl(qid, IPC_RMID, NULL) == -1) {
        printf("Failed to remove queue: %s\n", strerror(errno));
    }
#endif


#ifdef VSMEM
#define SEM_KEY   "/tmp/example_semaphore"
#define MEM_KEY   "/tmp/example_smemory"
#define SEM_ID    'Y'
    // verify that file exist before this call
    key_t key_sem = ftok(SEM_KEY, SEM_ID);
    if (key_sem < 0) {
        printf("Failed to get queue key: %s\n", strerror(errno));
        return 0;
    }
    int sem_id = semget(key_sem, 1, 0 /*it already exists*/);
    if (sem_id < 0) {
        printf("Failed to get semaphore id: %s\n", strerror(errno));
        return 0;
    }
    key_t key_mem = ftok(MEM_KEY, SEM_ID);
    if (key_mem < 0) {
        printf("Failed to get shared memory key: %s\n", strerror(errno));
        return 0;
    }

    char message[56] = { 0 };
    int shm_id = shmget(key_mem, sizeof(message), 0);
    if (shm_id < 0) {
        printf("Failed to get shared memory id: %s\n", strerror(errno));
        return 0;
    }

    char* msg_ptr = (char*) shmat(shm_id, NULL, 0); 
    if (msg_ptr < (char*) 0) {
        printf("Failed to map shared memory: %s\n", strerror(errno));
        return 0;
    }

    struct sembuf asem = { 0 };
    for (int i = 0; i <= 10; i++) {
        struct timespec ts = { 0 };
        ts.tv_sec = 3;
        asem.sem_op = -1;
        if (semtimedop(sem_id, &asem, 1, &ts) < 0) {
            printf("Failed to wait on semaphore: %s\n", strerror(errno));
            if (errno == EAGAIN && i >= 10) {
                break;
            }
        } else {
            memcpy(message, msg_ptr, sizeof(message));
            message[LEN_STR(message)] = '\0';
            printf("Received message: %s\n", message);
        }
    }

    if (shmdt((void*) msg_ptr) < 0) {
    	printf("Failed to remove msg pointer: %s\n", strerror(errno));
    }
    if (shmctl(shm_id, IPC_RMID, NULL) < 0) {
        printf("Failed to remove shared memory: %s\n", strerror(errno));
    }
    if (semctl(sem_id, 0, IPC_RMID) < 0) {
        printf("Failed to remove semaphore: %s\n", strerror(errno));
    }
#endif


#ifdef XSMEM
#define SEM_KEY   "/example_semaphoreX"
#define MEM_KEY   "/example_smemoryX"
    sem_t* sem = sem_open(SEM_KEY, 0, 0777, 0);
    if (sem == SEM_FAILED) {
        printf("Failed to open a semaphore: %s\n", strerror(errno));
        return 0;
    }
    int fd_mem = shm_open(MEM_KEY, O_RDONLY, 0777);
    if (fd_mem < 0) {
        printf("Failed to open a shared memory: %s\n", strerror(errno));
        return 0;
    }

    char message[56] = { 0 };
    char* shm = (char*) mmap(NULL, sizeof(message), PROT_READ, MAP_SHARED, fd_mem, 0);
    if (shm == MAP_FAILED) {
        printf("Failed to map shared memory: %s\n", strerror(errno));
        return 0;
    }

    for (int i = 0; i < 10; i++) {
        if (sem_wait(sem) < 0) {
            printf("Failed to wait on semaphore: %s\n", strerror(errno));
        }
        memcpy(message, shm, strlen(shm));
        printf("Received message: %s\n", message);
    }
    if (munmap(shm, sizeof(message))) {
        printf("Failed to unmap memory: %s\n", strerror(errno));
    }
    if (sem_close(sem) < 0) {
        printf("Failed to close semaphore: %s\n", strerror(errno));
    }
#endif
    return 0;
}
