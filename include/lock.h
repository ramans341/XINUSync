#define NSPINLOCKS      20


typedef struct _sl_lock_t {
    int flag;
}sl_lock_t;

typedef struct _lock_t {
    int flag;
    int guard;
    qid16 lock_list;
}lock_t;

extern uint32 test_and_set(uint32 *, uint32);

extern int spinlock_count;
extern int lock_count;

extern void setpark(pid32);
extern void park();
extern void unpark(pid32);

extern syscall sl_initlock(sl_lock_t*);
extern syscall sl_lock(sl_lock_t*);
extern syscall sl_unlock(sl_lock_t*);

extern syscall initlock(lock_t*);
extern syscall lock(lock_t*);
extern syscall unlock(lock_t*);



