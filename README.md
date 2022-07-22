# jkwait
运行时限制cpu使用率

# 背景
公司的扫描客户端为了不影响客户的机器业务，需要限制cpu使用率，因此就开发了这个小程序，实测可以很好的控制好CPU使用率——让系统觉得你的程序该暂停，而不要你以为该暂停

# 支持环境
windows，linux

# 使用方法
1、将 jkwait.h, jkwait.c 负责到自己的项目
2、调用 JK_WAIT_START 启动监控线程（注：如果有调用deamon让程序后台运行的，必须在deamon后调用JK_WAIT_START；不支持多进程）
3、在适当位置插入JK_TRY_WAIT
