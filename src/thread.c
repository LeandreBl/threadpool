#include <errno.h>
#include <string.h>

#include "lthread.h"

static int wake_waiters(lthread_t *thread)
{
	int sval;

	if (sem_getvalue(&thread->sem_wait, &sval) == -1 || (sval == 0 && sem_post(&thread->sem_wait) == -1))
		return (-1);
	return (0);
}

static void *caller_wrapper(lthread_t *thread)
{
	while (1) {
		if (sem_wait(&thread->sem_sync) != 0)
			break;
		thread->returned = thread->call(thread->data);
		if (wake_waiters(thread) == -1)
			break;
		thread->is_running = false;
		thread->is_paused = false;
	}
	fprintf(stderr, "Error: lthread: sem_wait: %s\n", strerror(errno));
	return (NULL);
}

int lthread_create(lthread_t *thread, void *(*function)(void *), void *data)
{
	thread->call = function;
	thread->data = data;
	thread->is_paused = false;
	thread->is_running = false;
	thread->returned = NULL;
	if (sem_init(&thread->sem_sync, 0, 0) == -1 || sem_init(&thread->sem_wait, 0, 0) == -1)
		return (-1);
	if (pthread_create(&thread->thread, NULL, (void *(*)(void *)) & caller_wrapper, thread) == -1)
		return (-1);
	return (0);
}

void lthread_destroy(lthread_t *thread)
{
	sem_destroy(&thread->sem_sync);
	sem_destroy(&thread->sem_wait);
	pthread_cancel(thread->thread);
}