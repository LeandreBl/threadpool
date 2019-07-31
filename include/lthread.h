#ifndef _LTHREAD_H_
#define _LTHREAD_H_

#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <semaphore.h>

#include <stdio.h>

struct lthread_s;

typedef void (*lthread_function_t)(const struct lthread_s *thread);

typedef struct lthread_s {
	pthread_t thread;
	sem_t sem_sync;
	sem_t sem_wait;
	lthread_function_t call;
	void *userdata;
	bool is_running : 1;
	bool is_canceled : 1;
} lthread_t;

int lthread_create(lthread_t *thread, lthread_function_t function, void *userdata);
void lthread_set_function(lthread_t *thread, lthread_function_t function, void *userdata);
int lthread_launch(lthread_t *thread, useconds_t delay);
int lthread_wait(lthread_t *thread, long timeout);
void lthread_destroy(lthread_t *thread);

#endif /* !_LTHREAD_H_ */
