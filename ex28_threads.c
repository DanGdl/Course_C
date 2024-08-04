/*
compile: gcc -Wall -Wextra -Wpedantic ex28_threads.c -lpthread -o ex28   OR: gcc ex28_threads.c -lpthread -o ex28
run: ./ex28
*/

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>


#define LEN_ARRAY(array) (sizeof(array)/sizeof(array[0]))
#define PRODUCE_CNT 100

#define SYNC_COND
// #define SYNC_MUTEX
// #define SYNC_SPINLOCK
// #define SYNC_SEMAPHORE


typedef struct State {
    unsigned int produced;
    unsigned int consumed;
    atomic_uchar is_active;

#if defined(SYNC_MUTEX)
    pthread_mutex_t mutex;
#elif defined(SYNC_COND)
    pthread_mutex_t mutex_cond;
    pthread_cond_t signal;  
#elif defined(SYNC_SPINLOCK)
    pthread_spinlock_t spin;
#elif defined(SYNC_SEMAPHORE)
    sem_t sem;
#endif
} State_t;


void thread_clear(void* parameters) {
    puts(__func__);
    State_t* const state = (State_t*) parameters;
    if (state == NULL) {
        return;
    }
#if defined(SYNC_MUTEX)
    pthread_mutex_unlock(&state->mutex);
#elif defined(SYNC_COND)
    pthread_mutex_unlock(&state->mutex_cond);
#elif defined(SYNC_SPINLOCK)
    pthread_spin_unlock(&state->spin);
#endif
}



static void consume(State_t* const state) {
    if (atomic_load(&state->is_active)) {
        state->produced--;
        state->consumed++;
        printf("Thread %ld consumes %d\n", pthread_self(), state->consumed);
    }
}

void* thread_consumer(void* params) {
    State_t* const state = (State_t*) params;
    printf("Thread %ld runs\n", pthread_self());
    pthread_cleanup_push(thread_clear, state);

    while(atomic_load(&state->is_active) != 0) {
#if defined(SYNC_MUTEX)
        if (pthread_mutex_lock(&state->mutex) != 0) {
            printf("Failed to lock mutex in consumer\n");
        }
        consume(state);
        pthread_mutex_unlock(&state->mutex);
#elif defined(SYNC_COND)
        if (pthread_mutex_lock(&state->mutex_cond) != 0) {
            printf("Failed to lock mutex in consumer\n");
        }
        pthread_cond_wait(&state->signal, &state->mutex_cond);
        consume(state);
        pthread_mutex_unlock(&state->mutex_cond);
#elif defined(SYNC_SPINLOCK)
        if (pthread_spin_lock(&state->spin) != 0) {
            printf("Failed to lock mutex in producer\n");
        }
        consume(state);
        pthread_spin_unlock(&state->spin);
#elif defined(SYNC_SEMAPHORE)
        sem_wait(&state->sem);
        consume(state);
#endif
    }
    printf("Thread %ld stops\n", pthread_self());
    pthread_cleanup_pop(0);
    pthread_exit(NULL);
    return NULL;
}

void* thread_producer(void* params) {
    State_t* const state = (State_t*) params;
    printf("Thread %ld runs\n", pthread_self());
    pthread_cleanup_push(thread_clear, state);

    for (int i = 0; i < PRODUCE_CNT; i++) {
#if defined(SYNC_MUTEX)
        if (pthread_mutex_lock(&state->mutex) != 0) {
            printf("Failed to lock mutex in producer\n");
        }
        state->produced++;
        pthread_mutex_unlock(&state->mutex);
#elif defined(SYNC_COND)
        if (pthread_mutex_lock(&state->mutex_cond) != 0) {
            printf("Failed to lock mutex in producer\n");
        }
        state->produced++;
        pthread_cond_signal(&state->signal);
        pthread_mutex_unlock(&state->mutex_cond);
#elif defined(SYNC_SPINLOCK)
        if (pthread_spin_lock(&state->spin) != 0) {
            printf("Failed to lock mutex in producer\n");
        }
        state->produced++;
        pthread_spin_unlock(&state->spin);
#elif defined(SYNC_SEMAPHORE)
        state->produced++;
        sem_post(&state->sem);
#endif

        usleep(1 * 1e3);
    }
    printf("Thread %ld stops\n", pthread_self());
    pthread_cleanup_pop(0);
    pthread_exit(NULL);
    return NULL;
}

