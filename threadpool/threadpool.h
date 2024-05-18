#ifndef THREADPOOL_H
#define THREADPOOL_H
#include "/home/realmayor/web_server/CGImysql/sql_connection_pool.h"
#include "/home/realmayor/web_server/lock/lock.h"
#include <exception>
#include <list>
#include <pthread.h>
template <class T> class threadpool {
public:
  threadpool(int actor_model, connection_pool *connPool, int thread_number,
             int max_request = 10000);
  ~threadpool();
  bool append(T *request, int state);
  bool append_p(T *request);

private:
  static void *worker(void *arg);
  void run();

private:
  int m_thread_number;         // 线程池中的线程数
  int m_max_requests;          // 请求队列中允许的最大请求数
  pthread_t *m_threads;        // 线程池数组
  std::list<T *> m_workqueue;  // 请求队列
  sem m_queuestat;             // 请求队列任务信号量
  locker m_queuelocker;        // 请求队列互斥锁
  int m_actor_model;           // 模型切换
  connection_pool *m_connPool; // 数据库
};
template <typename T>
threadpool<T>::threadpool(int actor_model, connection_pool *connPool,
                          int thread_number, int max_request)
    : m_thread_number(thread_number), m_max_requests(max_request),
      m_connPool(connPool), m_threads(NULL), m_actor_model(actor_model) {
  if (thread_number <= 0 || max_request <= 0) {
    throw std::exception();
  }
  m_threads = new pthread_t[m_thread_number];
  if (m_threads == NULL) {
    throw std::exception();
  }
  for (int i = 0; i < m_thread_number; i++) {
    if (pthread_create(m_threads + i, NULL, worker, this)) {
      throw std::exception();
    }
    if (pthread_detach(m_threads[i])) {
      throw std::exception();
    }
  }
}
template <typename T> threadpool<T>::~threadpool() {
  delete[] m_threads;
  m_threads = NULL;
}
template <typename T> bool threadpool<T>::append(T *request, int state) {
  m_queuelocker.lock();
  if (m_workqueue.size() >= m_max_requests) {
    m_queuelocker.unlock();
    return false;
  }
  request->m_state = state;
  m_workqueue.push_back(request);
  m_queuelocker.unlock();
  m_queuestat.post();
  return true;
}
template <typename T> bool threadpool<T>::append_p(T *request) {
  m_queuelocker.lock();
  if (m_workqueue.size() >= m_max_requests) {
    m_queuelocker.unlock();
    return false;
  }
  m_workqueue.push_back(request);
  m_queuelocker.unlock();
  m_queuestat.post();
  return true;
}
template <typename T> void *threadpool<T>::worker(void *arg) {
  threadpool *pool = (threadpool *)arg;
  pool->run();
  return pool;
}
template <typename T> void threadpool<T>::run() {
  while (true) {
    m_queuestat.wait();
    m_queuelocker.lock();
    if (m_workqueue.empty()) {
      m_queuelocker.unlock();
      continue;
    }
    T *request = m_workqueue.front();
    m_workqueue.pop_front();
    m_queuelocker.unlock();
    if (!request)
      continue;
    // reactor
    if (1 == m_actor_model) {
      if (0 == request->m_state) {
        if (request->read_once()) {
          request->improv = 1;
          connectionRAII mysqlcon(&request->mysql, m_connPool);
          request->process();

        } else {
          request->improv = 1;
          request->time_flag = 1;
        }
      } else {

        if (request->write()) {
          request->improv = 1;
        } else {
          request->improv = 1;
          request->time_flag = 1;
        }
      }
    } else {
      // proactor
      connectionRAII mysqlconn(&request->mysql, m_connPool);
      request->process();
    }
  }
}

#endif
