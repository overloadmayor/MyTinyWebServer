# TinyWebServer
这是一个Linux下的轻量级web服务器
* 实现了同步/异步日志系统
* 实现了用户注册和登陆功能
* 经Webbench压力测试可以实现上万的并发连接数据交换

#### 使用：
直接运行 cmake.sh即可

#### 不足：
经过测试在proactor模式下进行Webbench压力测试会有段错误，目前找不到原因