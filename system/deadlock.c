#include <xinu.h>

void find_deadlock(){
    kprintf("Deadlock called \n");
    int count = 0;
    int seen[13];
    int i,j = 0;
    pid32 temp;

    for (i = 0; i < 13; i++){
        seen[i] = 0;
    }

    for (i = 0; i < 13; i++){
        kprintf("%d", P[i]);
    }

   
    for (i = 0; i < 13; i++){
        kprintf("In for loop %d, %d\n",i, seen[i]);
        if (seen[i] != 1){
            j = i;

            while (P[j] != -1) {
                j = P[j];
                seen[j] = 1;
                if (P[j] == i){
                    enqueue(cycle_origin_list,P[j]);
                    count++;
                    break;
                }
            }

            seen[i] = 1;
                
        }
    }
    kprintf("%d \n",count); 
    while (!isempty(cycle_origin_list)){
        temp = dequeue(cycle_origin_list);
        kprintf ("%d", temp);
    }
}