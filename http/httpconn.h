#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H
#include "sql_connection_pool.h"
#include <arpa/inet.h>
#include <map>
#include <string>
#include <sys/stat.h>
class http_conn {
public:
  // 读取文件名称大小
  static const int FILENAME_LEN = 200;
  // 读缓冲区大小
  static const int READ_BUFFER_SIZE = 2048;
  // 写缓冲区大小
  static const int WRITE_BUFFER_SIZE = 1024;
  // 报文请求方法
  enum METHOD {
    GET = 0,
    POST,
    HEAD,
    PUT,
    DELETE,
    TRACE,
    OPTIONS,
    CONNECT,
    PATH
  };
  // 主状态机状态
  enum CHECK_STATE {
    CHECK_STATE_REQUESTLINE = 0,
    CHECK_STATE_HEADER,
    CHECK_STATE_CONTENT
  };
  // 报文解析结果
  enum HTTP_CODE {
    NO_REQUEST,  // 请求不完整，需要继续读取请求报文数据
    GET_REQUEST, // 获得了完整的HTTP请求
    BAD_REQUEST, // HTTP请求报文有语法错误或请求资源为目录
    NO_RESOURCE, // 请求资源不存在
    FORBIDDEN_REQUEST, // 请求资源禁止访问，没有读取权限
    FILE_REQUEST,      // 请求资源可以正常访问
    INTERNAL_ERROR,    // 服务器内部错误
    CLOSED_CONNECTION
  };
  // 从状态机状态
  enum LINE_STATUS {
    LINE_OK = 0, // 完整读取一行
    LINE_BAD,    // 报文语法有误
    LINE_OPEN    // 读取的行不完整
  };

public:
  http_conn();
  ~http_conn();

public:
  // 初始化套接字地址
  void init(int sockfd, const sockaddr_in &addr, char *, int, int,
            std::string user, std::string passwd, std::string sqlname);
  // 关闭http连接
  void close_conn(bool real_close = true);
  // 读取浏览器发来的全部数据
  bool read_once();
  // 响应报文写入数据
  bool write();
  // 获取套接字地址
  sockaddr_in *get_address();
  // 同步线程初始化数据库读取表
  void initmysql_result(connection_pool *connPool);
  void process();
  int time_flag;
  int improv;

private:
  void init();
  // 读取m_read_buf,并处理请求报文
  HTTP_CODE process_read();
  // 向m_write_buf写入响应报文数据
  bool process_write(HTTP_CODE ret);
  // 主状态机解析报文中的请求行数据
  HTTP_CODE parse_request_line(char *text);
  // 主状态机解析报文中的请求头数据
  HTTP_CODE parse_headers(char *text);
  // 主状态机解析报文中的请求内容
  HTTP_CODE parse_content(char *text);
  // 生成响应报文
  HTTP_CODE do_request();
  // 未处理字符指针
  char *get_line();
  // 从状态机读取一行，分析是请求报文的哪一部分
  LINE_STATUS parse_line();
  void unmap();

  // 生成响应报文,生成八部分，以下由do_request调用
  bool add_response(const char *format, ...);
  bool add_content(const char *content);
  bool add_status_line(int status, const char *title);
  bool add_headers(int content_length);
  bool add_content_type();
  bool add_content_length(int content_len);
  bool add_linger();
  bool add_blank_line();

public:
  static int m_epollfd;
  static int m_user_count;
  MYSQL *mysql;
  int m_state; // 读为0，写为1

private:
  int m_sockfd;
  sockaddr_in m_address;
  // 读取的请求报文
  char m_read_buf[READ_BUFFER_SIZE];
  // m_read_buf下一次要读取的位置
  int m_read_idx;
  // m_read_buf读取的位置m_checked_idx
  int m_checkd_idx;
  // m_read_buf已经解析的字符个数
  int m_start_line;
  // 存储的响应报文数据
  char m_write_buf[WRITE_BUFFER_SIZE];
  // m_write_buf长度
  int m_write_idx;

  // 主状态机状态
  CHECK_STATE m_check_state;
  // 请求方法
  METHOD m_method;

  // 解析请求报文中的6个变量
  char m_real_file[FILENAME_LEN];
  char *m_url;
  char *m_version;
  char *m_host;
  int m_content_length;
  bool m_linger;

  // 读取服务器上的文件地址
  char *m_file_address;
  struct stat m_file_stat;
  // io向量机制iovec
  struct iovec m_iv[2];
  int m_iv_count;
  // 是否启用POST
  int cgi;
  // 存储请求头数据
  char *m_string;
  // 剩余发送字节数
  int bytes_to_send;
  // 已发送字节数
  int bytes_have_send;
  // 是否启用ET
  int m_TRIGMode;
  char *doc_root;

  std::map<std::string, std::string> m_users;
  int m_close_log;
  char sql_user[100];
  char sql_passwd[100];
  char sql_name[100];
};
#endif