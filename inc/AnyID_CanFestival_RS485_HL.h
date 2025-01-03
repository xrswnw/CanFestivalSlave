#ifndef _ANYID_CANFESTIVAL_RS485_HL_
#define _ANYID_CANFESTIVAL_RS485_HL_

#include "AnyID_CanFestival_Config.h"

#define UART_PORT                       USART1
#define Uart_IRQHandler                 USART1_IRQHandler

#define UART_BAUDRARE                   115200

#define Uart_ReadByte()                 ((u16)(UART_PORT->DR & (u16)0x01FF))
extern const PORT_INF UART_485_CTRL;
#define Uart_Enable485Rx()      (UART_485_CTRL.Port->BRR = UART_485_CTRL.Pin)
#define Uart_Enable485Tx()      (UART_485_CTRL.Port->BSRR = UART_485_CTRL.Pin)
#define Uart_Chk485TxOver()     while(((UART_PORT)->SR & USART_FLAG_TC) == (u16)RESET)


void Uart_Delayms(u32 n);
void Uart_InitInterface(u32 baudrate);
void Uart_ConfigInt(void);
void Uart_EnableInt(FunctionalState rxState, FunctionalState txState);
void Uart_WriteByte(u8 ch);
void Uart_WriteBuffer(u8 *pFrame, u16 len);
void Uart_WriteErrStr(char *str);
void Uart_WriteWarStr(char *str);
void Uart_InitFloating();
#endif



