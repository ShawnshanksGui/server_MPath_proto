#include "semaphore.h"
#include "stdio.h"

//target memory will be shared among threads in one procedure if pshared = 0 
void Sem_init(sem_t *sem, int pshared, unsigned int value) 
{
	if (sem_init(sem, pshared, value)){
		perror("sem_init failed");
		exit(0);
	} 
}

void *P(sem_t *sem) 
{
	if (sem_wait(sem) < 0) {
		perror("P error");
		exit(0);
	}
}

void *V(sem_t *sem)
{
	if(sem_post(sem) < 0) {
		perror("V error");
		exit(0);
	}
}
