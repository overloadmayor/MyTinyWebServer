#include "lock.h"
#include <exception>
// sem类
sem::sem(int num) {
  if (sem_init(&m_sem, 0, num)) {
    throw std::exception();
  }
}
sem::~sem() {
  if (sem_destroy(&m_sem)) {
    throw std::exception();
  }
}
bool sem::wait() { return sem_wait(&m_sem) == 0; }
bool sem::post() { return sem_post(&m_sem) == 0; }

// locker类
locker::locker() {
  if (pthread_mutex_init(&m_mutex, NULL)) {
    throw std::exception();
  }
}
locker::~locker() {
  if (pthread_mutex_destroy(&m_mutex)) {
    throw std::exception();
  }
}
bool locker::lock() { return pthread_mutex_lock(&m_mutex) == 0; }
bool locker::unlock() { return pthread_mutex_unlock(&m_mutex) == 0; }
pthread_mutex_t *locker::get() { return &m_mutex; }

// cond类
cond::cond() {
  if (pthread_cond_init(&m_cond, NULL)) {
    throw std::exception();
  }
}
cond::~cond() {
  if (pthread_cond_destroy(&m_cond)) {
    throw std::exception();
  }
}
bool cond::wait(pthread_mutex_t *mutex) {
  return pthread_cond_wait(&m_cond, mutex) == 0;
}
bool cond::timewait(pthread_mutex_t *mutex, struct timespec abstime) {
  return pthread_cond_timedwait(&m_cond, mutex, &abstime) == 0;
}
bool cond::signal() { return pthread_cond_signal(&m_cond) == 0; }
bool cond::broadcast() { return pthread_cond_broadcast(&m_cond) == 0; }
