#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include "shm_sem_test.h"
int p(int sem_id)
{
#ifdef SUPPORT_P_V
	struct sembuf sem_p;
	memset(&sem_p, 0, sizeof(struct sembuf));
	sem_p.sem_num = 0;
	sem_p.sem_op = -1;
	if(semop(sem_id, &sem_p, 1) == -1) {
		perror("semop P");
		return -1;
	}
#endif
	return 0;
}

int v(int sem_id)
{
#ifdef SUPPORT_P_V
	struct sembuf sem_v;
	memset(&sem_v, 0, sizeof(struct sembuf));
	sem_v.sem_num = 0;
	sem_v.sem_op = 1;
	if(semop(sem_id, &sem_v, 1) == -1) {
		perror("semop P");
		return -1;
	}
#endif
	return 0;
}
