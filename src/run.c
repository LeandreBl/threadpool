#include <stdlib.h>

#include "lthread_pool.h"

static int set_and_start(lthread_t *thread, lthread_function_t function, void *data)
{
	lthread_set_function(thread, function, data);
	if (lthread_launch(thread, 0) == -1)
		return -1;
	return 0;
}

int lthread_pool_run(lthread_pool_t *handle, lthread_function_t function, void *data)
{
	size_t size = clist_size(handle->threads);

	clist_foreach(thread, handle->threads)
	{
		if (thread->is_running == false) {
			if (set_and_start(thread, function, data) == -1)
				return -1;
			return 0;
		}
	}
	if (lthread_pool_reserve(handle, (size == 0) ? 1 : size * 2) == -1)
		return -1;
	return lthread_pool_run(handle, function, data);
}
