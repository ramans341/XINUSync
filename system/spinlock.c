#include <xinu.h>

syscall sl_initlock(sl_lock_t *l){
    if (++spinlock_count > NSPINLOCKS) {
        return SYSERR;
    }
    l->flag = 0;
}

syscall sl_lock(sl_lock_t *l){
    while (test_and_set(&l->flag,1)==1);
}

syscall sl_unlock(sl_lock_t *l){
    if (spinlock_count != 0){
        l->flag = 0; 
    }

    else
    {
        return SYSERR;
    }
    


}