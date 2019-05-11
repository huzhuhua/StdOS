#ifndef _ARCH_UART_H
#define _ARCH_UART_H

#include <stdint.h>
#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
	{
		//uint8_t data[1024];
		uint8_t data[512];
		uint32_t len;
	}
	_data_Q_t;
	
	typedef void (*pFun_UART)(uint8_t c); 
	
	
	void UART1_Dis_IRQ(void);
	void UART2_Dis_IRQ(void);
	void UART3_Dis_IRQ(void);
	void UART4_Dis_IRQ(void);
	void UART5_Dis_IRQ(void);
	
	void UART1_En_IRQ(void);
	void UART2_En_IRQ(void);
	void UART3_En_IRQ(void);
	void UART4_En_IRQ(void);
	void UART5_En_IRQ(void);
	
	void UART_1_send_byte(uint8_t c);
	void UART_2_send_byte(uint8_t c);
	void UART_3_send_byte(uint8_t c);
	void UART_4_send_byte(uint8_t c);
	void UART_5_send_byte(uint8_t c);

#ifdef __cplusplus
}
#endif

#endif
