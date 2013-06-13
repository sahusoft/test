#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include "shm_sem_test.h"

int main(void)
{
	int sem_id, shm_id;
	key_t sem_key, shm_key;
	union sem_un sem_u;
	uint8_t val = 0;

	sem_key = ftok("server.c", 0);
	shm_key = ftok("client.c", 0);

	sem_id = semget(sem_key, 1, 0666|IPC_CREAT);
	if(sem_id == -1) {
		perror("semget");
		return -1;
	}

	shm_id = shmget(shm_key, SHM_SEM_TEST_SIZE, 0666|IPC_CREAT);
	if(shm_id == -1) {
		perror("shmget");
		return -2;
	}

	sem_u.val = 1;
	semctl(sem_id, 0, SETVAL, sem_u);

	uint8_t *shm_ptr = NULL;
	shm_ptr = (uint8_t *)shmat(shm_id, 0, 0);
	if(shm_ptr == (uint8_t *)-1) {
		printf("shm shmat failed\r\n");
		return -3;
	}

	while(1) {
		if(p(sem_id) != 0)
			continue;
		memset(shm_ptr, val, SHM_SEM_TEST_SIZE);
		v(sem_id);
		val++;
		//sleep(1);
	}

	if(shmdt(shm_ptr) == -1) {
		printf("shmdt failed\r\n");
		return -__LINE__;
	}
	if(semctl(sem_id, 0, IPC_RMID, 0) == -1) {
		printf("semctl delete error\r\n");
		return -__LINE__;
	}
	if(shmctl(shm_id, IPC_RMID, NULL) == -1){
		printf("shmctl delete error\r\n");
		return -__LINE__;
	}
	
	return 0;
}
