#ifndef LST_TIMER
#define LST_TIMER
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
class util_timer;
// 客户端资源
struct client_data {
  // 客户端socket地址
  sockaddr_in address;
  // socket文件描述符
  int sockfd;
  // 定时器
  util_timer *time;
};
// 定时器类
class util_timer {
public:
  util_timer();

public:
  // 超时时间
  time_t expire;
  // 回调函数
  void (*cb_func)(client_data *);
  // 连接资源
  client_data *user_data;
  // 前向定时器
  util_timer *prev;
  // 后继定时器
  util_timer *next;
};

class sort_timer_lst {
public:
  sort_timer_lst();
  ~sort_timer_lst();

  // 添加定时器
  void add_timer(util_timer *timer);
  // 调整定时器
  void adjust_timer(util_timer *timer);
  // 删除定时器
  void del_timer(util_timer *timer);
  // 定时任务处理函数
  void tick();

private:
  // 添加定时器，调整链表内部结点
  void add_timer(util_timer *timer, util_timer *lst_head);

private:
  util_timer *head;
  util_timer *tail;
};

class Utils {
public:
  Utils() {}
  ~Utils() {}
  void init(int timeslot);

  // 对文件描述符设置非阻塞
  int setnonblocking(int fd);
  // 将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
  void addfd(int epollfd, int fd, bool one_shot, int TRIGMode);

  // 信号处理函数
  static void sig_handler(int sig);

  // 设置信号函数
  void addsig(int sig, void (*handler)(int), bool restart = true);

  // 定时处理函数，重新定时以不断触发SIGALRM信号
  void timer_handler();

  void show_error(int connfd, const char *info);

public:
  static int *u_pipefd;
  static int u_epollfd;
  sort_timer_lst m_timer_lst;
  int m_TIMESLOT;
};

void cb_func(client_data *user_data);
#endif