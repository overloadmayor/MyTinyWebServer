#include "httpconn.h"
#include "/home/realmayor/web_server/log/log.h"
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <errno.h>
#include <fcntl.h>
#include <mysql/mysql.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <unistd.h>

// 定义http响应的一些状态信息
const char *ok_200_titie = "ok";
const char *error_400_title = "Bad Request";
const char *error_400_form =
    "Your request has bad syntax or is inherently impossible to staisfy\n";
const char *error_403_title = "Forbidden";
const char *error_403_form =
    "You do not have permission to get file from this server\n";
const char *error_404_title = "Not Found";
const char *error_404_form = "The request file was not found on this server\n";
const char *error_500_title = "Internal Error";
const char *error_500_form =
    "There was an unusual problem serving the request file\n";

http_conn::http_conn(){};
http_conn::~http_conn(){};

sockaddr_in *http_conn::get_address() { return &m_address; }

locker m_lock;
std::map<std::string, std::string> users;

void http_conn::initmysql_result(connection_pool *connPool) {
  MYSQL *mysql = NULL;
  // 先从连接池中取一个连接
  connectionRAII mysqlcon(&mysql, connPool);

  // 在user表中检索username，passwd数据，浏览器端输入
  if (mysql_query(mysql, "SELECT username,passwd From user")) {
    LOG_ERROR("SELECT error:%d\n", mysql_error(mysql));
  }
  // 从表中检索完整的结果集
  MYSQL_RES *result = mysql_store_result(mysql);
  // 返回结果集中的列数
  int num_fields = mysql_num_fields(result);
  // 返回所有字段结构的数组
  MYSQL_FIELD *field = mysql_fetch_fields(result);
  // 从结果集中获取下一行，将对应的用户名和密码，存入map中
  while (MYSQL_ROW row = mysql_fetch_row(result)) {
    std::string temp1(row[0]);
    std::string temp2(row[1]);
    users[temp1] = temp2;
  }
}

// 对文件描述符设置非阻塞
int setnonblocking(int fd) {
  int oldflag = fcntl(fd, F_GETFL);
  int newflag = oldflag | O_NONBLOCK;
  fcntl(fd, F_SETFL, newflag);
  return oldflag;
}
// 内核事件表注册新事件
void addfd(int epollfd, int fd, bool one_shot, int TRIGMODE) {
  epoll_event event;
  event.data.fd = fd;

  if (1 == TRIGMODE)
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
  else
    event.events = EPOLLIN | EPOLLRDHUP;
  if (one_shot)
    event.events |= EPOLLONESHOT;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
  setnonblocking(fd);
}

// 从内核时间表删除描述符
void removefd(int epollfd, int fd) {
  epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, 0);
  close(fd);
}

// 将事件重置为EPOLLONESHOT
void modfd(int epollfd, int fd, int ev, int TRIGMODE) {
  epoll_event event;
  event.data.fd = fd;
  if (1 == TRIGMODE) {
    event.events = ev | EPOLLRDHUP | EPOLLET | EPOLLONESHOT;
  } else {
    event.events = ev | EPOLLRDHUP | EPOLLONESHOT;
  }
  epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}
int http_conn::m_user_count = 0;
int http_conn::m_epollfd = -1;

// 初始化连接
void http_conn::init(int sockfd, const sockaddr_in &addr, char *root,
                     int TRIGMode, int close_log, std::string user,
                     std::string passwd, std::string sqlname) {
  m_sockfd = sockfd;
  m_address = addr;
  addfd(m_epollfd, sockfd, true, m_TRIGMode);
  m_user_count++;

  // 当浏览器出现连接重置时，可能是网站根目录出错或者http相应格式出错或者访问的文件内容完全为空
  doc_root = root;
  m_TRIGMode = TRIGMode;
  m_close_log = close_log;

  strcpy(sql_user, user.c_str());
  strcpy(sql_passwd, passwd.c_str());
  strcpy(sql_name, sqlname.c_str());

  init();
}

