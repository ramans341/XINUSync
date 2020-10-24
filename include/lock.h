#define NSPINLOCKS      20


extern  int P[];
extern  int printed[];
typedef struct _sl_lock_t {
    int flag;
}sl_lock_t;

typedef struct _lock_t {
    int flag;
    int guard;
    qid16 lock_list;
    pid32 owner_pid;
}lock_t;

typedef struct _al_lock_t {
    int flag;
    int guard;
    qid16 lock_list;
    pid32 owner_pid;
}al_lock_t;

extern uint32 test_and_set(uint32 *, uint32);

extern int spinlock_count;
extern int lock_count;
extern int al_lock_count;

extern void setpark(pid32);
extern void park();
extern void unpark(pid32);

extern syscall sl_initlock(sl_lock_t*);
extern syscall sl_lock(sl_lock_t*);
extern syscall sl_unlock(sl_lock_t*);

extern syscall initlock(lock_t*);
extern syscall lock(lock_t*);
extern syscall unlock(lock_t*);

extern syscall al_initlock(al_lock_t*);
extern syscall al_lock(al_lock_t*);
extern syscall al_unlock(al_lock_t*);



