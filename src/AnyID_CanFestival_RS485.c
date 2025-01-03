#include "AnyID_CanFestival_RS485.h"

UART_RCVFRAME g_sUartRcvTempFrame = {0};
UART_RCVFRAME g_sRS485RcvFrame = {0};

char g_nRs485DebugInfo[R485_DEBUG_ERR_INFO_LEN] = {0};
void RS485_Init(u32 bud)
{

	Uart_InitInterface(bud);
	Uart_ConfigInt();
	Uart_EnableInt(ENABLE, DISABLE);
}