void http_conn::init() {
  mysql = NULL;
  bytes_to_send = 0;
  bytes_have_send = 0;
  m_check_state = CHECK_STATE_REQUESTLINE;
  m_linger = false;
  m_method = GET;
  m_url = 0;
  m_version = 0;
  m_content_length = 0;
  m_host = 0;
  m_start_line = 0;
  m_checkd_idx = 0;
  m_read_idx = 0;
  m_write_idx = 0;
  cgi = 0;
  m_state = 0;
  time_flag = 0;
  improv = 0;

  memset(m_read_buf, '\0', READ_BUFFER_SIZE);
  memset(m_write_buf, '\0', WRITE_BUFFER_SIZE);
  memset(m_real_file, '\0', FILENAME_LEN);
}

// 关闭客户端连接
void http_conn::close_conn(bool real_close) {
  if (real_close && m_sockfd != -1) {
    printf("close %d\n", m_sockfd);
    removefd(m_epollfd, m_sockfd);
    m_sockfd = -1;
    m_user_count--;
  }
}

bool http_conn::read_once() {

  if (m_read_idx >= READ_BUFFER_SIZE) {
    return false;
  }
  int byte_read = 0;
  // ET模式
  if (1 == m_TRIGMode) {
    while (true) {
      byte_read = recv(m_sockfd, m_read_buf + m_read_idx,
                       READ_BUFFER_SIZE - m_read_idx, 0);
      if (byte_read == -1) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
          break;
        }
        return false;
      } else if (byte_read == 0) {
        return false;
      }
      m_read_idx += byte_read;
    }
  } else {
    // LT模式
    byte_read = recv(m_sockfd, m_read_buf + m_read_idx,
                     READ_BUFFER_SIZE - m_read_idx, 0);

    if (byte_read <= 0) {
      return false;
    }
    m_read_idx += byte_read;
  }
  return true;
}

char *http_conn::get_line() { return m_read_buf + m_start_line; }

// 从状态机，分析一行内容
http_conn::LINE_STATUS http_conn::parse_line() {
  for (; m_checkd_idx < m_read_idx; ++m_checkd_idx) {
    // putchar(m_read_buf[m_checkd_idx]);
    if (m_read_buf[m_checkd_idx] == '\r') {
      if (m_checkd_idx + 1 == m_read_idx) {
        return LINE_OPEN;
      } else if (m_read_buf[m_checkd_idx + 1] == '\n') {
        m_read_buf[m_checkd_idx++] = '\0';
        m_read_buf[m_checkd_idx++] = '\0';
        return LINE_OK;
      } else
        return LINE_BAD;
    } else if (m_read_buf[m_checkd_idx] == '\n') {
      if (m_checkd_idx > 0 && m_read_buf[m_checkd_idx] == '\r') {
        m_read_buf[m_checkd_idx - 1] = '\0';
        m_read_buf[m_checkd_idx++] = '\0';
        return LINE_OK;
      } else
        return LINE_BAD;
    }
  }
  return LINE_OPEN;
}
// 解析请求行，获得请求方法，url和版本号
http_conn::HTTP_CODE http_conn::parse_request_line(char *text) {
  m_url = strpbrk(text, " \t");
  if (!m_url) {
    return BAD_REQUEST;
  }
  *m_url++ = '\0';
  char *method = text;
  if (strcasecmp(method, "GET") == 0) {
    m_method = GET;
  } else if (strcasecmp(method, "POST") == 0) {
    m_method = POST;
    cgi = 1;
  } else {
    return BAD_REQUEST;
  }

  m_url += strspn(m_url, " \t");
  m_version = strpbrk(m_url, " \t");

  if (!m_version) {
    return BAD_REQUEST;
  }
  *m_version++ = '\0';
  m_version += strspn(m_version, " \t");
  if (strcasecmp(m_version, "HTTP/1.1") != 0) {
    return BAD_REQUEST;
  }
  if (strncasecmp(m_url, "http://", 7) == 0) {
    m_url += 7;
    m_url = strchr(m_url, '/');
  } else if (strncasecmp(m_url, "https://", 8) == 0) {
    m_url += 8;
    m_url = strchr(m_url, '/');
  }
  if (!m_url || m_url[0] != '/') {
    return BAD_REQUEST;
  }
  if (strlen(m_url) == 1) {
    strcat(m_url, "judge.html");
  }
  m_check_state = CHECK_STATE_HEADER;
  return NO_REQUEST;
}
// 解析请求头
http_conn::HTTP_CODE http_conn::parse_headers(char *text) {

  if (text[0] == '\0') {
    if (m_content_length != 0) {
      m_check_state = CHECK_STATE_CONTENT;
      return NO_REQUEST;

    } else
      return GET_REQUEST;
  } else if (strncasecmp(text, "Connection:", 11) == 0) {
    text += 11;
    text += strspn(text, " \t");

    if (strcasecmp(text, "keep-alive") == 0) {
      m_linger = true;
    }
  } else if (strncasecmp(text, "Content-length:", 15) == 0) {
    text += 15;
    text += strspn(text, " \t");
    m_content_length = atoi(text);
  } else if (strncasecmp(text, "Host:", 5) == 0) {
    text += 5;
    text += strspn(text, " \t");
    // printf("text:%s\n");
    m_host = text;
  } else {
    // LOG
    LOG_INFO("oop!unknow header:%s\n", text);
  }
  return NO_REQUEST;
}
// 解析请求体
http_conn::HTTP_CODE http_conn::parse_content(char *text) {
  if (m_read_idx >= (m_content_length + m_checkd_idx)) {
    text[m_content_length] = '\0';
    // POST请求中最后为输入的用户名和密码
    m_string = text;
    return GET_REQUEST;
  }
  return NO_REQUEST;
}

