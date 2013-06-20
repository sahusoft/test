#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TEST_ROUND  20000
#define THREAD_NUM  10

#define SYNC
#define LOCKLESS

#ifndef LOCKLESS
	pthread_mutex_t mutex_lock;
#endif

static volatile int count = 0;

void *test_func(void *arg)
{
	int i = 0;
	for(i = 0; i < TEST_ROUND; i++){
#ifdef SYNC
#ifdef LOCKLESS
		__sync_fetch_and_add(&count, 1);
#else
		pthread_mutex_lock(&mutex_lock);
		count++;
		pthread_mutex_unlock(&mutex_lock);
#endif
#else
		count++;
#endif
	}
	return NULL;
}

int main(int argc, const char *argv[])
{
	pthread_t thread_ids[THREAD_NUM];
	int i = 0;

#ifndef LOCKLESS
	pthread_mutex_init(&mutex_lock, NULL);
#endif
	for(i = 0; i < sizeof(thread_ids)/sizeof(pthread_t); i++){
		pthread_create(&thread_ids[i], NULL, test_func, NULL);
	}

	for(i = 0; i < sizeof(thread_ids)/sizeof(pthread_t); i++){
		pthread_join(thread_ids[i], NULL);
	}

	printf("count=%d\r\n", count);
	return 0;
}
