#include "lthread_pool.h"

static size_t count_running(const lthread_pool_t *pool)
{
	size_t count = 0;

	clist_foreach(thread, pool->threads)
	{
		if (thread->is_running == true)
			++count;
	}
	return count;
}

int lthread_pool_wait_all(lthread_pool_t *handle, lthread_pool_callback_t callback, void *userdata,
			  long timeout)
{
	size_t count = count_running(handle);
	long t = (timeout < 0) ? timeout : (timeout / (ssize_t)count);

	clist_foreach(thread, handle->threads)
	{
		if (thread->is_running) {
			if (lthread_wait(thread, t) == -1)
				return -1;
			if (callback != NULL)
				callback(handle, thread, userdata);
		}
	}
	return 0;
}
