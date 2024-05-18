cmake_minimum_required(VERSION 3.28)

project(server VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/CGImysql ${CMAKE_SOURCE_DIR}/http 
${CMAKE_SOURCE_DIR}/lock ${CMAKE_SOURCE_DIR}/log  ${CMAKE_SOURCE_DIR}/threadpool
${CMAKE_SOURCE_DIR}/timer ${CMAKE_SOURCE_DIR}/webserver ${MYSQL_DIRECTORY})

file(GLOB SRC_FILES RELATIVE ${CMAKE_SOURCE_DIR} *.cpp )

message("${SRC_FILES}")

message("${SRC_DIR}")

foreach(dir ${SRC_DIR})
    file(GLOB FILES ${dir}/*.cpp)
    list(APPEND SRC_FILES ${FILES})
endforeach()

message("${SRC_FILES}")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SRC_FILES})

set(EXTRA_LIBS pthread mysqlclient)

add_library(mysqlclient SHARED IMPORTED)

set_target_properties(mysqlclient PROPERTIES
    IMPORTED_LOCATION /lib64/mysql/libmysqlclient.so
)

# # 在构建myExecutable之后执行自定义脚本
# add_custom_command(
#     TARGET ${PROJECT_NAME} 
#     POST_BUILD 
#     COMMAND ${CMAKE_SOURCE_DIR}/copymakefile.sh
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build
# )
list(APPEND SRC_DIR ${CMAKE_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${SRC_DIR}
)
