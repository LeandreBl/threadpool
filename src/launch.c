#include <unistd.h>

#include "lthread.h"

int lthread_launch(lthread_t *thread, useconds_t delay)
{
  if (delay)
    usleep(delay);
  if (sem_post(&thread->sem_sync) == -1)
    return (-1);
  thread->is_running = true;
  return (0);
}