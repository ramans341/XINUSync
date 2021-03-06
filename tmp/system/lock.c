#include <xinu.h>

syscall initlock(lock_t* l){
    if (++lock_count > NLOCKS) {
        return SYSERR;
    }
    l->flag = 0;
    l->guard = 0;
    l->lock_list = newqueue();
    
}

syscall lock(lock_t *l){
    pri16 temp_prio;
    while (test_and_set(&l->guard,1)==1);
    temp_prio = proctab[currpid].prprio;
    proctab[currpid].prprio = 1000;

    if (l->flag == 0){
        l->flag = 1;
        l->guard = 0;
        proctab[currpid].prprio = temp_prio;
    }
    else {
        enqueue(currpid, l->lock_list);
        setpark(currpid);
        l->guard = 0;
        proctab[currpid].prprio = temp_prio;
        park();
    }
    l->owner_pid = (pid32)getpid();
}

syscall unlock(lock_t *l){
    pid32 next_pid;
    pri16 temp_prio = 0;

    if (lock_count != 0 && (currpid == l->owner_pid)){
        while (test_and_set(&l->guard,1)==1);
        temp_prio = proctab[currpid].prprio;
        proctab[currpid].prprio = 1000;
        if (isempty(l->lock_list)){
            l->flag = 0;
        }
        else{
            next_pid = dequeue(l->lock_list);
            l->owner_pid = next_pid;
            unpark(next_pid);
            proctab[currpid].prprio = temp_prio;
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