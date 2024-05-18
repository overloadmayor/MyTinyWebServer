g++ main.cpp config.cpp webserver/webserver.cpp http/httpconn.cpp timer/lst_timer.cpp \
    log/log.cpp lock/lock.cpp CGImysql/sql_connection_pool.cpp \
    -I/usr/include/mysql -L/lib64/mysql -lmysqlclient -lpthread -std=gnu++11 -o app
./app
