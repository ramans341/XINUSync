# include <xinu.h>

void priority_boosting(){
    kprintf("PBC \n");
    int32 i = 0;
    pri16 oldpri = 0;
    i = currpid;
    
    if (P[currpid] != -1) {
        if (proctab[i].prprio > proctab[P[i]].prprio){
            oldpri = proctab[currpid].prprio;
            proctab[P[i]].prprio = proctab[i].prprio;
            kprintf("PRIORITY_CHANGE = P%d::%d-%d \n", currpid, oldpri, proctab[currpid].prprio);

            i = P[i];
            while (P[i] != -1){
                proctab[P[i]].prprio = proctab[i].prprio;
                i = P[i];
            }
        }
    }
}

