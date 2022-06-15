
#ifndef __COROUTINE_COMMON_20150806193305_1438860785_H__
#define __COROUTINE_COMMON_20150806193305_1438860785_H__

#include "External.h"

// 默认协程栈大小 和 初始化协程数量
#define STACK_INT       (1048576)
#define COROUTINE_INT   (16)

#define CO_DEAD         (0)     // 协程死亡状态
#define CO_READY        (1)     // 协程已经就绪
#define CO_RUNNING      (2)     // 协程正在运行
#define CO_SUSPEND      (3)     // 协程暂停等待

// comng_t - 协程管理器对象
typedef struct comng * comng_t;

// co_f - 协程运行函数体
// g        : 协程管理器对象
// arg      : 用户创建协程传入参数
typedef void(*co_f)(comng_t g, void * arg);


#endif//__COROUTINE_WIN32_20150806193305_1438860785_H__
