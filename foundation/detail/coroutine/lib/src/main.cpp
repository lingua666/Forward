
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>

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

// 声明 co - 协程结构 和 comng - 协程管理器结构
struct co {
	co_f func;              // 协程执行行为
	void * arg;             // 协程执行参数
	int status;             // 当前协程状态 CO_*
	void * ctx;             // 当前协程运行的上下文环境
};

struct comng {
	struct co ** cs;        // 协程对象集, 循环队列
	int cap;                // 协程对象集容量
	int idx;                // 当前协程集中轮询到的索引
	int len;                // 当前协程集存在的协程个数

	int running;            // 当前协程集中运行的协程 id
	void * ctx;             // 当前主协程记录运行上下文环境
};

// comng_run - 协程管理器运行实体
static void __stdcall comng_run(void* g) {
	int id = ((comng*)g)->running;
	struct co * c = ((comng*)g)->cs[id];

	// 执行协程体
	c->func(((comng*)g), c->arg);
	c->status = CO_DEAD;

	// 跳转到主纤程体中销毁
	SwitchToFiber(((comng*)g)->ctx);
}


int main(int argc, char* argv[])
{
	comng_t g;

	CreateFiberEx(STACK_INT, 0, FIBER_FLAG_FLOAT_SWITCH, comng_run, g);

	return 0;
}
