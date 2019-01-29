#ifndef _LTHREAD_H_
#define _LTHREAD_H_

#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <semaphore.h>

#include <stdio.h>

typedef struct lthread_s {
  pthread_t thread;
  sem_t sem_sync;
  sem_t sem_wait;
  void *(* call)(void *);
  void *data;
  void *returned;
  bool is_running : 1;
  bool is_paused : 1;
} lthread_t;

int lthread_create(lthread_t *thread, void *(* function)(void *), void *data);
void lthread_set_function(lthread_t *thread, void *(* function)(void *), void *data);
int lthread_launch(lthread_t *thread, useconds_t delay);
int lthread_wait(lthread_t *thread, void **returned, long timeout);
void lthread_destroy(lthread_t *thread);

#endif /* !_LTHREAD_H_ */