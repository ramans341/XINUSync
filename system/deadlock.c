#include <xinu.h>


void find_deadlock(){
    qid16 cycle_origin_list = newqueue();
	//qid16 detected_cycle_list = newqueue();
    kprintf("Deadlock called \n");
    int count = 0;
    int seen[NPROC];
    int i,j = 0;
    int temp;

    for (i = 0; i < NPROC; i++){
        seen[i] = 0;
    }

    for (i = 0; i < 15; i++){
        kprintf("%d ", P[i]);
    }

   
    for (i = 0; i < NPROC; i++){
        //kprintf("In for loop %d, %d\n",i, seen[i]);
        if (seen[i] != 1){
            j = i;

            while (P[j] != -1) {
                j = P[j];
                seen[j] = 1;
                if (P[j] == i){
                    //kprintf("enqueueing %d\n",P[j]);
                    enqueue(P[j],cycle_origin_list);
                    count++;
                    break;
                }
            }

            seen[i] = 1;
                
        }
    }
    //kprintf("%d \n",count); 
    while (count--){
        temp = dequeue(cycle_origin_list);
        kprintf ("%d ", temp);
    }
}