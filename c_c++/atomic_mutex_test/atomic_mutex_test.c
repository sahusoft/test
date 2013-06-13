#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define ATOMIC_SUPPORT 1
//#define MUTEX_SUPPORT  1

#define TEST_BUF_SIZE  1024
#define TEST_ROUND     10000
#define THREAD_NUM     3

typedef struct
{
	volatile int refs;
	volatile uint64_t w_cnts;
	volatile uint64_t r_cnts;
	uint8_t data[TEST_BUF_SIZE];
} test_buf_t;

typedef struct
{
	pthread_t t_id;
	int id;
	uint64_t err_cnt;
	double time_use;
} tasklet_t;

test_buf_t test_buf;
tasklet_t tasks[THREAD_NUM];

pthread_mutex_t test_buf_lock;

void *writer_func(void *arg)
{
	uint8_t value = 0;
	while(1) {
#ifdef ATOMIC_SUPPORT
		while(__sync_val_compare_and_swap(&(test_buf.refs), 0, 1) == 1)
			usleep(0);
#else
#ifdef MUTEX_SUPPORT
		pthread_mutex_lock(&test_buf_lock);
#else
#endif
#endif
		memset(&(test_buf.data), value, sizeof(test_buf.data));	
		value++;
		test_buf.w_cnts++;
#ifdef ATOMIC_SUPPORT
		//__sync_fetch_and_add(&(test_buf.w_cnts), 1);
		__sync_val_compare_and_swap(&(test_buf).refs, 1, 0);
#else
#ifdef MUTEX_SUPPORT
		pthread_mutex_unlock(&test_buf_lock);
#else
#endif
#endif
	}
	return NULL;
}

void *reader_func(void *arg)
{
	int i = 0;
	uint64_t test_round = TEST_ROUND;
	int id = 0;
	struct timeval start_t, end_t;

	id = *((int *)arg);

	gettimeofday(&start_t, NULL);
	while(test_round--) {
#ifdef ATOMIC_SUPPORT
		while(__sync_val_compare_and_swap(&(test_buf.refs), 0, 1) == 1)
			usleep(0);
#else
#ifdef MUTEX_SUPPORT
		pthread_mutex_lock(&test_buf_lock);
#else
#endif
#endif
		for(i = 0; i < TEST_BUF_SIZE - 1; i++){
			if(test_buf.data[i] != test_buf.data[i + 1]) {
				tasks[id].err_cnt++;
				break;
			}
		}
		test_buf.r_cnts++;
#ifdef ATOMIC_SUPPORT
		//__sync_fetch_and_add(&(test_buf.r_cnts), 1);
		__sync_val_compare_and_swap(&(test_buf).refs, 1, 0);
#else
#ifdef MUTEX_SUPPORT
		pthread_mutex_unlock(&test_buf_lock);
#else
#endif
#endif
	}
	gettimeofday(&end_t, NULL);
	tasks[id].time_use = end_t.tv_sec - start_t.tv_sec + (end_t.tv_usec - start_t.tv_usec) / 1000000.0;
	return NULL;
}

int main(int argc, const char *argv[])
{
	int i = 0;
	pthread_t writer_t_id;
	int loop_times = 0; 
	uint64_t err_cnt_total = 0;
	struct timeval start_t, end_t;
	double time_use_total = 0;

	memset(&test_buf, 0, sizeof(test_buf));
	memset(&tasks, 0, sizeof(tasks));

	pthread_mutex_init(&test_buf_lock, NULL);

	pthread_create(&writer_t_id, NULL, writer_func, NULL);

	gettimeofday(&start_t, NULL);
	while(1) {
		for(i=0; i < THREAD_NUM; i++){
			tasks[i].id = i;
			pthread_create(&(tasks[i].t_id), NULL, reader_func, &(tasks[i].id));
		}

		for(i=0; i < THREAD_NUM; i++){
			pthread_join(tasks[i].t_id, NULL);
		}
		gettimeofday(&end_t, NULL);
		time_use_total = end_t.tv_sec - start_t.tv_sec + (end_t.tv_usec - start_t.tv_usec) / 1000000.0;

		err_cnt_total = 0;
		printf("------------------------------------\r\n");
		printf("%3s | ", "ID");
		printf("%8s | ", "time_use");
		printf("%8s | ", "errors");
		printf("%7s\r\n", "percent");
		printf("------------------------------------\r\n");
		for(i = 0; i< THREAD_NUM; i++) {
			err_cnt_total += tasks[i].err_cnt;
			printf("%3d   ", tasks[i].id);	
			printf("%8.6f   ", tasks[i].time_use);
			printf("%8zu   ", tasks[i].err_cnt);
			printf("%7.6f\r\n", (tasks[i].err_cnt * 1.0) / (loop_times * TEST_ROUND));
		}
		printf("------------------------------------\r\n");
		printf("loop_times=%d err_cnt_total=%zu time_total=%f\r\n", loop_times, err_cnt_total, time_use_total);
		printf("w_cnts=%zu r_cnts=%zu\r\n", test_buf.w_cnts, test_buf.r_cnts);
		printf("w_dps=%fGb/s r_dps=%fGb/s\r\n", test_buf.w_cnts * TEST_BUF_SIZE * 8/ time_use_total / (1 << 30),
				test_buf.r_cnts * TEST_BUF_SIZE * 8/ time_use_total / (1 << 30));
		printf("------------------------------------\r\n");
		loop_times++;
	}
	return 0;
}