http_conn::HTTP_CODE http_conn::do_request() {
  strcpy(m_real_file, doc_root);
  int len = strlen(doc_root);
  const char *p = strrchr(m_url, '/');
  // 处理cgi
  if (cgi == 1 && (*(p + 1) == '2' || *(p + 1) == '3')) {
    // 判断登陆检测还是注册检测
    int flag = m_url[1];
    char *m_url_real = (char *)malloc(sizeof(char) * 200);
    strcpy(m_url_real, "/");
    strcat(m_url_real, m_url + 2);
    strncpy(m_real_file + len, m_url_real, FILENAME_LEN - len - 1);
    free(m_url_real);

    // 将用户名和密码提取出来
    // user=123&passwd=123
    char name[100], password[100];
    int i;
    for (i = 5; m_string[i] != '&'; i++) {
      name[i - 5] = m_string[i];
    }
    name[i - 5] = '\0';
    int j = 0;
    for (j = i + 10; m_string[j] != '\0'; j++) {
      password[j - 10 - i] = m_string[j];
    }
    password[j - 10 - i] = '\0';
    if (*(p + 1) == '3') {

      // 如果是注册，检查有没有重名的，如果没有重名则增加数据

      char *sql_insert = (char *)malloc(sizeof(char) * 200);
      // snprintf(sql_insert, 200,
      //          "INSERT INTO user(username,passwd) VALUES('%s','%s')", name,
      //          password);
      strcpy(sql_insert, "INSERT INTO user(username, passwd) VALUES(");
      strcat(sql_insert, "'");
      strcat(sql_insert, name);
      strcat(sql_insert, "', '");
      strcat(sql_insert, password);
      strcat(sql_insert, "')");

      if (users.find(name) == users.end()) {

        m_lock.lock();

        int res = mysql_query(mysql, sql_insert);

        users.insert(std::make_pair<std::string, std::string>(name, password));
        m_lock.unlock();

        if (!res) {
          strcpy(m_url, "/log.html");
        } else {
          strcpy(m_url, "/registerError.html");
        }
      } else {
        strcpy(m_url, "/registerError.html");
      }

    } else if (*(p + 1) == '2') {

      // 如果是登陆，直接判断，若浏览器端输入的用户名和密码在表中可以查找到，返回1，否则返回0；
      if (users.find(name) != users.end() && users[name] == password) {
        strcpy(m_url, "/welcome.html");
      } else {

        strcpy(m_url, "/logError.html");
      }
    }
  }
  if (*(p + 1) == '0') {

    char *m_url_real = (char *)malloc(sizeof(char *) * 200);
    strcpy(m_url_real, "/register.html");
    strncpy(m_real_file + len, m_url_real, strlen(m_url_real));
    free(m_url_real);
  } else if (*(p + 1) == '1') {
    char *m_url_real = (char *)malloc(sizeof(char) * 200);
    strcpy(m_url_real, "/log.html");
    strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

    free(m_url_real);
  } else if (*(p + 1) == '5') {
    char *m_url_real = (char *)malloc(sizeof(char) * 200);
    strcpy(m_url_real, "/picture.html");
    strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

    free(m_url_real);
  } else if (*(p + 1) == '6') {

    char *m_url_real = (char *)malloc(sizeof(char *) * 200);
    strcpy(m_url_real, "/video.html");
    strncpy(m_real_file + len, m_url_real, strlen(m_url_real));
    free(m_url_real);
  } else if (*(p + 1) == '7') {

    char *m_url_real = (char *)malloc(sizeof(char *) * 200);
    strcpy(m_url_real, "/fans.html");
    strncpy(m_real_file + len, m_url_real, strlen(m_url_real));
    free(m_url_real);
  } else {

    strncpy(m_real_file + len, m_url, FILENAME_LEN - len - 1);
  }
  // 是否有该资源存在
  if (stat(m_real_file, &m_file_stat) < 0)
    return NO_RESOURCE;

  if (!(m_file_stat.st_mode & S_IROTH)) {
    return FORBIDDEN_REQUEST;
  }
  if (S_ISDIR(m_file_stat.st_mode)) {
    return NO_REQUEST;
  }
  int fd = open(m_real_file, O_RDONLY);

  m_file_address =
      (char *)mmap(NULL, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  close(fd);
  return FILE_REQUEST;
}
void http_conn::unmap() {
  if (m_file_address) {
    munmap(m_file_address, m_file_stat.st_size);
    m_file_address = NULL;
  }
}

http_conn::HTTP_CODE http_conn::process_read() {
  LINE_STATUS line_status = LINE_OK;
  HTTP_CODE ret = NO_REQUEST;
  char *text = 0;
  while ((m_check_state == CHECK_STATE_CONTENT && line_status == LINE_OK) ||
         ((line_status = parse_line()) == LINE_OK)) {
    text = get_line();
    m_start_line = m_checkd_idx;
    switch (m_check_state) {
    case CHECK_STATE_REQUESTLINE: {
      ret = parse_request_line(text);
      if (ret == BAD_REQUEST) {
        return BAD_REQUEST;
      }
      break;
    }
    case CHECK_STATE_HEADER: {
      ret = parse_headers(text);
      if (ret == BAD_REQUEST)
        return BAD_REQUEST;
      else if (ret == GET_REQUEST) {
        return do_request();
      }
      break;
    }
    case CHECK_STATE_CONTENT: {
      ret = parse_content(text);

      if (ret == GET_REQUEST) {
        HTTP_CODE ans = do_request();
        return ans;
        break;
      }
      break;
    }
    default:
      return INTERNAL_ERROR;
    }
  }
  return NO_REQUEST;
}
bool http_conn::add_response(const char *format, ...) {
  if (m_write_idx >= WRITE_BUFFER_SIZE) {
    return false;
  }
  va_list arg_list;
  va_start(arg_list, format);
  int len = vsnprintf(m_write_buf + m_write_idx,
                      WRITE_BUFFER_SIZE - 1 - m_write_idx, format, arg_list);
  if (len >= (WRITE_BUFFER_SIZE - 1 - m_write_idx)) {
    va_end(arg_list);
    return false;
  }
  m_write_idx += len;
  va_end(arg_list);
  return true;
}

bool http_conn::write() {
  int temp = 0;
  if (bytes_to_send == 0) {
    modfd(m_epollfd, m_sockfd, EPOLLIN, m_TRIGMode);
    init();
    return false;
  }

  while (1) {
    temp = writev(m_sockfd, m_iv, m_iv_count);
    if (temp < 0) {
      if (errno == EAGAIN) {
        modfd(m_epollfd, m_sockfd, EPOLLOUT, m_TRIGMode);
        return true;
      }
      unmap();
      return false;
    }
    bytes_to_send -= temp;
    bytes_have_send += temp;
    if (bytes_have_send >= m_iv[0].iov_len) {
      m_iv[0].iov_len = 0;
      m_iv[1].iov_base = m_file_address + (bytes_have_send - m_write_idx);
      m_iv[1].iov_len = bytes_to_send;
    } else {
      m_iv[0].iov_base = m_write_buf + bytes_have_send;
      m_iv[0].iov_len = m_iv[0].iov_len - temp;
      // m_iv[0].iov_len = m_iv[0].iov_len - bytes_have_send;
    }
    if (bytes_to_send <= 0) {
      unmap();
      modfd(m_epollfd, m_sockfd, EPOLLIN, m_TRIGMode);
      if (m_linger) {
        init();
        return true;
      } else {
        return false;
      }
    }
  }
}

bool http_conn::add_status_line(int status, const char *title) {
  return add_response("%s %d %s\r\n", "HTTP/1.1", status, title);
}
bool http_conn::add_content_length(int content_len) {
  return add_response("Content-Length:%d\r\n", content_len);
}
bool http_conn::add_content_type() {
  return add_response("Content-Type:%s\r\n", "text/html");
}
bool http_conn::add_linger() {
  return add_response("Conntection:%s\r\n", m_linger ? "keep-alive" : "close");
}
bool http_conn::add_blank_line() { return add_response("\r\n"); }
bool http_conn::add_headers(int content_len) {
  return add_content_length(content_len) && add_linger() && add_blank_line();
}
bool http_conn::add_content(const char *content) {
  return add_response("%s", content);
}
bool http_conn::process_write(HTTP_CODE read_ret) {
  switch (read_ret) {
  case INTERNAL_ERROR: {
    add_status_line(500, error_500_title);
    add_headers(strlen(error_500_title));
    if (!add_content(error_500_title))
      return false;
    break;
  }
  case BAD_REQUEST: {
    add_status_line(400, error_400_title);
    add_headers(strlen(error_400_title));

    if (!add_content(error_400_title))
      return false;
    break;
  }
  case FORBIDDEN_REQUEST: {
    add_status_line(403, error_403_title);
    add_headers(strlen(error_403_title));

    if (!add_content(error_403_title))
      return false;
    break;
  }
  case FILE_REQUEST: {
    add_status_line(200, ok_200_titie);

    if (m_file_stat.st_size != 0) {
      add_headers(m_file_stat.st_size);
      m_iv[0].iov_base = m_write_buf;
      m_iv[0].iov_len = m_write_idx;
      m_iv[1].iov_base = m_file_address;
      m_iv[1].iov_len = m_file_stat.st_size;
      m_iv_count = 2;
      bytes_to_send = m_write_idx + m_file_stat.st_size;
      return true;
    } else {
      const char *ok_string = "<html><body></body></html>";
      add_headers(strlen(ok_string));
      if (!add_content(ok_string))
        return false;
      break;
    }
  }
  default:

    return false;
  }
  m_iv[0].iov_base = m_write_buf;
  m_iv[0].iov_len = m_write_idx;
  m_iv_count = 1;
  bytes_to_send = m_write_idx;
  return true;
}
void http_conn::process() {
  HTTP_CODE read_ret = process_read();
  if (read_ret == NO_REQUEST) {
    modfd(m_epollfd, m_sockfd, EPOLLIN, m_TRIGMode);
    return;
  }
  bool write_ret = process_write(read_ret);
  if (!write_ret) {
    close_conn();
    return;
  }
  modfd(m_epollfd, m_sockfd, EPOLLOUT, m_TRIGMode);
}