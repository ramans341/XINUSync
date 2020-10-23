#include <xinu.h>

void find_deadlock(){
    kprintf("Deadlock called \n");
    int count = 0;
    int seen[NPROC];
    int i,j = 0;

    for (i = 0; i < NPROC; i++){
        seen[i] = 0;
    }

    for (i = 0; i < NPROC; i++){
        if (seen[i] != 1){
            j = i;

            while (P[j] != -1) {

                j = P[j];
                seen[j] = 1;
                if (P[j] == i){
                    //cycles.push(i);
                    count++;
                    break;
                }
            }

            seen[i] = 1;
                
        }
    }
    kprintf("%d \n",count); 
}