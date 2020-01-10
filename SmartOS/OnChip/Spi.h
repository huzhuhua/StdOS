#ifndef __SPI_H__
#define __SPI_H__

#include "Device\Port.h"

// Spi类
class Spi
{
private:
    byte	_index;
    void*	_SPI;
	Pin		Pins[4];	// NSS/CLK/MISO/MOSI
    OutputPort	_nss;

    AlternatePort _clk;
    AlternatePort _miso;
    AlternatePort _mosi;

	void Init();

public:
    int		Speed;  // 速度
    int		Retry;  // 等待重试次数，默认200
    int		Error;  // 错误次数
	bool	Opened;

	Spi();
	// 使用端口和最大速度初始化Spi，因为需要分频，实际速度小于等于该速度
    Spi(SPI spi, uint speedHz = 9000000, bool useNss = true);
    ~Spi();

	void Init(SPI spi, uint speedHz = 9000000, bool useNss = true);

	void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0, Pin nss = P0);
	void GetPin(Pin* clk = nullptr, Pin* miso = nullptr, Pin* mosi = nullptr, Pin* nss = nullptr);
	void Open();
	void Close();

	// 基础读写
    byte Write(byte data);
    ushort Write16(ushort data);

	// 批量读写。以字节数组长度为准
	void Write(const Buffer& bs);
	void Read(Buffer& bs);

    void Start();   // 拉低NSS，开始传输
    void Stop();    // 拉高NSS，停止传输
	
private:
	static int GetPre(int index, uint& speedHz);
	void OnInit();
	void OnOpen();
	void OnClose();
};

// Spi会话类。初始化时打开Spi，超出作用域析构时关闭
class SpiScope
{
private:
	Spi* _spi;

public:
	SpiScope(Spi* spi)
	{
		_spi = spi;
		_spi->Start();
	}

	~SpiScope()
	{
		_spi->Stop();
	}
};

// Spi设备配置
class SpiConfig
{
public:
	SPI Spi;

	Pin	Irq;
	Pin Reset;
};

typedef enum
{
	CPOL_Low = 0,  // 时钟极性，空闲时为低
	CPOL_High = 1,  // 时钟极性，空闲时为高
} CPOLTYPE;
typedef enum
{
	CPHA_1Edge = 0,  //时钟相位 在串行同步时钟的第一个跳变沿（上升或下降）数据被采样
	CPHA_2Edge = 1,  //时钟相位 在串行同步时钟的第二个跳变沿（上升或下降）数据被采样
} CPHATYPE;
class SpiBase
{
public:
	virtual void Init(CPOLTYPE cpol = CPOL_High, CPHATYPE cpha = CPHA_2Edge)=0;

	virtual void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0)=0;
	virtual void SetNss(Pin nss = P0)=0;

	// 基础读写
	virtual byte Write(byte data)=0;
	byte Read() { return this->Write(0XFF); }
	virtual ushort Write16(ushort data) = 0;

	virtual void Start()=0; // 拉低NSS，开始传输
	virtual void Stop()=0; // 拉高NSS，停止传输
};
class SpiSoft_ARMKU :public SpiBase
{
	public:
		CPOLTYPE CPOL; //时钟极性
		CPHATYPE CPHA; //时钟相位
	public:
		SpiSoft_ARMKU(); //使用端口和最大速度初始化Spi，因为需要分频，实际速度小于等于该速度   
		virtual void Init(CPOLTYPE cpol = CPOL_High, CPHATYPE cpha = CPHA_2Edge);
		virtual void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0);
		virtual void SetNss(Pin nss = P0);
		virtual byte Write(byte data);
		virtual ushort Write16(ushort data);
		void Open();
		void Close();
		virtual void Start(); // 拉低NSS，开始传输
		virtual void Stop(); // 拉高NSS，停止传输
	private:
		Port _nss;
		Port _clk;
		Port _mosi;
		Port _miso;
		uint delayus; //延时时间
	private:
		byte WaitBusy();
};
#endif
