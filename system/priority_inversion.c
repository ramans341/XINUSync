# include <xinu.h>

void priority_boosting(){
    //kprintf("PBC %d\n",currpid);
    int32 i = 0;
    pri16 oldpri = 0;
    intmask mask;
    i = currpid;
    mask = disable();
    if (P[currpid] != -1) {
        if (proctab[i].prprio > proctab[P[i]].prprio){
            oldpri = proctab[P[i]].prprio;
            proctab[P[i]].prprio = proctab[i].prprio;
            kprintf("PRIORITY_CHANGE = P%s::%d-%d \n", proctab[P[currpid]].prname, oldpri, proctab[currpid].prprio);

            i = P[i];
            while (P[i] != -1){
                if (proctab[i].prprio > proctab[P[i]].prprio)
                {oldpri = proctab[P[i]].prprio;
                proctab[P[i]].prprio = proctab[i].prprio;
                kprintf("PRIORITY_CHANGE = P%s::%d-%d \n", proctab[P[i]].prname, oldpri, proctab[i].prprio);}
                i = P[i];
            }
        }
    }
    restore (mask);
}

