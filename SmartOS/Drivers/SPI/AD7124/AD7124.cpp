#include "AD7124.h"
#include "AD7124def.h"

#include "Sys.h"

void AD7124::SetSpi(SpiBase_ARMKU *spi)
{
	this->pspi = spi;
}

ushort AD7124::Write16(ushort sendData)
{
    ushort ret = 0;
    ret = this->pspi->Write(sendData >> 8);
    ret <<= 8;
    ret += this->pspi->Write(sendData &0xff);
    return ret;
}

uint AD7124::Write24(uint sendData)
{
    uint ret = 0;
    ret = this->pspi->Write(sendData >> 16);
    ret <<= 8;
    ret += this->pspi->Write(sendData >> 8);
    ret <<= 8;
    ret += this->pspi->Write(sendData &0xff);
    return ret;
}

uint AD7124::Write32(uint sendData)
{
    uint ret = 0;
    ret = this->pspi->Write(sendData >> 24);
    ret <<= 8;
    ret += this->pspi->Write(sendData >> 16);
    ret <<= 8;
    ret += this->pspi->Write(sendData >> 8);
    ret <<= 8;
    ret += this->pspi->Write(sendData &0xff);
    return ret;
}

uint AD7124::ReadReg(byte reg, byte bytes)
{
    uint retVal;
    this->pspi->Start();
    this->pspi->Write(AD7124_RD | reg);
    if (bytes == 1)
    {
        retVal = this->pspi->Write(0xFF);
    }
    else if (bytes == 2)
    {
        retVal = Write16(0xFFFF);
    }
    else if (bytes == 3)
    {
        retVal = Write24(0xFFFFFF);
    }
    this->pspi->Stop();
    return retVal;
}
//读取AD值
uint AD7124::ReadRlt()
{
	this->pspi->Stop();
	delayMicroseconds(5);
	uint ret= this->ReadReg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
	delayMicroseconds(5);
	this->pspi->Stop();
	this->pspi->Start();
	return ret;
}
//读取AD值
uint AD7124::ReadRlt(byte& status)
{
	uint adin=this->ReadReg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES+1);
	status=adin&0xff;
	adin>>=8;
	return adin;
}
/*******************************************************************************
 * @function	: ReadReg
 * @brief		: 读取AD7124寄存器内容
 * @param		: reg   寄存器地址
 * @param		: bytes 寄存器大小
 * @retval		: 读取的内容
 * @notes		: 
 *****************************************************************************/
uint AD7124::ReadRegNoCS(byte reg, byte bytes)
{
    uint retVal;
    this->pspi->Write(AD7124_RD | reg);
    if (bytes == 1)
    {
        retVal = this->pspi->Write(0xFF);
    }
    else if (bytes == 2)
    {
        retVal = Write16(0xFFFF);
    }
    else if (bytes == 3)
    {
        retVal = Write24(0xFFFFFF);
    }
    return retVal;
}

/*******************************************************************************
 * @function	: WriteReg
 * @brief		: 写AD7124寄存器
 * @param		: reg	  寄存器地址
 * @param		: bytes 寄存器大小
 * @param		: data	写入的数据
 * @retval		: 无
 * @notes		: 
 *****************************************************************************/
void AD7124::WriteReg(byte reg, byte bytes, uint data)
{
    this->pspi->Start();
    this->pspi->Write(AD7124_WR | reg);
    if (bytes == 1)
    {
        this->pspi->Write(data);
    }
    else if (bytes == 2)
    {
        Write16(data);
    }
    else if (bytes == 3)
    {
        Write24(data);
    }
    this->pspi->Stop();
	this->pspi->Start();
}

/*******************************************************************************
 * @function	: Init
 * @brief		: SPI端口配置，模拟SPI
 * @param		: 无
 * @retval		: 无
 * @notes		: 
 *****************************************************************************/
