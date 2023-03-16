#ifndef __U_WAIT_H
#define __U_WAIT_H

#define JK_WAIT_START            start();
#define JK_SET_LIMIT(rate)       set_limit(rate);
#define JK_TRY_WAIT              try_wait();
#define JK_WAIT_STATUS           wait_status();
#define JK_STOP                  stop();

enum {
	UNRUN,
	RUNNING
};

int start();

/* 尝试是否需要等待，使用的越密集限制CPU效果越好（当然，如果不觉得太密集会不好看的话）
*  代码中插入try_wait时机：
*  1、在循环中
*  2、调用系统函数可能的不确定耗时：如读写文件，收发数据等等
*  3、调用调用第三方库函数可能的不确定耗时：如 正则匹配，子字符串查找, json, protobuf, xml等等数据解析及构造。。。。。等等
*  4、较长的函数进入和离开都可以加下
*
*  插入的原则就是：该加的地方加，不需要加的地方也可以加，不知该不该加的地方加
*/
void try_wait();

// 设置单核cpu使用率上限，默认单核使用率上限10%
int set_limit(int rate);

// 获取uwait检查运行状态，UNRUN 异常，RUNNING 正常
int wait_status();

void stop();

#endif

