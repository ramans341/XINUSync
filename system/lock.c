#include <xinu.h>

syscall initlock(lock_t* l){
    if (++lock_count > NSPINLOCKS) {
        return SYSERR;
    }
    l->flag = 0;
    l->guard = 0;
    l->lock_list = newqueue();
}

syscall lock(lock_t *l){
    while (test_and_set(&l->guard,1)==1);
    if (l->flag == 0){
        l->flag = 1;
        l->guard = 0;
    }
    else {
        enqueue(currpid, l->lock_list);
        setpark(currpid);
        l->guard = 0;
        park();
    }
}

syscall unlock(lock_t *l){
    if (spinlock_count != 0){
        while (test_and_set(&l->guard,1)==1);
        if (isempty(l->lock_list)){
            l->flag = 0;
        }
        else{
            unpark(dequeue(l->lock_list));
        }

        l->guard = 0;   
    }

    else{
        return SYSERR;
    }
}



void park() {
    intmask mask = disable();

    if (proctab[currpid].park == 1){
        proctab[currpid].prstate = PR_SUSP;	
        resched();
    }
   
    restore (mask);

}

void setpark(pid32 pid) {
    proctab[pid].park = 1;
}

void unpark(pid32 pid) {
    
    intmask mask = disable();
    proctab[pid].park = 0;
    proctab[pid].prstate = PR_READY;
    insert(pid, readylist, proctab[pid].prprio);
    restore(mask);

}