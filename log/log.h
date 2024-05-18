#ifndef LOG_H
#define LOG_H
#include "block_queue.h"
#include <lock.h>
#include <stdio.h>
#include <string>
class Log {
public:
  static Log *get_instance();
  static void *flush_log_thread(void *args);
  // 参数有日志文件，日志缓冲区大小，最大行数，最大行数以及最长日志条队列
  bool init(const char *file_name, int close_log, int log_buf_size = 8192,
            int split_lines = 5000000, int max_queue_size = 0);
  void write_log(int level, const char *format, ...);
  void flush(void);

private:
  Log();
  ~Log();
  Log(const Log &other) = delete;
  const Log &operator=(const Log &other) = delete;
  void *async_write_log();

private:
  char dir_name[128]; // 路径名
  char log_name[128]; // log文件名
  int m_split_lines;  // 日志最大行数
  int m_log_buf_size; // 日志缓冲区大小
  long long m_count;  // 日志行数记录
  int m_today;        // 记录当前天数是哪一天
  FILE *m_fp;         // 打开log的文件指针
  char *m_buf;
  block_queue<std::string> *m_log_queue; // 日志阻塞队列
  bool m_is_async;                       // 是否是异步日志
  locker m_mutex;
  int m_close_log; // 关闭日志
};

#define LOG_DEBUG(format, ...)                                                 \
  if (0 == m_close_log) {                                                      \
    Log::get_instance()->write_log(0, format, ##__VA_ARGS__);                  \
    Log::get_instance()->flush();                                              \
  }

#define LOG_INFO(format, ...)                                                  \
  if (0 == m_close_log) {                                                      \
    Log::get_instance()->write_log(1, format, ##__VA_ARGS__);                  \
    Log::get_instance()->flush();                                              \
  }

#define LOG_WARN(format, ...)                                                  \
  if (0 == m_close_log) {                                                      \
    Log::get_instance()->write_log(2, format, ##__VA_ARGS__);                  \
    Log::get_instance()->flush();                                              \
  }

#define LOG_ERROR(format, ...)                                                 \
  if (0 == m_close_log) {                                                      \
    Log::get_instance()->write_log(3, format, ##__VA_ARGS__);                  \
    Log::get_instance()->flush();                                              \
  }
#endif