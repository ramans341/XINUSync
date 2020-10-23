/*  main.c  - main */

#include <xinu.h>


process main(void){
kprintf("Test Case -1 Deadlock \n");
int P[13] = {-1, -1, -1, 4,-1,10,8,5,6,-1,7,-1,-1};
find_deadlock();

return OK;
}


