#include "lthread.h"

void lthread_set_function(lthread_t *thread, void *(* function)(void *), void *data)
{
  thread->call = function;
  thread->data = data;
}