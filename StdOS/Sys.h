#ifndef _Sys_H_
#define _Sys_H_

#include <stdint.h>
#include "../component/Type.h"
#include "List.h"
#include "Delegate.h"
#include "../component/lib/Version.h"

int StdPrintf(const char *format, ...); //���� printf
#ifdef __cplusplus
extern "C" {
#endif
#ifdef DEBUG
	#define debug_printf StdPrintf	
#else
	#define debug_printf(format, ...)
#endif
	void delay_ms(int ms); // ���뼶�ӳ� ����ϵͳû����ʱ��ʱʹ��
	void delay_us(int us); // ΢�뼶�ӳ�
#ifdef __cplusplus
}
#endif

#ifdef DEBUG
#define assert(expr, msg) ((expr) ? (void)0 : assert_failed2(msg, (const char*)__FILE__, __LINE__))

#else

#define assert(expr, msg) ((void)0)

#endif

// ϵͳ��
class TSys
{
public:
    uint32_t	Clock;  	// ϵͳʱ��
    uint32_t	CystalClock;// ����ʱ��
	uint32_t	HeapSize;	// �Ѵ�С
	uint32_t	StackSize;	// ջ��С

	uint8_t	ID[12];		// оƬID��
    uint16_t	FlashSize;	// оƬFlash������
    //uint16_t	RAMSize;	// оƬRAM����
	Version OsVer;//ϵͳ�汾
	Version AppVer;//�����汾

	TSys();

	void Init();     	// ��ʼ��ϵͳ
	void ShowInfo() const;
	
	uint64_t	Ms() const;		// ϵͳ������ĺ�����
	uint32_t	Seconds() const;	// ϵͳ���Ե�ǰʱ�䣬��

    void Sleep(uint32_t dwMs) const; // ���뼶�ӳ�
    void delayMicroseconds(uint32_t usec) const; // ΢�뼶�ӳ�
	
	// �ӳ��첽����
	void Reboot(int msDelay = 0) const;
	// ��ȫ���ж�
	void GlobalEnable();
	// �ر�ȫ���ж�
	void GlobalDisable();
		
private:
	void Reset() const;
	void OnInit();
	void OnShowInfo() const;

public:
	// �������񣬷��������š�dueTime�״ε���ʱ��ms��period���ȼ��ms��-1��ʾ������һ��
	uint32_t AddTask(Action func, void* param, int dueTime = 0, int period = 0, const char* name = nullptr) const;
	template<typename T>
	uint32_t AddTask(void(T::*func)(), T* target, int dueTime = 0, int period = 0, const char* name = nullptr)
	{
		return AddTask(*(Action*)&func, target, dueTime, period, name);
	}
	void RemoveTask(uint32_t& taskid) const;
	// ��������Ŀ���״̬��ͬʱ����ָ���������һ�ε��ȵ�ʱ�䣬0��ʾ���ϵ���
	bool SetTask(uint32_t taskid, bool enable, int msNextTime = -1) const;
	// �ı���������
	bool SetTaskPeriod(uint32_t taskid, int period) const;

	bool Started;
	void Start();	// ��ʼϵͳ��ѭ��
};

extern TSys Sys;		// ����һ��ȫ�ֵ�Sys����  ����main����֮ǰִ�й��캯����������������

#define STDOS_VERSION "0.8.2019.0114" //ϵͳ�汾��
extern char* AppVersion;//��Ҫ���������汾��ʵ�� �磺char *AppVersion = "0.1.2018.1114";

#endif //_Sys_H_

/*

v0.1.2018.0820  ʹ��C++11ʵ�֣�ʵ��F1 F4 F1��GPIO�ʹ��ڡ�IIC��SPI���жϹ���
v0.2.2018.		���ڷ��͡����ղ���DMA��ʽʵ�֡�
v0.3.2018       ��������ʵ�ַ�ʽ
v0.4.2018.1024  ���������Ż�������ʱ�ر�ȫ���жϣ�������ɿ�������ֹ�����쳣
v0.5.2018.1030  ϵͳͳ����Ϣ����ȥ������Ҫ����
*/