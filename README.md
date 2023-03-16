# jkwait
这是一个运行时限制程序cpu使用率（CPU Limit）的代码封装，能够比较有效的限制程序CPU的使用率，而且使用起来非常简单。<br/>如果程序在用户的机器上运行，可能需要做一些CPU密集型活动（比如安全工具的各种扫描，一些长时间计算等等）但是又不希望对用户的机器造成过多的影响，使用jkwait就可以较好的控制程序对CPU的使用。

# 支持环境
windows，linux

# 使用方法
1、将 jkwait.h, jkwait.c 复制到自己的项目<br/>
2、调用 JK_WAIT_START 启动监控线程<br/>           （注：如果期望在deamon，fork等函数后的子进程中生效，必须在deamon，fork后在子进程调用JK_WAIT_START）<br/>
3、在适当位置插入JK_TRY_WAIT<br/>

# API说明

## JK_WAIT_START

启动CPU监控线程，要使用jkwait首先第一步就是要调用该函数启动CPU监控线程。如果程序在运行过程中调用deamon，fork等函数创建了子进程，并且期望子进程中也能够控制CPU，那么必须在deamon，fork后在子进程调用JK_WAIT_START

## JK_SET_LIMIT(rate)

设置CPU使用的最大使用量，默认是10。<br/>PS：CPU最大值：单核 100， 双核：200，四核：400，八核：800.。。。。。。依此类推

## JK_TRY_WAIT

调用该函数检查线程是否需要等待，如果程序CPU已经超过设定上限，那么线程就会暂停等待直到CPU用量降回到上限以下

## JK_WAIT_STATUS

获取jkwait运行状态，1 代表正常运行，0 代表未运行或运行异常

## JK_STOP

停止jkwait CPU监控
