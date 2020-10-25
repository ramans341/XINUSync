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

    pri16 temp_prio;

    while (test_and_set(&l->guard,1)==1);
    
    temp_prio = proctab[currpid].prprio;
    proctab[currpid].prprio = 30000;

    if (l->flag == 0){
        //kprintf("%d acq lk\n", currpid);
        l->flag = 1;
        P[currpid] = -1;
        l->guard = 0;
        proctab[currpid].prprio = temp_prio;

    }
    else {
        if (isempty(l->lock_list)){
            P[currpid] = l->owner_pid;
        }

        else {
            P[currpid] = lastid(l->lock_list);
        }
 
        
        //kprintf("%d enqd\n", currpid);
        enqueue(currpid, l->lock_list);

        proctab[currpid].prprio = temp_prio;
        priority_boosting();
        temp_prio = proctab[currpid].prprio;
        proctab[currpid].prprio = 30000;
        pi_setpark(currpid);
        l->guard = 0;
        proctab[currpid].prprio = temp_prio;
        pi_park();
    }
    l->owner_pid = currpid;
}

syscall pi_unlock(pi_lock_t *l){
    pid32 next_pid;
    int32 o = 0;
    pri16 maxim,old,temp_prio = 0;

    if (pi_lock_count != 0 && (currpid == l->owner_pid)){
        //kprintf("Enterted Unlock \n");
        while (test_and_set(&l->guard,1)==1);

        temp_prio = proctab[currpid].prprio;
        proctab[currpid].prprio = 30000;
        
        if (isempty(l->lock_list)){
            //kprintf("%d released lk\n", currpid);
            l->flag = 0;
        }

        else{
            next_pid = dequeue(l->lock_list);
            //kprintf("%d released lk and deqd %d\n", currpid, next_pid);
            P[next_pid] = -1;
            l->owner_pid = next_pid;
            pi_unpark(next_pid);
        }
        l->guard = 0; 
         
        proctab[currpid].prprio = temp_prio;
        reduce_priority();
         
        
    }

    else{
        return SYSERR;
    }
}

void reduce_priority(){
    int o = 0;
    int maxim = 0;
    pri16 old = 0;
    intmask mask;
    mask = disable();
    for (o = 0; o <NPROC; o++){
            //kprintf("In FoR %d \n", o);
            if (P[o] == currpid && (proctab[o].prprio > maxim)){
                //kprintf("FL max is %d \n",maxim); 
                maxim = proctab[o].prprio;
                //break;
            }   
        }
        old = proctab[currpid].prprio;
        //kprintf("max is %d \n",maxim);
        if (maxim != 0){
            proctab[currpid].prprio = maxim;
        }
        else {
            proctab[currpid].prprio = proctab[currpid].oldprio;
        } 
    if (currpid != old) {
        kprintf("PRIORITY_CHANGE = P%d::%d-%d \n", currpid, old, proctab[currpid].prprio);}
    maxim = 0;
    restore(mask);
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