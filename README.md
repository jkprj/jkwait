# jkwait
这是一个运行时限制程序cpu使用率（CPU Limit）的代码封装，能够比较有效的限制程序CPU的使用率。

# 支持环境
windows，linux

# 使用方法
1、将 jkwait.h, jkwait.c 复制到自己的项目<br/>
2、调用 JK_WAIT_START 启动监控线程<br/>           （注：如果期望在deamon，fork等函数后的子进程中生效，必须在deamon，fork后在子进程调用JK_WAIT_START）<br/>
3、在适当位置插入JK_TRY_WAIT<br/>
