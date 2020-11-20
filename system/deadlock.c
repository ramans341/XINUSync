#include <xinu.h>


void find_deadlock(){
    //kprintf("DLC %d\n", currpid);
    qid16 cycle_origin_list = newqueue();
	qid16 detected_cycle_list = newqueue();
    int count = 0;
    int seen[NPROC];
    int i,j,h,flag = 0;
    int32 origin,temp;

    for (i = 0; i < NPROC; i++){
        seen[i] = 0;
    }

   
   
    for (i = 0; i < NPROC; i++){
        //kprintf("In for loop %d, %d\n",i, seen[i]);
        kprintf("%d \n",i);
        if (seen[i] != 1){
            seen[i] == 1;
            j = i;

            while (P[j] != -1) {
                j = P[j];
                if (seen[j] == 1) {
                    enqueue(j,cycle_origin_list);
                    break;
                }
                seen[j] = 1;
                if(P[j]==-1)break;
                if (P[j] == i){
                    enqueue(i,cycle_origin_list);
                    kprintf("Enqueued %d \n",i);
                    count++;
                    break;
                }
            }

            seen[i] = 1;
                
        }
    }
    kprintf("FOR LOOP OVER \n");
    /* for (i = 0; i < 50; i++) {
        kprintf("seen \n");
        kprintf("%d ", seen[i]);
    } */
    
    while (!isempty(cycle_origin_list)){
        temp = origin = dequeue(cycle_origin_list);
        //kprintf("origin %d \n",temp);
        if(origin==-1)break;
        if (printed[origin] == 0) {
            printed[origin] = 1;
            do {
            insert(temp,detected_cycle_list,-temp);
                temp = P[temp];
            }while(temp != origin);

            while(!isempty(detected_cycle_list)){
                temp = dequeue(detected_cycle_list);
                //kprintf("process %d in deadlock with process %d \n",temp,origin);
                if (flag == 0){
                    kprintf("Deadlock Detected- ");
                    flag = 1;
                }
                if (flag == 1){
                kprintf("P%d-",temp);
                }
            }  
            flag = 0;             
            kprintf("\n");
        }
        
    }
}