void AD7124::Init_8()		//8通道初始化
{
	/*
	采用内部基准电压
	AIN0	NONE
	AIN1	NONE
	AIN2	NONE
	AIN3	NONE
	AIN4	NONE
	AIN5	AIN5	动压
	AIN6	AIN9	气室压力
	AIN7	AIN10	大泵流压
	AIN8	AIN6	静压
	AIN9	AIN7	小泵流压
	AIN10	AIN8	小泵计压
	AIN11	AIN11	大泵计压
	AIN12	AIN4	干温
	AIN13	AIN2	湿温温度
	AIN14	AIN3	计温
	AIN15	AIN1	烟温
	*/
//    WriteReg(AD7124_ADC_CTRL_REG, AD7124_ADC_CTRL_REG_BYTES, AD7124_ADC_CTRL_REG_DATA);
//    WriteReg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, AD7124_IO_CTRL1_REG_DATA);
//    WriteReg(AD7124_IO_CTRL2_REG, AD7124_IO_CTRL2_REG_BYTES, AD7124_IO_CTRL2_REG_DATA);

//    WriteReg(AD7124_FILT0_REG, AD7124_FILT0_REG_BYTES, AD7124_FILT0_REG_DATA);
//    WriteReg(AD7124_FILT1_REG, AD7124_FILT1_REG_BYTES, AD7124_FILT1_REG_DATA);
//    WriteReg(AD7124_FILT2_REG, AD7124_FILT2_REG_BYTES, AD7124_FILT2_REG_DATA);
//    WriteReg(AD7124_FILT3_REG, AD7124_FILT3_REG_BYTES, AD7124_FILT3_REG_DATA);
//    WriteReg(AD7124_FILT4_REG, AD7124_FILT4_REG_BYTES, AD7124_FILT4_REG_DATA);
//	
//    WriteReg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, AD7124_IO_CTRL1_REG_DATA_TEMP1_TEST);
//    AD7124_TEMP1_CH_CFG();
//    AD7124_TEMP1_RES_CH_CFG();
//    AD7124_FLOWA_CH_CFG();
//    AD7124_FLOWB_CH_CFG();
//    AD7124_BRDTEMP_CH_CFG();
//    AD7124_ADTEMP_CH_CFG();
	
	//WriteReg(AD7124_CFG0_REG, 2, (1<<11)| (0 << 9) | (2 << 3)|(5<<0)); //双极性 开路检测电流源关闭  使用内部基准电压源 增益32倍（±78.125mv）用于测量大泵流压
	//WriteReg(AD7124_CFG1_REG, 2, (0<<11)| (0 << 9) | (2 << 3)|(5<<0)); //单极性 开路检测电流源关闭  使用内部基准电压源 增益32倍（±78.125mv）用于测量大泵计压
	//WriteReg(AD7124_CFG2_REG, 2, (0<<11)| (0 << 9) | (2 << 3)|(0<<0)); //单极性 开路检测电流源关闭  使用内部基准电压源 增益1倍（±2.5v）用于测量板子温度
	//WriteReg(AD7124_CFG3_REG, 2, (0<<11)| (0 << 9) | (2 << 3)|(5<<0)); //单极性 开路检测电流源关闭  使用内部基准电压源 增益32倍（±156.25mv）用于测量小泵计温
	WriteReg(AD7124_CFG0_REG, 2, (0 << 11) | (0 << 9) | (2 << 3) | (0 << 0)); //单极性 开路检测电流源关闭  使用内部基准电压源 增益1倍（±2.5v）用于测量板子温度
	
	//this->CHCONFIG[0]=	0x0000 |(0<<12)| (0 << 5) | (1 << 0);//使用第一种设置 AINP:0 AIN 1 大泵流压
	//this->CHCONFIG[1]=	0x0000 |(1<<12)| (2 << 5) | (3 << 0);//使用第二种配置 AINP:2 AIN 3 大泵计压
	//this->CHCONFIG[2]=	0x0000 |(0<<12)| (4 << 5) | (5 << 0);//使用第一种配置 AINP:4 AIN 5 小泵流压
	//this->CHCONFIG[3]=	0x0000 |(1<<12)| (6 << 5) | (7 << 0);//使用第二种配置 AINP:6 AIN 7 小泵计压
	//this->CHCONFIG[4]=	0x0000 |(1<<12)| (8 << 5) | (9 << 0);//使用第二种配置 AINP:8 AIN 9 大气压
	//this->CHCONFIG[5]=	0x0000 |(3<<12)| (11 << 5) | (12 << 0);//使用第二种配置 AINP:11 AIN 12 小泵计温
	//this->CHCONFIG[6]=	0x0000 |(2<<12)| (14 << 5) | (19 << 0);//使用第三种配置 AINP:14 AIN GND 板子温度

	this->CHCONFIG[0] = 0x0000 | (0 << 12) | (16 << 5) | (19 << 0);//使用第一种设置 AINP:16 芯片温度 
	this->CHCONFIG[1] = 0x0000 | (0 << 12) | (1 << 5) | (19 << 0);//使用第一种设置 AINP:1 
	this->CHCONFIG[2] = 0x0000 | (0 << 12) | (2 << 5) | (19 << 0);//使用第一种设置 AINP:2 
	this->CHCONFIG[3] = 0x0000 | (0 << 12) | (3 << 5) | (19 << 0);//使用第一种设置 AINP:3 
	this->CHCONFIG[4] = 0x0000 | (0 << 12) | (4 << 5) | (19 << 0);//使用第一种设置 AINP:4 
	this->CHCONFIG[5] = 0x0000 | (0 << 12) | (5 << 5) | (19 << 0);//使用第一种设置 AINP:5 
	this->CHCONFIG[6] = 0x0000 | (0 << 12) | (6 << 5) | (19 << 0);//使用第一种设置 AINP:6 
	this->CHCONFIG[7] = 0x0000 | (0 << 12) | (7 << 5) | (19 << 0);//使用第一种设置 AINP:7 
	this->CHCONFIG[8] = 0x0000 | (0 << 12) | (8 << 5) | (19 << 0);//使用第一种设置 AINP:8 
	this->CHCONFIG[9] = 0x0000 | (0 << 12) | (9 << 5) | (19 << 0);//使用第一种设置 AINP:9 
	this->CHCONFIG[10] = 0x0000 | (0 << 12) | (10 << 5) | (19 << 0);//使用第一种设置 AINP:10 
	this->CHCONFIG[11] = 0x0000 | (0 << 12) | (11 << 5) | (19 << 0);//使用第一种设置 AINP:11 
	this->CHCONFIG[12] = 0x0000 | (0 << 12) | (12 << 5) | (19 << 0);//使用第一种设置 AINP:12 
	this->CHCONFIG[13] = 0x0000 | (0 << 12) | (13 << 5) | (19 << 0);//使用第一种设置 AINP:13 
	this->CHCONFIG[14] = 0x0000 | (0 << 12) | (14 << 5) | (19 << 0);//使用第一种设置 AINP:14 
	this->CHCONFIG[15] = 0x0000 | (0 << 12) | (15 << 5) | (19 << 0);//使用第一种设置 AINP:15 
		
	this->SetReadChannel(0,7);

    WriteReg(AD7124_ADC_CTRL_REG, 2, (1<<10)|(1<<8)|(3<<6)); //状态寄存器传输使能 内部基准电压使能 全功率模式
	//WriteReg(AD7124_IO_CTRL1_REG, 3, (4<<11)|(4<<8)|(10<<4)|(13<<0)); //控制寄存器传输使能 IOUT1=1000ua IOUT1通过AIN10提供	
	WriteReg(AD7124_IO_CTRL1_REG, 3, (0 << 11) | (0 << 8) | (10 << 4) | (13 << 0)); //控制寄存器传输使能 电流输出关闭	
	
    this->pspi->Start();
}
/*******************************************************************************
 * @function	: Init
 * @brief		: SPI端口配置，模拟SPI
 * @param		: 无
 * @retval		: 无
 * @notes		: 
 *****************************************************************************/
