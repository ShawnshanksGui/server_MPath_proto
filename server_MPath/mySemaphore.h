
//if pshared = 1, specified memory will be share among threads in one procedure
void Sem_init(sem_t *sem, int pshared, unsigned int value);
void P(sem_t *sem);
void V(sem_t *sem);