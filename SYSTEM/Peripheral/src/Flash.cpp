#include "Flash.h"
#include "stm32f10x.h"
#include "Sys.h"
#include "stdlib.h"
#ifdef DEBUG
    #include "stdlib.h"
#endif 

/*
小容量是16~32K
中容量是64~128K
大容量是大于256K

大容量每页2k，中小容量每页1k
 */
Flash::Flash()
{
    this->BytesPerBlock = 1024;
    this->Size = 1024 * 16;
}

//设置大小及扇区大小
void Flash::SetSectorSize(int bytesperblock, int size)
{
    this->BytesPerBlock = bytesperblock;
    if (size < 0)
    {
        this->Size = 1024;
    }
}

int Flash::Erase(uint addr, int size)
{
    return 0;
}

int Flash::WriteBlock(uint addr, byte *pBuffer, int size)
{
    return 0;
}

int Flash::Read(uint addr, byte *pBuffer, int size)
{
    if (size <= 0)
    {
        return 0;
    }
    for (int i = 0; i < size / 2; i++)
    {
        ((ushort*)pBuffer)[i] = ReadHalfWord(addr); //读取2个字节.
        addr += 2; //偏移2个字节.	
    }
    if (size % 2)
    {
        pBuffer[size - 1] = this->ReadHalfWord(addr - 1);
    }
    return size;
}

int Flash::Write(uint addr, byte *pBuffer, int size)
{
    return 0;
}

#ifdef DEBUG

    void Flash::TestFlash()
    {
        
            debug_printf("\r\n\r\n");
            debug_printf("TestFlash Start......\r\n");

            uint addr = 0x08010000;

            Flash flash;
			flash.SetSectorSize(2048,512);
            debug_printf("FlashSize: %d kBytes  BytesPerBlock: %d kBytes\r\n", flash.Size, flash.BytesPerBlock);
		#if 0
            flash.Erase(addr, 0x100);

            byte buf[] = "STM32F10x SPI Firmware Library Example: communication with an AT45DB SPI FLASH";
            uint size = ArrayLength(buf);

            flash.WriteBlock(addr, buf, size);

            byte *rx = (byte*)malloc(size);
            flash.Read(addr, rx, size);

            int n = 0;
            for (int i = 0; i < size; i++)
            {
                if (buf[i] != rx[i])
                    n++;
            }
            debug_printf("diffent %d\r\n", n);

            // 集成测试
            //flash.Erase(addr, 0x100);
            flash.Write(addr, buf, size);

            flash.Read(addr, rx, size);

            n = 0;
            for (int i = 0; i < size; i++)
            {
                if (buf[i] != rx[i])
                    n++;
            }
            debug_printf("diffent %d\r\n", n);

            free(rx);

            debug_printf("\r\nTestFlash Finish!\r\n");
        #else            
            ushort buftest1[20];
			debug_printf("测试开始\r\n");
            for (int i = 0; i < 20; i++)
            {
                buftest1[i] = 1000+i;
            }
			debug_printf("-1\r\n");
            flash.Write(addr, buftest1, 20);
			debug_printf("0\r\n");
            for (int i = 0; i < 20; i++)
            {
                buftest1[i] = 0;
            }
			debug_printf("1\r\n");
            flash.Read(addr, buftest1, 20);
			debug_printf("2\r\n");
            for (int i = 0; i < 20; i++)
            {
                debug_printf("%d:%d\r\n", i, buftest1[i]);
            }
            debug_printf("测试完成\r\n");
        #endif 
    }
#endif 
//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址	

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
ushort Flash::ReadHalfWord(uint addr)
{
    return *(volatile ushort*)addr;
}

//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void Flash::Write_NoCheck(uint addr, ushort *pBuffer, ushort size)
{
    ushort i;
    for (i = 0; i < size; i++)
    {
        FLASH_ProgramHalfWord(addr, pBuffer[i]);
        addr += 2; //地址增加2.
    }
}

//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
ushort STMFLASH_BUF1[1024]; //最多是2K字节
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void Flash::Read(uint addr, ushort *pBuffer, ushort size)
{
    ushort i;
    for (i = 0; i < size; i++)
    {
        pBuffer[i] = ReadHalfWord(addr); //读取2个字节.
        addr += 2; //偏移2个字节.	
    }

}



void Flash::Write(uint addr, ushort *pBuffer, ushort size)
{
    uint secpos; //扇区地址
    ushort secoff; //扇区内偏移地址(16位字计算)
    ushort secremain; //扇区内剩余地址(16位字计算)	   
    ushort i;
    uint offaddr; //去掉0X08000000后的地址
    if (addr < STM32_FLASH_BASE || (addr >= (STM32_FLASH_BASE + 1024 * this->Size)))
        return ;
    //非法地址
    FLASH_Unlock(); //解锁
    offaddr = addr - STM32_FLASH_BASE; //实际偏移地址.
    secpos = offaddr / this->BytesPerBlock; //扇区地址  0~127 for STM32F103RBT6
    secoff = (offaddr % this->BytesPerBlock) / 2; //在扇区内的偏移(2个字节为基本单位.)
    secremain = this->BytesPerBlock / 2-secoff; //扇区剩余空间大小   
    if (size <= secremain)
        secremain = size;
    //不大于该扇区范围
    while (1)
    {
        Read(secpos *this->BytesPerBlock + STM32_FLASH_BASE, STMFLASH_BUF1, this->BytesPerBlock / 2); //读出整个扇区的内容
        for (i = 0; i < secremain; i++)
        //校验数据
        {
            if (STMFLASH_BUF1[secoff + i] != 0XFFFF)
                break;
            //需要擦除  	  
        }
        if (i < secremain)
        //需要擦除
        {
            FLASH_ErasePage(secpos *this->BytesPerBlock + STM32_FLASH_BASE); //擦除这个扇区
            for (i = 0; i < secremain; i++)
            //复制
            {
                STMFLASH_BUF1[i + secoff] = pBuffer[i];
            }
			this->WriteSector(secpos *this->BytesPerBlock + STM32_FLASH_BASE, STMFLASH_BUF1, this->BytesPerBlock / 2); //写入整个扇区  
        }
        else
        {
           this->WriteSector(addr, pBuffer, secremain);
        }
        //写已经擦除了的,直接写入扇区剩余区间. 				   
        if (size == secremain)
            break;
        //写入结束了
        else
        //写入未结束
        {
            secpos++; //扇区地址增1
            secoff = 0; //偏移位置为0 	 
            pBuffer += secremain; //指针偏移
            addr += secremain; //写地址偏移	   
            size -= secremain; //字节(16位)数递减
            if (size > (this->BytesPerBlock / 2))
                secremain = this->BytesPerBlock / 2;
            //下一个扇区还是写不完
            else
                secremain = size;
            //下一个扇区可以写完了
        }
    };
    FLASH_Lock(); //上锁
}
//擦除整个扇区
int Flash::eraseSector(uint addr)
{
    int i = 0;
    Read(addr, STMFLASH_BUF1, this->BytesPerBlock / 2); //读出整个扇区的内容
    for (i = 0; i < this->BytesPerBlock / 2; i++)
    {
        if (STMFLASH_BUF1[i] != 0XFFFF)
            break;
    }
    if (i < this->BytesPerBlock / 2)
    {
        //需要擦除
        FLASH_ErasePage(addr); //擦除这个扇区		
    }
    return this->BytesPerBlock;
}
//写扇区
void Flash::WriteSector(uint addr, ushort *pBuffer, ushort size)
{
  this->Write_NoCheck(addr, pBuffer, size);
}
