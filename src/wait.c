#include "lthread.h"

int lthread_wait(lthread_t *thread, long ms_timeout)
{
	struct timespec ts = {
		ms_timeout / 1000,
		(ms_timeout - ms_timeout / 1000) * 1000000,
	};

	if (thread->is_running == false)
		return (-1);
	if ((ms_timeout < 0 && sem_wait(&thread->sem_wait) == -1)
	    || (ms_timeout > 0 && sem_timedwait(&thread->sem_wait, &ts) == -1))
		return (-1);
	return (0);
}