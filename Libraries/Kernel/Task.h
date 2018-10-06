#ifndef __Task_H__
#define __Task_H__

#include "Kernel\Sys.h"

class TaskScheduler;

// ����
class Task
{
public:
	TaskScheduler* Host;

	uint32_t	ID;			// ��� 4
	cstring	Name;		// ����

	Action	Callback;	// �ص� 4
	void*	Param;		// ����

	int		Period;		// ����ms 5
	uint64_t	NextTime;	// ��һ��ִ��ʱ��ms 3

	int		Times;		// ִ�д��� 8
	int		SleepTime;	// ��ǰ˯��ʱ��us 9
	int		Cost;		// ƽ��ִ��ʱ��us 10
	int		CostMs;		// ƽ��ִ��ʱ��ms 11
	int		MaxCost;	// ���ִ��ʱ��us 12

	bool	Enable;		// �Ƿ����� 52
	bool	Event;		// �Ƿ�ִֻ��һ�κ���ͣ���¼������� 53
	uint8_t	Deepth;		// ��ǰ��� 54
	uint8_t	MaxDeepth;	// �����ȡ�Ĭ��1�㣬���������� 55

	Task();
	Task(const Task& task)	= delete;
	~Task();

	// ִ�����񡣷����Ƿ�����ִ�С�
	bool Execute(uint64_t now);
	// ��������Ŀ���״̬��ͬʱ����ָ���������һ�ε��ȵ�ʱ�䣬0��ʾ���ϵ���
	void Set(bool enable, int msNextTime = -1);
	// ��ʾ״̬
	void ShowStatus();

	// ȫ�����������
	static TaskScheduler* Scheduler();
	static Task* Get(int taskid);
	static Task& Current();

private:
	friend class TaskScheduler;

	bool CheckTime(uint64_t end, bool isSleep);
	void Init();
};
// ���������
class TaskScheduler
{
private:
	List<Task*>	_Tasks;	// �����б�
	bool	_SkipSleep;	// �������һ��˯�ߣ����Ͽ�ʼ��һ��ѭ��

	friend class Task;

public:
	cstring	Name;	// ϵͳ���� 4
	int		Count;		// �������
	Task*	Current;	// ����ִ�е�����
	bool	Running;	// �Ƿ���������
	bool	Sleeping;	// �����ǰ����Sleep״̬������ֹͣ���˳�
	uint8_t	Deepth;		// ��ǰ���
	uint8_t	MaxDeepth;	// �����ȡ�Ĭ��5��

	int		Times;		// ִ�д��� 14
	int		Cost;		// ƽ��ִ��ʱ��us
	uint64_t	TotalSleep;	// �����������˯��ʱ��ms 12
	uint64_t	LastTrace;	// ���ͳ�Ƹ���ʱ��ms

	typedef void (*SAction)(int ms);
	SAction	EnterSleep;	// ֪ͨ�ⲿ����Ҫ˯�����ɺ���
	Func	ExitSleep;	// ֪ͨ�ⲿ��Ҫ���˳�˯�ߣ��ָ�����

	TaskScheduler(cstring name = nullptr);

	// ʹ���ⲿ��������ʼ�������б�������Ƶ���Ķѷ���
	void Set(Task* tasks, int count);
	// �������� ����ʹ�ô˺������׸������ID
	uint32_t FindID(Action func);
	// �������� ����ʹ�ô˺������׸�����
	Task* FindTask(Action func);
	// �������񣬷��������š�dueTime�״ε���ʱ��ms��-1��ʾ�¼�������period���ȼ��ms��-1��ʾ������һ��
	uint32_t Add(Action func, void* param, int dueTime = 0, int period = 0, cstring name = nullptr);
	template<typename T>
	uint32_t Add(void(T::*func)(), T* target, int dueTime = 0, int period = 0, cstring name = nullptr)
	{
		return Add(*(Action*)&func, target, dueTime, period, name);
	}
	void Remove(uint32_t taskid);

	void Start();
	void Stop();
	// ִ��һ��ѭ����ָ��������ʱ��
	void Execute(uint32_t msMax, bool& cancel);
	uint32_t ExecuteForWait(uint32_t msMax, bool& cancel);

	// �������һ��˯�ߣ����Ͽ�ʼ��һ��ѭ��
	void SkipSleep();

	void ShowStatus();	// ��ʾ״̬

    Task* operator[](int taskid);
};

#endif