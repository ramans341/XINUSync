/*  main.c  - main */

#include <xinu.h>


process main(void){
kprintf("Test Case -1 Deadlock \n");

find_deadlock();

return OK;
}


