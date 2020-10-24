#include <xinu.h>


void find_deadlock(){
    kprintf("chk\n");
    qid16 cycle_origin_list = newqueue();
	qid16 detected_cycle_list = newqueue();
    //kprintf("Deadlock called %d\n", currpid);
    int count = 0;
    int seen[NPROC];
    int i,j = 0;
    int32 origin,temp;

    for (i = 0; i < NPROC; i++){
        seen[i] = 0;
    }

    /*for (i = 0; i < 20; i++){
        kprintf("%d ", P[i]);
    }
    kprintf("\n");*/
   
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
    while (!isempty(cycle_origin_list)){
        kprintf ("Deadlock Detected %d \n",count);
        /*temp = origin = dequeue(cycle_origin_list);
        do {
           insert(temp,detected_cycle_list,-temp);
            temp = P[temp];
        }while(temp != origin);

        while(!isempty(detected_cycle_list)){
            temp = dequeue(detected_cycle_list);
            kprintf("P%d-",temp);
        }
        */
        kprintf("##%d \n",!isempty(cycle_origin_list));
        kprintf ("%d \n",dequeue(cycle_origin_list));
        kprintf ("\n");
    }
}