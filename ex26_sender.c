/*
compile: gcc -Wall -Wextra -Wpedantic -DFIFO ex26_sender.c -o ex26_sen_fifo   OR: gcc -DFIFO ex26_sender.c -o ex26_sen_fifo
compile: gcc -Wall -Wextra -Wpedantic -DXQUEUE ex26_sender.c -o ex26_sen_q_x   OR: gcc -DXQUEUE ex26_sender.c -o ex26_sen_q_x
compile: gcc -Wall -Wextra -Wpedantic -DVQUEUE ex26_sender.c -o ex26_sen_q_v   OR: gcc -DVQUEUE ex26_sender.c -o ex26_sen_q_v

compile: gcc -Wall -Wextra -Wpedantic -DVSMEM ex26_sender.c -o ex26_sen_sm_v   OR: gcc -DVSMEM ex26_sender.c -o ex26_sen_sm_v
compile: gcc -Wall -Wextra -Wpedantic -DXSMEM ex26_sender.c -o ex26_sen_sm_x   OR: gcc -DXSMEM ex26_sender.c -o ex26_sen_sm_x
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


int main(void) {
#ifdef FIFO
#define PATH_FIFO   "/tmp/example_fifo"

    // mkfifoat - allows relative path
    // errno 17 -> file exists (EEXIST)
    if (mkfifo(PATH_FIFO, 0777) < 0 && errno != 17) {
        printf("Failed to create FIFO: %s\n", strerror(errno));
        return 0;
    }
    // int fd = open(PATH_FIFO, O_RDWR);
    int fd = open(PATH_FIFO, O_WRONLY);
    if (fd < 0) {
        printf("Failed to open FIFO: %s\n", strerror(errno));
        return 0;
    }
    char message[56] = { 0 };
    for (int i = 0; i < 10; i++) {
        sprintf(message, "MESSAGE %d", i);
        printf("Sending message: %s\n", message);
        const int rc = write(fd, message, strlen(message));
        if (rc < 0) {
            printf("Failed to send message: %s\n", message);
        }
    }
    close(fd);
#endif


#ifdef XQUEUE
#define PATH_QUEUE   "/example_queue"
    // int rc = mq_unlink(PATH_QUEUE);
    // if (rc < 0 && errno != ENOENT) {
    //     printf("Failed to unlink previous queue: %s\n", strerror(errno));
    //     return 0;
    // }

    struct mq_attr attrs = {
        .mq_flags = 0777,       // Flags (ignored for mq_open())
        .mq_maxmsg = 10,        // Max. # of messages on queue
        .mq_msgsize = 55,       // Max. message size (bytes)
        .mq_curmsgs = 0,        // # of messages currently in queue (ignored for mq_open())
    };
    // S_IWUSR|S_IRUSR
    mqd_t queue = mq_open(PATH_QUEUE, O_CREAT | O_WRONLY, 0777, attrs);
    if (queue < 0) {
        printf("Failed to open queue: %s\n", strerror(errno));
        return 0;
    }
    sleep(2);
    char message[56] = { 0 };
    for (int i = 0; i < 10; i++) {
        sprintf(message, "MESSAGE %d", i);
        printf("Sending message: %s\n", message);
        const int rc = mq_send(queue, message, strlen(message), 0); // or mq_timedsend
        if (rc < 0) {
            printf("Failed to send message: %s, error %s\n", message, strerror(errno));
        }
    }
    mq_close(queue);
#endif


#ifdef VQUEUE
#define QUEUE_KEY           "/tmp/example_key"
#define QUEUE_PROJECT_ID    'Y'
    // verify that file exist before this call
    key_t queue = ftok(QUEUE_KEY, QUEUE_PROJECT_ID);
    if (queue < 0) {
        printf("Failed to get queue key: %s\n", strerror(errno));
        return 0;
    }

    int qid = msgget(queue, IPC_CREAT | 0777);
    if (qid < 0) {
        printf("Failed to open queue: %s\n", strerror(errno));
        return 0;
    }
    sleep(2);
    char message[56] = { 0 };
    for (int i = 0; i < 10; i++) {
        sprintf(message, "MESSAGE %d", i);
        printf("Sending message: %s\n", message);
        const int rc = msgsnd(qid, message, strlen(message), 0);
        if (rc < 0) {
            printf("Failed to send message: %s, error %s\n", message, strerror(errno));
        }
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
    int sem_id = semget(key_sem, 1, 0777 | IPC_CREAT);
    if (sem_id < 0) {
        printf("Failed to get semaphore id: %s\n", strerror(errno));
        return 0;
    }
    SemAttr_t sem_attr = { 0 };
    if (semctl(sem_id, 0, SETVAL, sem_attr) < 0) {
        printf("Failed to set initial value to semaphore: %s\n", strerror(errno));
        return 0;
    }
    key_t key_mem = ftok(MEM_KEY, SEM_ID);
    if (key_mem < 0) {
        printf("Failed to get shared memory key: %s\n", strerror(errno));
        return 0;
    }

    char message[56] = { 0 };
    int shm_id = shmget(key_mem, sizeof(message), 0777 | IPC_CREAT);
    if (shm_id < 0) {
        printf("Failed to get shared memory id: %s\n", strerror(errno));
        return 0;
    }

    char* msg_ptr = (char*) shmat(shm_id, NULL, 0); 
    if (msg_ptr < (char*) 0) {
        printf("Failed to map shared memory: %s\n", strerror(errno));
        return 0;
    }
    sleep(1);
    struct sembuf asem = { 0 };
    for (int i = 0; i < 10; i++) {
        sprintf(message, "MESSAGE %d", i);
        printf("Sending message: %s\n", message);
        memcpy(msg_ptr, message, strlen(message));
        asem.sem_op = 1;
        if (semop(sem_id, &asem, 1) < 0) {
            printf("Failed to notify semaphore: %s\n", strerror(errno));
        }
        sleep(1);
    }

    // receiver will close them
    // if (semctl(shm_id, 0, IPC_RMID) < 0) {
    //     printf("Failed to remove shared memory: %s\n", strerror(errno));
    //     return 0;
    // }
    // if (semctl(sem_id, 0, IPC_RMID) < 0) {
    //     printf("Failed to remove semaphore: %s\n", strerror(errno));
    //     return 0;
    // }
#endif



#ifdef XSMEM
#define SEM_KEY   "/example_semaphoreX"
#define MEM_KEY   "/example_smemoryX"
    sem_t* sem = sem_open(SEM_KEY, O_CREAT, 0777, 0);
    if (sem == SEM_FAILED) {
        printf("Failed to open a semaphore: %s\n", strerror(errno));
        return 0;
    }

    int fd_mem = shm_open(MEM_KEY, O_CREAT|O_RDWR, 0777);
    if (fd_mem < 0) {
        printf("Failed to open a shared memory: %s\n", strerror(errno));
        return 0;
    }

    char message[56] = { 0 };
    if (ftruncate(fd_mem, sizeof(message)) == -1) {
        printf("Failed to truncate shared memory: %s\n", strerror(errno));
        return 0;
    }
    char* shm = (char*) mmap(NULL, sizeof(message), PROT_WRITE, MAP_SHARED, fd_mem, 0);
    if (shm == MAP_FAILED) {
        printf("Failed to map shared memory: %s\n", strerror(errno));
        return 0;
    }

    for (int i = 0; i < 10; i++) {
        int current = 0;
        do {
            if (sem_getvalue(sem, &current) < 0) {
                printf("Failed to get value of semaphore: %s\n", strerror(errno));
            }
        } while(current > 0);

        sprintf(message, "MESSAGE %d", i);
        printf("Sending message: %s\n", message);
        memcpy(shm, message, strlen(message));
        if (sem_post(sem) < 0) {
            printf("Failed to post to semaphore: %s\n", strerror(errno));
        }
    }
    sleep(2);
    if (munmap(shm, sizeof(message))) {
        printf("Failed to unmap memory: %s\n", strerror(errno));
    }
    if (sem_close(sem) < 0) {
        printf("Failed to close semaphore: %s\n", strerror(errno));
    }
    if (shm_unlink(MEM_KEY) < 0) {
        printf("Failed to remove a shared memory: %s\n", strerror(errno));
    }
    if (sem_unlink(SEM_KEY) < 0) {
        printf("Failed to remove a semaphore: %s\n", strerror(errno));
    }
#endif
    return 0;
}
