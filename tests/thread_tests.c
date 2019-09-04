#include <criterion/criterion.h>

#include "lthread.h"

void func(const lthread_t *thread)
{
	int *i = thread->userdata;

	*i = 1;
}

void funcf(const lthread_t *thread)
{
	int *i = thread->userdata;

	*i = 2;
}

Test(create, lthread)
{
	lthread_t thread;
	int val = 0;

	cr_assert(lthread_create(&thread, &func, &val) == 0);
	lthread_launch(&thread, 0);
	lthread_wait(&thread, -1);
	cr_assert(val == 1);
	lthread_destroy(&thread);
}

Test(set_function, lthread)
{
	lthread_t thread;
	int val = 0;

	cr_assert(lthread_create(&thread, &func, &val) == 0);
	cr_assert(lthread_launch(&thread, 0) == 0);
	lthread_wait(&thread, -1);
	cr_assert(val == 1);
	lthread_set_function(&thread, &funcf, &val);
	cr_assert(lthread_launch(&thread, 0) == 0);
	lthread_wait(&thread, -1);
	cr_assert(val == 2);
	lthread_destroy(&thread);
}