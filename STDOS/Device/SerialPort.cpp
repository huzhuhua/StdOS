#include "Sys.h"
#include <stdio.h>
#include "stm32f10x.h"
#include "SerialPort.h"
#include "TInterrupt.h"
#include "TTime.h"

#define COM_DEBUG 0
#define STM32F1XX

#define UARTS {USART1,USART2,USART3,UART4,UART5}

//以下为过期需要整理内容
//		private:
//            byte _index;
//            USART_TypeDef *_port;
//            AlternatePort _tx;
//            #if defined(STM32F0) || defined(STM32F4)
//                AlternatePort _rx;
//            #else 
//                InputPort _rx;
//            #endif 
//        public:
//            SerialPort(byte index, int baudRate = 115200, byte parity = USART_Parity_No, byte dataBits = USART_WordLength_8b, byte stopBits = USART_StopBits_1);
//SerialPort(USART_TypeDef *com, int baudRate = 115200, byte parity = USART_Parity_No, byte dataBits = USART_WordLength_8b, byte stopBits = USART_StopBits_1);            
//            void Init(byte index, int baudRate = 115200, byte parity = USART_Parity_No, byte dataBits = USART_WordLength_8b, byte stopBits = USART_StopBits_1); 
//        protected:            
//            virtual bool OnWrite(const byte *buf, uint size);
//            virtual uint OnRead(byte *buf, uint size);
//        public:
//原始:private
//            static void OnUsartReceive(ushort num, void *param);
SerialPort::SerialPort()
{
    Init();
}

SerialPort::SerialPort(COM index, int baudRate)
{
    this->Index = index;
    if (baudRate <= 0)
    {
        this->_baudRate = 115200;
    }
    else
    {
        this->_baudRate = baudRate;
    }
}

//SerialPort::SerialPort(USART_TypeDef *com, int baudRate, byte parity, byte dataBits, byte stopBits)
//{
//    assert_param(com);
//    const USART_TypeDef *const g_Uart_Ports[] = UARTS;
//    byte _index = 0xFF;

//    for (int i = 0; i < ArrayLength(g_Uart_Ports); i++)
//    {
//        if (g_Uart_Ports[i] == com)
//        {
//            _index = i;
//            break;
//        }
//    }
//    Init();
//    Init(_index, baudRate, parity, dataBits, stopBits);
//}

// 析构时自动关闭
SerialPort::~SerialPort()
{
    if (RS485)
    {
        delete RS485;
    }
    RS485 = NULL;
}

void SerialPort::Init()
{
    //    _index = 0xFF;
    RS485 = NULL;
    Error = 0;

    Remap = 0;
}


//void SerialPort::Init(byte index, int baudRate, byte parity, byte dataBits, byte stopBits)
//{

//    USART_TypeDef *const g_Uart_Ports[] = UARTS;
//    _index = index;
//	this->Index=(COM)index;
//    assert_param(_index < ArrayLength(g_Uart_Ports));

//    _port = g_Uart_Ports[_index];
//    _baudRate = baudRate;
//    _parity = parity;
//    _dataBits = dataBits;
//    _stopBits = stopBits;
//    // 根据端口实际情况决定打开状态
//    if (_port->CR1 &USART_CR1_UE)
//        Opened = true;
//    // 设置名称
//    //Name = "COMx";
//    *(uint*)Name = *(uint*)"COMx";
//    Name[3] = '0' + _index + 1;
//    Name[4] = 0;
//}

bool SerialPort::OnWrite(const Buffer &bs)
{
    return false;
}

uint SerialPort::OnRead(Buffer &bs)
{
    return 0;
}

