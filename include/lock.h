#define NSPINLOCKS      20

typedef struct _lock_t {
    int flag;
}sl_lock_t;

extern int lock_count;

extern syscall sl_initlock(sl_lock_t);
extern syscall sl_lock(sl_lock_t);
extern syscall sl_unlock(sl_lock_t);
