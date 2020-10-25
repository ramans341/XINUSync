#include <xinu.h>

syscall pi_initlock(pi_lock_t* l){
    if (++pi_lock_count > NSPINLOCKS) {
        return SYSERR;
    }
    l->flag = 0;
    l->guard = 0;
    l->lock_list = newqueue();
    
}

syscall pi_lock(pi_lock_t *l){
    while (test_and_set(&l->guard,1)==1);
    if (l->flag == 0){
        kprintf("%d acq lk\n", currpid);
        l->flag = 1;
        P[currpid] = -1;
        l->guard = 0;
    }
    else {
        if (isempty(l->lock_list)){
            P[currpid] = l->owner_pid;
        }

        else {
            P[currpid] = lastid(l->lock_list);
        }
 
        
        kprintf("%d enqd\n", currpid);
        enqueue(currpid, l->lock_list);
        priority_boosting();
        pi_setpark(currpid);
        l->guard = 0;
        pi_park();
    }
    l->owner_pid = currpid;
}

syscall pi_unlock(pi_lock_t *l){
    pid32 next_pid;
    int32 i = 0;
    pri16 max,old = 0;

    if (pi_lock_count != 0 && (currpid == l->owner_pid)){

        for (i = 0; i <NPROC; i++){
            if (P[i] == currpid && (proctab[i].prprio > max)){ 
                max = proctab[P[i]].prprio;
            }   
        }
        old = proctab[currpid].prprio;
        if (max != 0){
            proctab[currpid].prprio = max;
        }
        else {
            proctab[currpid].prprio = proctab[currpid].oldprio;
        } 
        kprintf("PRIORITY_CHANGE = P%d::%d-%d \n", currpid, old, proctab[currpid].prprio); 

        while (test_and_set(&l->guard,1)==1);
        if (isempty(l->lock_list)){
            kprintf("%d released lk\n", currpid);
            l->flag = 0;
        }


        else{
            next_pid = dequeue(l->lock_list);
            kprintf("%d released lk and deqd %d\n", currpid, next_pid);
            P[next_pid] = -1;
            l->owner_pid = next_pid;
            pi_unpark(next_pid);
        }

        l->guard = 0;   

        
    }

    else{
        return SYSERR;
    }
}



void pi_park() {
    intmask mask = disable();

    if (proctab[currpid].park == 1){
        proctab[currpid].prstate = PR_SLEEP;	
        resched();
    }
   
    restore (mask);

}

void pi_setpark(pid32 pid) {
    proctab[pid].park = 1;
}

void pi_unpark(pid32 pid) {
    
    intmask mask = disable();
    proctab[pid].park = 0;
    proctab[pid].prstate = PR_READY;
    insert(pid, readylist, proctab[pid].prprio);
    restore(mask);

}