#include "lthread.h"

void lthread_set_function(lthread_t *thread, lthread_function_t function, void *userdata)
{
	thread->call = function;
	thread->userdata = userdata;
}