#ifndef __SHM_SEM_TEST_H__
#define __SHM_SEM_TEST_H__

#define SHM_SEM_TEST_SIZE   1024
#define SHM_SEM_TEST_ROUND  100000
#define SUPPORT_P_V

union sem_un {
	int val;
	struct semid_ds *buf;
	ushort *array;
};

extern int p(int sem_id);
extern int v(int sem_id);
#endif /*__SHM_SEM_TEST_H__*/
