
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>

// Ĭ��Э��ջ��С �� ��ʼ��Э������
#define STACK_INT       (1048576)
#define COROUTINE_INT   (16)

#define CO_DEAD         (0)     // Э������״̬
#define CO_READY        (1)     // Э���Ѿ�����
#define CO_RUNNING      (2)     // Э����������
#define CO_SUSPEND      (3)     // Э����ͣ�ȴ�

// comng_t - Э�̹���������
typedef struct comng * comng_t;

// co_f - Э�����к�����	
// g        : Э�̹���������
// arg      : �û�����Э�̴������
typedef void(*co_f)(comng_t g, void * arg);

// ���� co - Э�̽ṹ �� comng - Э�̹������ṹ
struct co {
	co_f func;              // Э��ִ����Ϊ
	void * arg;             // Э��ִ�в���
	int status;             // ��ǰЭ��״̬ CO_*
	void * ctx;             // ��ǰЭ�����е������Ļ���
};

struct comng {
	struct co ** cs;        // Э�̶���, ѭ������
	int cap;                // Э�̶�������
	int idx;                // ��ǰЭ�̼�����ѯ��������
	int len;                // ��ǰЭ�̼����ڵ�Э�̸���

	int running;            // ��ǰЭ�̼������е�Э�� id
	void * ctx;             // ��ǰ��Э�̼�¼���������Ļ���
};

// comng_run - Э�̹���������ʵ��
static void __stdcall comng_run(void* g) {
	int id = ((comng*)g)->running;
	struct co * c = ((comng*)g)->cs[id];

	// ִ��Э����
	c->func(((comng*)g), c->arg);
	c->status = CO_DEAD;

	// ��ת�����˳���������
	SwitchToFiber(((comng*)g)->ctx);
}


int main(int argc, char* argv[])
{
	comng_t g;

	CreateFiberEx(STACK_INT, 0, FIBER_FLAG_FLOAT_SWITCH, comng_run, g);

	return 0;
}
