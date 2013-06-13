#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include "shm_sem_test.h"

int main(void)
{
	int sem_id, shm_id;
	key_t sem_key, shm_key;
	int i = 0;
	uint64_t err_cnt = 0;
	uint64_t test_round = SHM_SEM_TEST_ROUND;

	sem_key = ftok("server.c", 0);
	shm_key = ftok("client.c", 0);

	sem_id = semget(sem_key, 0, 0666);
	if(sem_id == -1) {
		printf("get sem failed\r\n");
		return __LINE__;
	}

	shm_id = shmget(shm_key, 0, 0666);
	if(shm_id == -1) {
		printf("get shm failed\r\n");
		return __LINE__;
	}

	uint8_t *shm_ptr = NULL;
	shm_ptr = (uint8_t *)shmat(shm_id, 0, 0);
	if(shm_ptr == (uint8_t *)-1) {
		printf("shm shmat failed\r\n");
		return __LINE__;
	}

	struct timeval tv_start, tv_end;
	double time_use;
	gettimeofday(&tv_start,NULL);
	while(test_round) {
		if(p(sem_id) !=0)
			continue;
		for(i = 0; i < SHM_SEM_TEST_SIZE - 1; i++) {
			if(shm_ptr[i] != shm_ptr[i + 1]) {
				err_cnt++;
				break;
			}
		}
		v(sem_id);
		test_round--;
		//sleep(1);
	}

	gettimeofday(&tv_end,NULL);
	time_use = tv_end.tv_sec - tv_start.tv_sec + (tv_end.tv_usec - tv_start.tv_usec)/1000000.0;

	printf("err_cnt = %ju\tpercent=%f\r\n",err_cnt, err_cnt*1.0/SHM_SEM_TEST_ROUND);
	printf("time=%f\ttime/round=%fus\r\n", time_use, time_use*1000*1000/SHM_SEM_TEST_ROUND);

	if(shmdt(shm_ptr) == -1) {
		printf("shmdt failed\r\n");
		return -__LINE__;
	}

	return 0;
}
