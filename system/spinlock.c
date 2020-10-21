#include <xinu.h>

syscall sl_initlock(sl_lock_t *l){
    if (++lock_count > NSPINLOCKS){
        return SYSERR;
    }
    l->flag = 0;
}

syscall sl_lock(sl_lock_t *l){
    while (test_and_set(&l->flag,0,1)==1);
}

syscall sl_unlock(sl_lock_t *l){
    l->flag = 0;

}