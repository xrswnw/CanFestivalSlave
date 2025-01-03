#ifndef _ANYID_CANFESTIVAL_RS485_
#define _ANYID_CANFESTIVAL_RS485_

#include "AnyID_CanFestival_RS485_HL.h"
#include "AnyID_CanFestival_Uart_Receive.h"


#define R485_DEBUG_ERR_INFO_LEN		256


#define Debug_InfoClear()			do{memset(g_nRs485DebugInfo, 0, R485_DEBUG_ERR_INFO_LEN);}while(0)

extern UART_RCVFRAME g_sRS485RcvFrame;
extern UART_RCVFRAME g_sUartRcvTempFrame;
extern char g_nRs485DebugInfo[R485_DEBUG_ERR_INFO_LEN];


void RS485_Init(u32 bud);

#endif
