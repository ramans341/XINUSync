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
process p2_lock(pi_lock_t *l,pi_lock_t *m ){
	uint32 i;
	for (i=0; i<5; i++){
		pi_lock(l);
		run_for_ms(10);
        pi_lock(m);
        run_for_ms(1000);
		pi_unlock(m);
        run_for_ms(10);	
        pi_unlock(l);
	}
	return OK;
}

process main(void) {
    pi_lock_t lock1,lock2;
    pid32 p1,p2,p3,p4;
	int i;
	
    pi_initlock(&lock1);
    pi_initlock(&lock2);
    p1 = create((void *)p_lock, INITSTK, 1,"nthreads", 1, &lock1);
    p2 = create((void *)p2_lock, INITSTK, 2,"nthreads", 2, &lock1,&lock2);
    p3 = create((void *)p_lock, INITSTK, 3,"nthreads", 1, &lock1);
    p4 = create((void *)p_lock, INITSTK, 4,"nthreads", 1, &lock1);
	kprintf("Created Processes \n");

	resume(p1);
	kprintf("Resumed p1 \n");
	sleepms(50);
	resume(p2);
	kprintf("Resumed p2 \n");
    sleepms(10);
	resume(p3);
	kprintf("Resumed p3 \n");
    sleepms(10);
	resume(p4);
	kprintf("Resumed p4 \n");

	for (i = 0; i < 4; i++){
		receive();
		kprintf("Process %d / %d received \n", i+1, 4);
	}
	
	
	return OK;

}