void AD7124::Init_4()		//4通道初始化
{
	WriteReg(AD7124_CFG0_REG, 2, (0<<11)| (0 << 9) | (2 << 3)|(5<<0)); //单极性 开路检测电流源关闭  使用内部基准电压源 增益32倍（±78.125mv）用于测量所有温度
	
	
	this->CHCONFIG[0]=	0x0000 |(0<<12)| (0 << 5) | (1 << 0);//使用第一种设置 AINP:0 AIN 1 大泵计温
	this->CHCONFIG[1]=	0x0000 |(0<<12)| (4 << 5) | (5 << 0);//使用第一种配置 AINP:2 AIN 3 干温
	this->CHCONFIG[2]=	0x0000 |(0<<12)| (6 << 5) | (7 << 0);//使用第一种配置 AINP:4 AIN 5 湿温
		
	this->SetReadChannel(0,3);

	WriteReg(AD7124_ADC_CTRL_REG, 2, (1<<10)|(1<<8)|(3<<6)); //状态寄存器传输使能 内部基准电压使能 全功率模式
	WriteReg(AD7124_IO_CTRL1_REG, 3, (4<<11)|(4<<8)|(4<<4)|(5<<0)); //控制寄存器传输使能 IOUT1=500ua IOUT1通过AIN2提供
    
    this->pspi->Start();
}
//设置需要读取的通道，默认通道0
void AD7124::SetReadChannel(byte ch,byte chMax)
{
    for (int i = 0; i < chMax; i++)
    {
        if (i == ch)
        {
            this->CHCONFIG[i] |= 0x8000;
        }
        else
        {
            this->CHCONFIG[i] &= 0X7FFF;
        }
        this->WriteReg(AD7124_CH0_MAP_REG + i, 2, this->CHCONFIG[i]);
    }
}

/*******************************************************************************
 * @function	: ReadID
 * @brief		: 读取AD7124 ID寄存器
 * @param		: 无
 * @retval		: 无
 * @notes		: 
 *****************************************************************************/
byte AD7124::ReadID()
{
    byte retVal;
    retVal = ReadReg(AD7124_ID_REG, AD7124_ID_REG_BYTES);
    //debug_printf("ID:0x%02x\r\n", retVal);
    return retVal;
}

/*******************************************************************************
 * @function	: ReadStatus
 * @brief		: 读取AD7124 状态寄存器
 * @param		: 无
 * @retval		: 无
 * @notes		: 
 *****************************************************************************/
byte AD7124::ReadStatus()
{
    byte retVal;
    retVal = ReadReg(AD7124_STATUS_REG, AD7124_STATUS_REG_BYTES);
    //debug_printf("Status:0x%02x\r\n", retVal);
	this->pspi->Start();
    return retVal;
}
/*******************************************************************************
 * @function	: Reset
 * @brief		: 复位AD芯片
 * @param		: 无
 * @retval		: 无
 * @notes		: 
 *****************************************************************************/
void AD7124::Reset()
{
    this->pspi->Start();
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Stop();
}