#define UART_PINS {PA9,PA10,PA2,PA3,PB10,PB11,PC10,PC11,PC12,PD3}
#define UART_PINS_FULLREMAP {PA9,PA10,PA2,PA3,PB10,PB11,PC10,PC11,PC12,PD3}   //需要整理
// 获取引脚
void GetPins(Pin *txPin, Pin *rxPin, COM index, bool Remap = false)
{

    *rxPin =  *txPin = P0;

    const Pin g_Uart_Pins[] = UART_PINS;
    const Pin g_Uart_Pins_Map[] = UART_PINS_FULLREMAP;
    const Pin *p = g_Uart_Pins;
    if (Remap)
    {
        p = g_Uart_Pins_Map;
    }
    int n = index << 1;
    *txPin = p[n];
    *rxPin = p[n + 1];
}

// 打开串口
bool SerialPort::OnOpen()
{
    Pin rx, tx;


    if (COM_NONE == Sys.MessagePort)
    {
        return false;
    }
    GetPins(&tx, &rx, this->Index);

    //    debug_printf("Serial%d Open(%d, %d, %d, %d)\r\n", _index + 1, _baudRate, _parity, _dataBits, _stopBits);
    #if COM_DEBUG        

        ShowLog: debug_printf("Serial%d Open(%d", _index + 1, _baudRate);
        switch (_parity)
        {
            case USART_Parity_No:
                debug_printf(", Parity_None");
                break;
            case USART_Parity_Even:
                debug_printf(", Parity_Even");
                break;
            case USART_Parity_Odd:
                debug_printf(", Parity_Odd");
                break;
        }
        switch (_dataBits)
        {
            case USART_WordLength_8b:
                debug_printf(", WordLength_8b");
                break;
            case USART_WordLength_9b:
                debug_printf(", WordLength_9b");
                break;
        }
        switch (_stopBits)
        {
            #ifdef STM32F10X
                case USART_StopBits_0_5:
                    debug_printf(", StopBits_0_5");
                    break;
                #endif 
            case USART_StopBits_1:
                debug_printf(", StopBits_1");
                break;
            case USART_StopBits_1_5:
                debug_printf(", StopBits_1_5");
                break;
            case USART_StopBits_2:
                debug_printf(", StopBits_2");
                break;
        }
        debug_printf(") TX=P%c%d RX=P%c%d\r\n", _PIN_NAME(tx), _PIN_NAME(rx));

        // 有可能是打开串口完成以后跳回来
        if (Opened)
            return true;

    #endif 

    USART_InitTypeDef p;

    //串口引脚初始化
    AlternatePort txx;
    InputPort rxx;
    txx.Set(tx);
    rxx.Set(rx);

    // 不要关调试口，否则杯具
    //    if (_index != Sys.MessagePort)
    //        USART_DeInit(_port);
    // USART_DeInit其实就是关闭时钟，这里有点多此一举。但为了安全起见，还是使用

    // 检查重映射
    #ifdef STM32F1XX
        if (Remap)
        {
            switch (this->Index)
            {
                case 0:
                    AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;
                    break;
                case 1:
                    AFIO->MAPR |= AFIO_MAPR_USART2_REMAP;
                    break;
                case 2:
                    AFIO->MAPR |= AFIO_MAPR_USART3_REMAP_FULLREMAP;
                    break;
            }
        }
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    #endif 

    // 打开 UART 时钟。必须先打开串口时钟，才配置引脚
    #ifdef STM32F0XX
        switch (this->Index)
        {
            case COM1:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
                break; //开启时钟
            case COM2:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
                break;
            default:
                break;
        }
    #else 
        if (this->Index)
        {
            // COM2-5 on APB1
            RCC->APB1ENR |= RCC_APB1ENR_USART2EN >> 1 << this->Index;
        }
        else
        {
            // COM1 on APB2
            //RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        }
    #endif 

    #ifdef STM32F0
        GPIO_PinAFConfig(_GROUP(tx), _PIN(tx), GPIO_AF_1); //将IO口映射为USART接口
        GPIO_PinAFConfig(_GROUP(rx), _PIN(rx), GPIO_AF_1);
    #elif defined(STM32F4)
        const byte afs[] = 
        {
            GPIO_AF_USART1, GPIO_AF_USART2, GPIO_AF_USART3, GPIO_AF_UART4, GPIO_AF_UART5, GPIO_AF_USART6, GPIO_AF_UART7, GPIO_AF_UART8
        };
        GPIO_PinAFConfig(_GROUP(tx), _PIN(tx), afs[_index]);
        GPIO_PinAFConfig(_GROUP(rx), _PIN(rx), afs[_index]);
    #endif 

    USART_StructInit(&p);
    p.USART_BaudRate = _baudRate;
    p.USART_WordLength = _dataBits;
    p.USART_StopBits = _stopBits;
    p.USART_Parity = _parity;
    USART_TypeDef *const g_Uart_Ports[] = UARTS;
    USART_Init(g_Uart_Ports[this->Index], &p);

    USART_ITConfig(g_Uart_Ports[this->Index], USART_IT_RXNE, ENABLE); // 串口接收中断配置
    // 初始化的时候会关闭所有中断，这里不需要单独关闭发送中断
    //USART_ITConfig(_port, USART_IT_TXE, DISABLE); // 不需要发送中断

    USART_Cmd(g_Uart_Ports[this->Index], ENABLE); //使能串口

    if (RS485)
    {
        *RS485 = false;
    }

    //Opened = true;    
    #if COM_DEBUG
        if (_index == Sys.MessagePort)
        {
            // 提前设置为已打开端口，ShowLog里面需要判断
            Opened = true;
            goto ShowLog;
        }
    #endif 

    return true;
}

// 关闭端口
void SerialPort::OnClose()
{
    //    debug_printf("~Serial%d Close\r\n", _index + 1);

    //    Pin tx, rx;

    //    GetPins(&tx, &rx);

    //    USART_DeInit(_port);

    // 检查重映射
    #ifdef STM32F1XX
        if (Remap)
        {
            //            switch (_index)
            //            {
            //                case 0:
            //                    AFIO->MAPR &= ~AFIO_MAPR_USART1_REMAP;
            //                    break;
            //                case 1:
            //                    AFIO->MAPR &= ~AFIO_MAPR_USART2_REMAP;
            //                    break;
            //                case 2:
            //                    AFIO->MAPR &= ~AFIO_MAPR_USART3_REMAP_FULLREMAP;
            //                    break;
            //            }
        }
    #endif 
}

// 发送单一字节数据
int SerialPort::SendData(byte data, int times)
{
    USART_TypeDef *const g_Uart_Ports[] = UARTS;
    switch (this->Index)
    {
        case COM1:
            while (USART_GetFlagStatus(g_Uart_Ports[0], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
        case COM2:
            while (USART_GetFlagStatus(g_Uart_Ports[1], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
        case COM3:
            while (USART_GetFlagStatus(g_Uart_Ports[2], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
        case COM4:
            while (USART_GetFlagStatus(g_Uart_Ports[3], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
        case COM5:
            while (USART_GetFlagStatus(g_Uart_Ports[4], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
        default:
            //while (USART_GetFlagStatus(g_Uart_Ports[0], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
    }
    //等待发送完毕
    if (times > 0)
    {
        {
            switch (this->Index)
            {
                case COM1:
                    USART_SendData(g_Uart_Ports[0], (ushort)data);
                    break;
                case COM2:
                    USART_SendData(g_Uart_Ports[1], (ushort)data);
                    break;
                case COM3:
                    USART_SendData(g_Uart_Ports[2], (ushort)data);
                    break;
                case COM4:
                    USART_SendData(g_Uart_Ports[3], (ushort)data);
                    break;
                case COM5:
                    USART_SendData(g_Uart_Ports[4], (ushort)data);
                    break;
                default:
                    //USART_SendData(g_Uart_Ports[0], (ushort)data);
                    break;
            }
        }
    }
    else
    {
        Error++;
    }
    return 0;
}

void SerialPort::ChangePower(int level){

}

// 向某个端口写入数据。如果size为0，则把data当作字符串，一直发送直到遇到\0为止
//bool SerialPort::OnWrite(const byte *buf, uint size)
//{
//    if (RS485)
//    {
//        *RS485 = true;
//    }
//    if (size > 0)
//    {
//        for (int i = 0; i < size; i++)
//        {
//            SendData(*buf++);
//        }
//    }
//    else
//    {
//        while (*buf)
//        {
//            SendData(*buf++);
//        }
//    }

//    if (RS485)
//    {
//        Sys.Delay(200);
//        *RS485 = false;
//    }
//    return true;
//}


// 从某个端口读取数据
//uint SerialPort::OnRead(byte *buf, uint size)
//{
//    // 在100ms内接收数据
//    uint usTimeout = 100;
//    UInt64 us = Time.Current() + usTimeout;

//    uint count = 0; // 收到的字节数    
//    while (count < size && Time.Current() < us)
//    {
//        // 轮询接收寄存器，收到数据则放入缓冲区
//        if (USART_GetFlagStatus(_port, USART_FLAG_RXNE) != RESET)
//        {
//            *buf++ = (byte)USART_ReceiveData(_port);
//            count++;
//            us = Time.Current() + usTimeout;
//        }
//    }
//    return count;
//}

// 刷出某个端口中的数据
bool SerialPort::Flush(int times)
{
    //uint times = 3000;
    //    while (USART_GetFlagStatus(_port, USART_FLAG_TXE) == RESET && --times > 0)
    //        ;
    //等待发送完毕
    return times > 0;
}

#ifdef STM32F10X_HD
    #define UART_IRQs {USART1_IRQn,USART2_IRQn,USART3_IRQn,UART4_IRQn,UART5_IRQn}
#else 
    #define UART_IRQs {USART1_IRQn,USART2_IRQn,USART3_IRQn}
#endif 

void SerialPort::Register(TransportHandler handler, void *param)
{
    ITransport::Register(handler, param);

    //    const byte irqs[] = UART_IRQs;
    //    byte irq = irqs[_index];
    if (handler)
    {
        //        Interrupt.SetPriority(irq, 1);
        //        Interrupt.Activate(irq, SerialPort::OnUsartReceive, this);
    }
    else
    {
        //        Interrupt.Deactivate(irq);
    }
}

// 真正的串口中断函数
//void SerialPort::OnUsartReceive(ushort num, void *param)
//{
//    SerialPort *sp = (SerialPort*)param;

//    if (sp && sp->HasHandler())
//    {
//        if (USART_GetITStatus(sp->_port, USART_IT_RXNE) != RESET)
//        {
//            // 从栈分配，节省内存
//            byte buf[512];
//            uint len = sp->Read(buf, sizeof(buf));
//            if (len)
//            {
//                len = sp->OnReceive(buf, len);
//                #if 0
//                    assert_param(len <= ArrayLength(buf));
//                #endif 
//                // 如果有数据，则反馈回去
//                #if 0
//                    if (len)
//                    {
//                        sp->Write(buf, len);
//                    }
//                #endif 
//            }
//        }
//    }
//}

SerialPort *_printf_sp;
bool isInFPutc; //正在串口输出
extern "C"
{
    /* 重载fputc可以让用户程序使用printf函数 */
    int fputc(int ch, FILE *f)
    {
        //        if (!Sys.Inited)
        //            return ch;

        if (Sys.MessagePort == COM_NONE)
            return ch;

        if (isInFPutc)
            return ch;
        isInFPutc = true;

        // 检查并打开串口
        //if ((port->CR1 &USART_CR1_UE) != USART_CR1_UE && _printf_sp == NULL)
        if (_printf_sp == NULL)
        {
            _printf_sp = new SerialPort(COM(Sys.MessagePort));
            _printf_sp->Open();
        }
        _printf_sp->SendData((byte)ch);
        isInFPutc = false;
        return ch;
    }
}

SerialPort *SerialPort::GetMessagePort()
{
    if (!_printf_sp)
    {
        if (Sys.MessagePort == COM_NONE)
        {
            return NULL;
        }
        _printf_sp = new SerialPort(COM(Sys.MessagePort));
        _printf_sp->Open();
    }

    return _printf_sp;
}
