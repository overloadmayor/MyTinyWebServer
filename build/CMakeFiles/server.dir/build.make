# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/cmake-3.28.3-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /opt/cmake-3.28.3-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/realmayor/web_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/realmayor/web_server/build

# Include any dependencies generated for this target.
include CMakeFiles/server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.dir/flags.make

CMakeFiles/server.dir/config.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/config.cpp.o: /home/realmayor/web_server/config.cpp
CMakeFiles/server.dir/config.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/realmayor/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/server.dir/config.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/config.cpp.o -MF CMakeFiles/server.dir/config.cpp.o.d -o CMakeFiles/server.dir/config.cpp.o -c /home/realmayor/web_server/config.cpp

CMakeFiles/server.dir/config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/server.dir/config.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/realmayor/web_server/config.cpp > CMakeFiles/server.dir/config.cpp.i

CMakeFiles/server.dir/config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/server.dir/config.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/realmayor/web_server/config.cpp -o CMakeFiles/server.dir/config.cpp.s

CMakeFiles/server.dir/main.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/main.cpp.o: /home/realmayor/web_server/main.cpp
CMakeFiles/server.dir/main.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/realmayor/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/server.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/main.cpp.o -MF CMakeFiles/server.dir/main.cpp.o.d -o CMakeFiles/server.dir/main.cpp.o -c /home/realmayor/web_server/main.cpp

CMakeFiles/server.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/server.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/realmayor/web_server/main.cpp > CMakeFiles/server.dir/main.cpp.i

CMakeFiles/server.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/server.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/realmayor/web_server/main.cpp -o CMakeFiles/server.dir/main.cpp.s

CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o: /home/realmayor/web_server/CGImysql/sql_connection_pool.cpp
CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/realmayor/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o -MF CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o.d -o CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o -c /home/realmayor/web_server/CGImysql/sql_connection_pool.cpp

CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/realmayor/web_server/CGImysql/sql_connection_pool.cpp > CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.i

CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/realmayor/web_server/CGImysql/sql_connection_pool.cpp -o CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.s

CMakeFiles/server.dir/http/httpconn.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/http/httpconn.cpp.o: /home/realmayor/web_server/http/httpconn.cpp
CMakeFiles/server.dir/http/httpconn.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/realmayor/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/server.dir/http/httpconn.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/http/httpconn.cpp.o -MF CMakeFiles/server.dir/http/httpconn.cpp.o.d -o CMakeFiles/server.dir/http/httpconn.cpp.o -c /home/realmayor/web_server/http/httpconn.cpp

CMakeFiles/server.dir/http/httpconn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/server.dir/http/httpconn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/realmayor/web_server/http/httpconn.cpp > CMakeFiles/server.dir/http/httpconn.cpp.i

CMakeFiles/server.dir/http/httpconn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/server.dir/http/httpconn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/realmayor/web_server/http/httpconn.cpp -o CMakeFiles/server.dir/http/httpconn.cpp.s

CMakeFiles/server.dir/lock/lock.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/lock/lock.cpp.o: /home/realmayor/web_server/lock/lock.cpp
CMakeFiles/server.dir/lock/lock.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/realmayor/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/server.dir/lock/lock.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/lock/lock.cpp.o -MF CMakeFiles/server.dir/lock/lock.cpp.o.d -o CMakeFiles/server.dir/lock/lock.cpp.o -c /home/realmayor/web_server/lock/lock.cpp

CMakeFiles/server.dir/lock/lock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/server.dir/lock/lock.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/realmayor/web_server/lock/lock.cpp > CMakeFiles/server.dir/lock/lock.cpp.i

CMakeFiles/server.dir/lock/lock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/server.dir/lock/lock.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/realmayor/web_server/lock/lock.cpp -o CMakeFiles/server.dir/lock/lock.cpp.s

CMakeFiles/server.dir/log/log.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/log/log.cpp.o: /home/realmayor/web_server/log/log.cpp
CMakeFiles/server.dir/log/log.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/realmayor/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/server.dir/log/log.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/log/log.cpp.o -MF CMakeFiles/server.dir/log/log.cpp.o.d -o CMakeFiles/server.dir/log/log.cpp.o -c /home/realmayor/web_server/log/log.cpp