int main (void) {
    State_t state = { .produced = 0, .consumed = 0, .is_active = 1, };
    pthread_t tid_producer = 0;
    pthread_t tid_consumers[2] = { 0 };

    do {
        // initialize synchronization
#if defined(SYNC_MUTEX)
        if (pthread_mutex_init(&state.mutex, NULL) != 0) {
            printf("Failed to initialize mutex: %s\n", strerror(errno));
            break;
        }
#elif defined(SYNC_COND)
        if (pthread_mutex_init(&state.mutex_cond, NULL) != 0) {
            printf("Failed to initialize mutex: %s\n", strerror(errno));
            break;
        }
        if (pthread_cond_init(&state.signal, NULL) != 0) {
            printf("Failed to initialize conditional variable: %s\n", strerror(errno));
            break;
        }
#elif defined(SYNC_SPINLOCK)
        if (pthread_spin_init(&state.spin, 0) != 0) {
            printf("Failed to init spinlock: %s\n", strerror(errno));
            break;
        }
#elif defined(SYNC_SEMAPHORE)
        if (sem_init(&state.sem, 0, 0) != 0) {
            printf("Failed to init semaphore: %s\n", strerror(errno));
            break;
        }
#endif
        // launch threads
        unsigned int i = 0;
        for(; i < LEN_ARRAY(tid_consumers); i++) {
            if (pthread_create(tid_consumers + i, NULL, &thread_consumer, &state) < 0) {
                printf("Failed to create consumer thread: %s\n", strerror(errno));
                break;
            } else {
                printf("Consumer thread created: %ld\n", tid_consumers[i]);
            }
        }
        if (i < 2) {
            break;
        }
        if (pthread_create(&tid_producer, NULL, &thread_producer, &state) < 0) {
            printf("Failed to create producer thread: %s\n", strerror(errno));
            break;
        } else {
            printf("Producer thread created: %ld\n", tid_producer);
        }

        // stop threads
        pthread_join(tid_producer, NULL);
        pthread_cancel(tid_consumers[0]);
        usleep(1 * 1e3); // wait little bit (tid_consumers[0] must wake up)

        atomic_store(&state.is_active, 0);
        // wakeup consumers
#if defined(SYNC_COND)
        if (pthread_mutex_lock(&state.mutex_cond) != 0) {
            printf("Failed to lock mutex in producer\n");
        }
        pthread_cond_broadcast(&state.signal);
        pthread_mutex_unlock(&state.mutex_cond);
#elif defined(SYNC_SEMAPHORE)
        sem_post(&state.sem);
#endif
        pthread_join(tid_consumers[1], NULL);

        tid_producer = 0;
        memset(tid_consumers, 0, sizeof(tid_consumers));
    } while(0);

    // clear stuff in case of error
    if (tid_producer != 0) {
        pthread_cancel(tid_producer);
    }
    for(unsigned int i = 0; i < LEN_ARRAY(tid_consumers); i++) {
        if (tid_consumers[i] != 0) {
            pthread_cancel(tid_consumers[i]);
        }
    }

    // clear memory
#if defined(SYNC_MUTEX)
    pthread_mutex_destroy(&state.mutex);
#elif defined(SYNC_COND)
    pthread_cond_destroy(&state.signal);
    pthread_mutex_destroy(&state.mutex_cond);
#elif defined(SYNC_SPINLOCK)
    pthread_spin_destroy(&state.spin);
#elif defined(SYNC_SEMAPHORE)
    sem_destroy(&state.sem);
#endif

    printf("PRODUCED %d, CONSUMED %d\n", PRODUCE_CNT, state.consumed);
    return 0;
}
