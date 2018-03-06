#ifndef _AFFINITY_H_
#define _AFFINITY_H_

//set cpu affinity for thread
#define _GNU_SOURCE
#include "sched.h"
#include "pthread.h"

#include "unistd.h"
#include "sys/types.h"
#include "sys/syscall.h"

#include "../include/common.h"

//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei         Date:        20180205
//Description: bind the current thread with an specified core  
//Parameters:  id_core is the ID of core(for example: 0,1,2...)  
//==========================================================================
void affinity_set(int id_core) {
    
    int       amount_core;
    cpu_set_t        mask;
    cpu_set_t         get;

    amount_core = sysconf(_SC_NPROCESSORS_CONF);
    printf("the system has %d cores or processors\n", amount_core);

    if(id_core > amount_core) {
    	printf("id_core is bigger than total amount of cores!");
        exit(0);
    }

    CPU_ZERO(&mask);

    CPU_SET(id_core, &mask);
    //set cpu affinity
    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) != 0) {
        fprintf(stderr, "set thread affinity failed\n");
    }
    //look up the affinity
    CPU_ZERO(&get);

    if (pthread_getaffinity_np(pthread_self(), sizeof(get), &get) < 0) {
        fprintf(stderr, "get thread affinity failed\n");
    }
    //thread running which core or thread
    for(int i = 0; i < amount_core; i++) {
        if (CPU_ISSET(i, &get)) {
//            printf("the ID of the thread is %d\n", syscall(SYS_gettid));
            printf("this thread %ld is running on processor %d\n",
                  syscall(SYS_gettid), i);
        }
	}
}
//==========================================================================
#endif
