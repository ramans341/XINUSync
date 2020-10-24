#include <xinu.h>

syscall al_initlock(al_lock_t* l){
    if (++al_lock_count > NSPINLOCKS) {
        return SYSERR;
    }
    l->flag = 0;
    l->guard = 0;
    l->lock_list = newqueue();
    
}

syscall al_lock(al_lock_t *l){
    while (test_and_set(&l->guard,1)==1);
    if (l->flag == 0){
        l->flag = 1;
        l->owner_pid = (pid32)getpid();
        P[currpid] = -1;
        l->guard = 0;
    }
    else {
        P[currpid] = l->owner_pid; 
        //kprintf(" %d hldng lk %d enqd\n",l->owner_pid,currpid);
        enqueue(currpid, l->lock_list);
        find_deadlock();
        al_setpark(currpid);
        l->guard = 0;
        al_park();
    }
    //shifted
}

syscall al_unlock(al_lock_t *l){
    pid32 next_pid;
    if (al_lock_count != 0 && (currpid == l->owner_pid)){
        while (test_and_set(&l->guard,1)==1);
        if (isempty(l->lock_list)){
            l->flag = 0;
        }
        else{
            next_pid = dequeue(l->lock_list);
            l->owner_pid = next_pid;
            P[next_pid] = -1;
            al_unpark(next_pid);
        }

        l->guard = 0;   
    }

    else{
        return SYSERR;
    }
}



void al_park() {
    intmask mask = disable();

    if (proctab[currpid].park == 1){
        proctab[currpid].prstate = PR_SUSP;	
        resched();
    }
   
    restore (mask);

}

void al_setpark(pid32 pid) {
    proctab[pid].park = 1;
}

void al_unpark(pid32 pid) {
    
    intmask mask = disable();
    proctab[pid].park = 0;
    proctab[pid].prstate = PR_READY;
    insert(pid, readylist, proctab[pid].prprio);
    restore(mask);

}