#ifndef _ANYID_BOOT_UART_HL_
#define _ANYID_BOOT_UART_HL_

#include "AnyID_Boot_Config.h"

#define UART_PORT                       USART2
#define Uart_IRQHandler                 USART2_IRQHandler
#define UART_BAUDRARE                   38400

#define Uart_ReadByte()         ((u16)(UART_PORT->DR & (u16)0x01FF))

void Uart_InitInterface(u32 baudrate);
void Uart_ConfigInt(void);
void Uart_EnableInt(FunctionalState rxState, FunctionalState txState);

void Uart_WriteByte(u8 ch);
void Uart_WriteBuffer(u8 *pFrame, u16 len);
#endif
