#define NSPINLOCKS      20

typedef struct _lock_t {
    int flag;
}sl_lock_t;

extern int spinlock_count;

extern syscall sl_initlock(sl_lock_t*);
extern syscall sl_lock(sl_lock_t*);
extern syscall sl_unlock(sl_lock_t*);

extern uint32 test_and_set(uint32 *, uint32);