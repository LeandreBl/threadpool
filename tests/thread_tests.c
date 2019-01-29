#include <criterion/criterion.h>

#include "lthread.h"

void *func(void *data)
{
  int *i = data;

  *i = 1;
  return (data);
}

void *funcf(void *data)
{
  int *i = data;

  *i = 2;
  return (data);
}

Test(create, lthread)
{
  lthread_t thread;
  int val = 0;
  void *ret;

  cr_assert(lthread_create(&thread, &func, &val) == 0);
  lthread_launch(&thread, 0);
  lthread_wait(&thread, &ret, -1);
  cr_assert(ret == &val);
  cr_assert(val == 1);
  lthread_destroy(&thread);
}

Test(set_function, lthread)
{
  lthread_t thread;
  int val = 0;
  void *ret;

  cr_assert(lthread_create(&thread, &func, &val) == 0);
  cr_assert(lthread_launch(&thread, 0) == 0);
  lthread_wait(&thread, &ret, -1);
  cr_assert(ret == &val);
  cr_assert(val == 1);
  lthread_set_function(&thread, &funcf, &val);
  cr_assert(lthread_launch(&thread, 0) == 0);
  lthread_wait(&thread, &ret, -1);
  cr_assert(ret == &val);
  cr_assert(val == 2);
  lthread_destroy(&thread);
}