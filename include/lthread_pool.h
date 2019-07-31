#ifndef _LTHREAD_POOL_H_
#define _LTHREAD_POOL_H_

#include <lthread.h>
#include <clist.h>

typedef struct lblthread_pool_s {
	clist(lthread_t) * threads;
} lthread_pool_t;

typedef void (*lthread_pool_callback_t)(const lthread_pool_t *handle, const lthread_t *thread,
					void *userdata);

int lthread_pool_create(lthread_pool_t *handle, size_t count);
int lthread_pool_reserve(lthread_pool_t *handle, size_t count);
int lthread_pool_run(lthread_pool_t *handle, lthread_function_t function, void *userdata);
int lthread_pool_wait_all(lthread_pool_t *handle, lthread_pool_callback_t callback, void *userdata,
			  long timeout);
void lthread_pool_destroy(lthread_pool_t *handle);

#endif /* !_LTHREAD_POOL_H_ */
