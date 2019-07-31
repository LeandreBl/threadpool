#include <string.h>

#include "lthread_pool.h"

int lthread_pool_create(lthread_pool_t *handle, size_t count)
{
	memset(handle, 0, sizeof(*handle));
	if (lthread_pool_reserve(handle, count) == -1)
		goto error;
	return 0;
error:
	lthread_pool_destroy(handle);
	return -1;
}

void lthread_pool_destroy(lthread_pool_t *handle)
{
	clist_destroy(handle->threads);
}
