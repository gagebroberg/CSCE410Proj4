
/* Write your core functionality in this file. */

#include <pthread.h>
#include <queue>
#include "uthread.h"

// Kernel threads
pthread_t thread_list[4];

// User space thread queue
std::queue<pthread_t> user_queue;

// current_policy
enum uthread_policy current_policy;

// handler function for the kernel threads
void handler(void* arg) {
	while (true) {
		/* Select a user-space thread from the ready queue */
		pthread_t uthread = user_queue.front();
		/* Take the thread off the ready queue */
		user_queue.pop();
		/* Run the task of the user-space thread */
		//
	}
	return;
}

void uthread_init(void)
{
	/* Use pthread_create() to create 4 kernel threads which will schedule
	any user-space thread to run */
	pthread_create(&thread_list[0], NULL, (void*(*)(void*)) handler, NULL);
	pthread_create(&thread_list[1], NULL, (void*(*)(void*)) handler, NULL);
	pthread_create(&thread_list[2], NULL, (void*(*)(void*)) handler, NULL);
	pthread_create(&thread_list[3], NULL, (void*(*)(void*)) handler, NULL);
}

void uthread_create(void (*func) (void*), void* arg)
{
	if (current_policy == UTHREAD_DIRECT_PTHREAD) {
		pthread_t new_id;
		user_queue.push(new_id);
		pthread_create(&user_queue.front(), NULL, (void*(*)(void*)) func, arg);
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
