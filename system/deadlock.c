#include <xinu.h>

void find_deadlock(){
    kprintf("Deadlock called \n");
    int count = 0;
    int seen[13];
    int i,j = 0;

    for (i = 0; i < 13; i++){
        seen[i] = 0;
    }

    for (i = 0; i < 13; i++){
        kprintf("%d", P[i]);
    }

    kprintf("\n");
    kprintf("Seen initialised \n");
    for (i = 0; i < 13; i++){
        kprintf("In for loop %d, %d\n",i, seen[i]);
        if (seen[i] != 1){
            j = i;

            while (P[j] != -1) {
                //kprintf("In While \n");
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