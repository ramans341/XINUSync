/*  main.c  - main */

#include <xinu.h>
int x = 0;
process p_lock(al_lock_t* l) {
	uint32 i, j;
	uint32 k = 1;
	for (i = 0; i < 1000; i++) {
		//kprintf("%d\n", x++);
		al_lock(l);
		for (j = 0;j < 1000000;j++) k *= 2;
		al_unlock(l);
	}
	//kprintf("Finished\n");
	return OK;
}

process p_lock2(al_lock_t* l1, al_lock_t* l2) {
	uint32 i, j;
	uint32 k = 1;
	for (i = 0; i < 10000; i++) {
		//kprintf("%d\n", x++);
		al_lock(l1);
		for (j = 0;j < 10000000;j++)k *= 2;
		al_lock(l2);
		for (j = 0;j < 10000000;j++)k *= 2;
		al_unlock(l2);
		for (j = 0;j < 10000000;j++)k *= 2;
		al_unlock(l1);
	}
	//kprintf("Finished\n");
	return OK;
}

process	main(void)
{

	al_lock_t mutex1, mutex2, mutex3, mutex4, mutex5, mutex6, mutex7, mutex8;
	pid32		pid1, pid2, pid3, pid4, pid5, pid6, pid7, pid8, pid9, pid10;
	int i;

	kprintf("\n\n=========== TEST 1: Deadlock Detection  ===================\n\n");

	al_initlock(&mutex1);
	al_initlock(&mutex2);
	al_initlock(&mutex3);
	al_initlock(&mutex4);
	al_initlock(&mutex5);
	al_initlock(&mutex6);
	al_initlock(&mutex7);
	al_initlock(&mutex8);
    kprintf("All process initialised \n");
	pid1 = create((void*)p_lock, INITSTK, 1, "nthreads", 1, &mutex1);
	pid2 = create((void*)p_lock, INITSTK, 1, "nthreads", 1, &mutex2);
	pid3 = create((void*)p_lock, INITSTK, 1, "nthreads", 1, &mutex8);
	pid4 = create((void*)p_lock, INITSTK, 1, "nthreads", 1, &mutex8);
	pid5 = create((void*)p_lock2, INITSTK, 1, "nthreads", 2, &mutex3, &mutex4);
	pid6 = create((void*)p_lock2, INITSTK, 1, "nthreads", 2, &mutex6, &mutex7);
	pid7 = create((void*)p_lock2, INITSTK, 1, "nthreads", 2, &mutex5, &mutex3);
	pid8 = create((void*)p_lock2, INITSTK, 1, "nthreads", 2, &mutex7, &mutex6);
	pid9 = create((void*)p_lock, INITSTK, 1, "nthreads", 1, &mutex8);
	pid10 = create((void*)p_lock2, INITSTK, 1, "nthreads", 2, &mutex4, &mutex5);
    kprintf("All process created\n");

	resume(pid5);
	sleepms(500);
    kprintf("Process 5 resumed\n");
	resume(pid7);
    kprintf("Process 7 resumed\n");
	resume(pid10);
    kprintf("Process 10 resumed\n");
	sleepms(500);
	/*resume(pid6);
	sleepms(500);
	resume(pid8);
	sleepms(500);
	resume(pid1);
	resume(pid2);
	sleepms(500);
	resume(pid3);
	sleepms(500);
	resume(pid4);
	resume(pid9);*/


	for (i = 0;i < 3;i++) {
		receive();
		kprintf("%d / %d processes finished\n", i + 1, 10);
	}

	return OK;
}