CMakeFiles/server.dir/log/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/server.dir/log/log.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/realmayor/web_server/log/log.cpp > CMakeFiles/server.dir/log/log.cpp.i

CMakeFiles/server.dir/log/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/server.dir/log/log.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/realmayor/web_server/log/log.cpp -o CMakeFiles/server.dir/log/log.cpp.s

CMakeFiles/server.dir/timer/lst_timer.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/timer/lst_timer.cpp.o: /home/realmayor/web_server/timer/lst_timer.cpp
CMakeFiles/server.dir/timer/lst_timer.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/realmayor/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/server.dir/timer/lst_timer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/timer/lst_timer.cpp.o -MF CMakeFiles/server.dir/timer/lst_timer.cpp.o.d -o CMakeFiles/server.dir/timer/lst_timer.cpp.o -c /home/realmayor/web_server/timer/lst_timer.cpp

CMakeFiles/server.dir/timer/lst_timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/server.dir/timer/lst_timer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/realmayor/web_server/timer/lst_timer.cpp > CMakeFiles/server.dir/timer/lst_timer.cpp.i

CMakeFiles/server.dir/timer/lst_timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/server.dir/timer/lst_timer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/realmayor/web_server/timer/lst_timer.cpp -o CMakeFiles/server.dir/timer/lst_timer.cpp.s

CMakeFiles/server.dir/webserver/webserver.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/webserver/webserver.cpp.o: /home/realmayor/web_server/webserver/webserver.cpp
CMakeFiles/server.dir/webserver/webserver.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/realmayor/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/server.dir/webserver/webserver.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/webserver/webserver.cpp.o -MF CMakeFiles/server.dir/webserver/webserver.cpp.o.d -o CMakeFiles/server.dir/webserver/webserver.cpp.o -c /home/realmayor/web_server/webserver/webserver.cpp

CMakeFiles/server.dir/webserver/webserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/server.dir/webserver/webserver.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/realmayor/web_server/webserver/webserver.cpp > CMakeFiles/server.dir/webserver/webserver.cpp.i

CMakeFiles/server.dir/webserver/webserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/server.dir/webserver/webserver.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/realmayor/web_server/webserver/webserver.cpp -o CMakeFiles/server.dir/webserver/webserver.cpp.s

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/config.cpp.o" \
"CMakeFiles/server.dir/main.cpp.o" \
"CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o" \
"CMakeFiles/server.dir/http/httpconn.cpp.o" \
"CMakeFiles/server.dir/lock/lock.cpp.o" \
"CMakeFiles/server.dir/log/log.cpp.o" \
"CMakeFiles/server.dir/timer/lst_timer.cpp.o" \
"CMakeFiles/server.dir/webserver/webserver.cpp.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

/home/realmayor/web_server/server: CMakeFiles/server.dir/config.cpp.o
/home/realmayor/web_server/server: CMakeFiles/server.dir/main.cpp.o
/home/realmayor/web_server/server: CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o
/home/realmayor/web_server/server: CMakeFiles/server.dir/http/httpconn.cpp.o
/home/realmayor/web_server/server: CMakeFiles/server.dir/lock/lock.cpp.o
/home/realmayor/web_server/server: CMakeFiles/server.dir/log/log.cpp.o
/home/realmayor/web_server/server: CMakeFiles/server.dir/timer/lst_timer.cpp.o
/home/realmayor/web_server/server: CMakeFiles/server.dir/webserver/webserver.cpp.o
/home/realmayor/web_server/server: CMakeFiles/server.dir/build.make
/home/realmayor/web_server/server: /lib64/mysql/libmysqlclient.so
/home/realmayor/web_server/server: CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/realmayor/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable /home/realmayor/web_server/server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.dir/build: /home/realmayor/web_server/server
.PHONY : CMakeFiles/server.dir/build

CMakeFiles/server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.dir/clean

CMakeFiles/server.dir/depend:
	cd /home/realmayor/web_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/realmayor/web_server /home/realmayor/web_server /home/realmayor/web_server/build /home/realmayor/web_server/build /home/realmayor/web_server/build/CMakeFiles/server.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/server.dir/depend

