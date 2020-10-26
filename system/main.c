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
    pi_lock_t lock1,lock2,lock3,lock4,lock5,lock6,lock8;
    pid32 p1,p2,p3,p4,p5,p6,p7,p8;
	int i;
	kprintf("========== DEADLOCK - TESTCASE ========== \n");
    pi_initlock(&lock1);
    pi_initlock(&lock2);
    pi_initlock(&lock3);
    pi_initlock(&lock4);
    pi_initlock(&lock5);
    pi_initlock(&lock6);
    pi_initlock(&lock8);
    p1 = create((void *)p2_lock, INITSTK, 10,"1", 2, &lock1,&lock2);
    p2 = create((void *)p2_lock, INITSTK, 10,"2", 2, &lock2,&lock3);
    p3 = create((void *)p2_lock, INITSTK, 10,"3", 2, &lock3,&lock1);
    p4 = create((void *)p2_lock, INITSTK, 10,"4", 2, &lock4,&lock5);
    p5 = create((void *)p2_lock, INITSTK, 10,"5", 2, &lock5,&lock4);
    p6 = create((void *)p_lock, INITSTK, 10,"6", 1, &lock6);
    p7 = create((void *)p_lock, INITSTK, 10,"7", 1, &lock6);
    p8 = create((void *)p_lock, INITSTK, 10,"8", 1, &lock8);

	kprintf("Created Processes \n");

	resume(p1);
	resume(p2);
	resume(p3);
	
    sleepms(50);

	resume(p4);
    resume(p5);

    sleepms(5);

    resume(p6);
    sleepms(50);
    resume(p7);

    sleepms(10);
    resume(p8);

	for (i = 0; i < 8; i++){
		receive();
        kprintf("Number of Processes Received %d \n",i);
        if (i==2){
            kprintf("TEST CASE PASSED with 5 process in Deadlock\n");
        }
	}

    
    
	

	
	return OK;

}