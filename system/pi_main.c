/*  main.c  - main */

#include <xinu.h>

uint32 get_timestamp(){
	return ctr1000;
}

void run_for_ms(uint32 time){
	uint32 start = proctab[currpid].runtime;
	while (proctab[currpid].runtime-start < time);
}

process p_lock(pi_lock_t *l){
	uint32 i;
	for (i=0; i<5; i++){
		pi_lock(l);
		run_for_ms(1000);
		pi_unlock(l);		
	}
	return OK;
}

process main(void) {
    pi_lock_t lock1;
    pid32 p1,p2;

    p1 = create((void *)p_lock, INITSTK, 1,"nthreads", 1, &lock1);
    p2 = create((void *)p_lock, INITSTK, 2,"nthreads", 1, &lock1);

	return OK;

}