#include <errno.h>
#include <string.h>

#include "lthread.h"

static int wake_waiters(lthread_t *thread)
{
	int sval;

	if (sem_getvalue(&thread->sem_wait, &sval) == -1
	    || (sval == 0 && sem_post(&thread->sem_wait) == -1))
		return (-1);
	return (0);
}

static void *caller_wrapper(lthread_t *thread)
{
	thread->is_canceled = false;
	while (thread->is_canceled == false) {
		if (sem_wait(&thread->sem_sync) != 0) {
			fprintf(stderr, "Error: lthread: semaphore: %s\n", strerror(errno));
			break;
		}
		if (thread->is_canceled == true)
			break;
		thread->call(thread);
		if (wake_waiters(thread) == -1)
			break;
		thread->is_running = false;
	}
	return NULL;
}

int lthread_create(lthread_t *thread, lthread_function_t function, void *userdata)
{
	thread->call = function;
	thread->userdata = userdata;
	thread->is_running = false;
	if (sem_init(&thread->sem_sync, 0, 0) == -1 || sem_init(&thread->sem_wait, 0, 0) == -1)
		return (-1);
	if (pthread_create(&thread->thread, NULL, (void *(*)(void *)) & caller_wrapper, thread)
	    == -1)
		return (-1);
	return (0);
}

void lthread_destroy(lthread_t *thread)
{
	int ret;

	thread->is_canceled = true;
	sem_post(&thread->sem_sync);
	ret = pthread_join(thread->thread, NULL);
	if (ret < 0)
		fprintf(stderr, "Error: lthread: pthread_join: %s\n", strerror(-ret));
	sem_destroy(&thread->sem_sync);
	sem_destroy(&thread->sem_wait);
}
