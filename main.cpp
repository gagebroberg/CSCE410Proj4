/* This is the main body of the test program.
 * DO NOT write any of your core functionality here.
 * You are free to modify this file but be aware that
 * this file will be replace during grading. */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "uthread.h"

void foo(void* arg)
{
    	for (int i = 0; i < 10; i++) {
        	printf("Thread %lu\n", (unsigned long) arg);
        	usleep(1000);
    	}
    	uthread_exit();
}

void bar(void* arg)
{
	uthread_set_param((unsigned long) arg);
    	for (int i = 0; i < 10; i++) {
        	usleep(1000);
		uthread_yield();
    	}
	printf("Thread %lu is done.\n", (unsigned long) arg);
    	uthread_exit();
}

int main(int argc, const char** argv)
{
	int test = -1;
	if (argc > 1) {
		test = atoi(argv[1]);
	}

	switch(test) {
		case 1:
			puts("\e[34m# Test 1: Task 1.\e[0m");
			uthread_set_policy(UTHREAD_DIRECT_PTHREAD);
			uthread_init();
    			uthread_create(foo, (void *)1);
   		 	uthread_create(foo, (void *)2);
    			uthread_cleanup();
			break;
		case 2:
			puts("\e[34m# Test 2: Task 2, fewer uthreads than pthreads.\e[0m");
			uthread_set_policy(UTHREAD_PRIORITY);
			uthread_init();
    			uthread_create(foo, (void *)1);
   		 	uthread_create(foo, (void *)2);
    			uthread_cleanup();
			break;
		case 3:
			puts("\e[34m# Test 3: Task 2, equal number of uthreads and pthreads.\e[0m");
			uthread_set_policy(UTHREAD_PRIORITY);
			uthread_init();
			for (size_t i = 0; i < 4; i++) {
	    			uthread_create(foo, (void *)i);
			}
    			uthread_cleanup();
			break;
		case 4:
			puts("\e[34m# Test 4: Task 2, more uthreads than pthreads.\e[0m");
			uthread_set_policy(UTHREAD_PRIORITY);
			uthread_init();
			for (size_t i = 0; i < 5; i++) {
	    			uthread_create(foo, (void *)i);
			}
    			uthread_cleanup();
			break;
		case 5:
			puts("\e[34m# Test 5: Task 2, trying priority stuff. Expect threads 0-3 to finish first, then 4-7.\e[0m");
			uthread_set_policy(UTHREAD_PRIORITY);
			uthread_init();
			for (size_t i = 0; i < 4; i++) {
	    			uthread_create(foo, (void *)i);
			}
    			uthread_cleanup();
			break;
		default:
			puts("Invalid test number");
			break;
	}
	return 0;
}
