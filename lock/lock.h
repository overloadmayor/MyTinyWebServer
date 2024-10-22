#ifndef LOCKER_H
#define LOCKER_H
#include <pthread.h>
#include <semaphore.h>
class sem {
public:
  sem(int num = 0);
  ~sem();
  bool wait();
  bool post();

private:
  sem_t m_sem;
};
class locker {
public:
  locker();
  ~locker();
  bool lock();
  bool unlock();
  pthread_mutex_t *get();

private:
  pthread_mutex_t m_mutex;
};
class cond {
public:
  cond();
  ~cond();
  bool wait(pthread_mutex_t *mutex);
  bool timewait(pthread_mutex_t *m_mutex, struct timespec abstime);
  bool signal();
  bool broadcast();

private:
  pthread_cond_t m_cond;
};
#endif
