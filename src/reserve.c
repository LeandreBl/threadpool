#include "lthread_pool.h"

int lthread_pool_reserve(lthread_pool_t *handle, size_t count)
{
	for (size_t size = clist_size(handle->threads); size < count; ++size) {
		clist_emplace_back(handle->threads, lthread_destroy, lthread_create, NULL, NULL);
		if (size + 1 != clist_size(handle->threads))
			return -1;
	}
	return 0;
}
