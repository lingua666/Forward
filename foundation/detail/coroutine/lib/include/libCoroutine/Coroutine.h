
#ifndef __COROUTINE_20150806193305_1438860785_H__
#define __COROUTINE_20150806193305_1438860785_H__

#include <libCoroutine/Common.h>

#if defined(PLATFORM_OS_FAMILY_WINDOWS)

namespace _coroutine_ {

	struct FiberScopedGuard
	{
		FiberScopedGuard::FiberScopedGuard()
		{
			// 在当前线程环境中开启 winds 协程
			GetTlsContext() = ConvertThreadToFiberEx(NULL, FIBER_FLAG_FLOAT_SWITCH);

			//GetTlsContext() = ConvertThreadToFiber(nullptr);
		}

		FiberScopedGuard::~FiberScopedGuard()
		{
			ConvertFiberToThread();
			GetTlsContext() = nullptr;
		}

		static void*& GetTlsContext()
		{
			static thread_local void* native = nullptr;
			return native;
		}
	};

	class Context
	{
	public:
		typedef void (_CALLTYPE *fn_t)(intptr_t);

	public:
		Context(void)
			: _Ctx(NULL)
		{

		}

		~Context(void)
		{
			Release();
		}
		
		int Init(fn_t fn, Int64 vp, std::size_t stackSize)
		{
			UInt32 commit_size = 4 * 1024;
			_Ctx = CreateFiberEx(commit_size,
				__max(stackSize, commit_size), FIBER_FLAG_FLOAT_SWITCH,
				(LPFIBER_START_ROUTINE)fn, (LPVOID)vp);

			if (!_Ctx)
				return - 1;

			return 1;
		}

		void Release(void)
		{
			if (_Ctx != NULL)
			{
				DeleteFiber(_Ctx);
				_Ctx = NULL;
			}
		}

		_INLINE_ void SwapIn(void)
		{
			SwitchToFiber(_Ctx);
		}

		_INLINE_ void SwapOut(void)
		{
			SwitchToFiber(FiberScopedGuard::GetTlsContext());
		}

	private:

		void* _Ctx;
	};

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

	class Coroutine
	{
	public:
		typedef function20_handle<void(void)>	HFNEvent;
		
		static MemPool_type* AllocMem(void)
		{
			static MemPool_type s_Pool(__max(sizeof(sizeof(struct co)), sizeof(struct comng)));
			return &s_Pool;
		}

	public:
		Coroutine(void)
		{

		}

		~Coroutine(void)
		{

		}

		int Init(void)
		{
			return 1;
		}

		void Release(void)
		{

		}

		int Open(const HFNEvent& hEvent)
		{

		}

		void Close(void)
		{

		}

		void Resume(void)
		{
			
		}

		void Yield(void)
		{

		}

		void Running(void)
		{
			
		}

		void Status(void)
		{

		}

	private:
		HFNEvent	_hEvent;

	};

}




#endif //PLATFORM_OS_FAMILY_WINDOWS

#endif//__COROUTINE_WIN32_20150806193305_1438860785_H__
