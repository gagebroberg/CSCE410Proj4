
/* Write your core functionality in this file. */

#include <stdio.h>
#include <pthread.h>
#include <queue>
#include "uthread.h"
#include "concurrent_queue.h"
#include "concurrent_queue.cpp"

// Kernel threads
pthread_t thread_list[4];

// User space thread queue
concurrent_queue<uthread> user_queue;

// current_policy
enum uthread_policy current_policy;

// handler function for the kernel threads
void handler(void* arg) {
	printf("In handler number %d\n", arg);
	while (true) {
		/* Select a user-space thread from the ready queue */
		struct uthread user_thread;
		/* Take the thread off the ready queue */
		user_queue.try_pop(user_thread);
		/* Run the task of the user-space thread */
		user_thread.func(user_thread.arg);
		uthread_exit();
	}
	return;
}

void uthread_init(void)
{
	/* Use pthread_create() to create 4 kernel threads which will schedule
	any user-space thread to run */
	pthread_create(&thread_list[0], NULL, (void*(*)(void*)) handler, (void*) 1);
	pthread_create(&thread_list[1], NULL, (void*(*)(void*)) handler, (void*) 2);
	pthread_create(&thread_list[2], NULL, (void*(*)(void*)) handler, (void*) 3);
	pthread_create(&thread_list[3], NULL, (void*(*)(void*)) handler, (void*) 4);
}

void uthread_create(void (*func) (void*), void* arg)
{
	if (current_policy == UTHREAD_DIRECT_PTHREAD) {
		// create a uthread struct

		struct uthread user_thread;
		pthread_t new_id;
		user_thread.uthread_id = new_id;
		user_thread.func = (void*(*)(void*)) func;
		user_thread.arg = arg;

		// push the uthread to the queue
		user_queue.push(user_thread);

		// create the uthread
		pthread_create(&user_queue.front().uthread_id, NULL, (void*(*)(void*))func, arg);
	} else if (current_policy == UTHREAD_PRIORITY) {
		// Do something
	}
}

void uthread_exit(void)
{
	pthread_exit(NULL);
}

void uthread_yield(void)
{
	pthread_yield();
}

void uthread_cleanup(void)
{
	for (int i = 0; i < 4; i++) {
		pthread_join(thread_list[i], NULL);
	}
}

void uthread_set_policy(enum uthread_policy policy)
{
	current_policy = policy;
}

void uthread_set_param(int param)
{
}
