#ifndef _ANYID_UART_RECEIVE_
#define _ANYID_UART_RECEIVE_

#include "AnyID_Type.h"
#include "AnyID_Stdlib.h"

#define UART_BUFFER_MAX_LEN             (2048 + 256)
#define UART_FRAME_DATA_MAX_LEN         240
#define UART_FRAME_DATA_MIN_LEN         8
#define UART_FRAME_FLAG_HEAD1           0x7E
#define UART_FRAME_FLAG_HEAD2           0x55


#define UART_FLAG_RCV                   0x80
#define UART_STAT_IDLE                  0x00
#define UART_STAT_HEAD1                 0x81
#define UART_STAT_HEAD2                 0x82
#define UART_STAT_DATA                  0x84
#define UART_STAT_TO                    0x05
#define UART_STAT_END                   0x06


#define UART_FRAME_POS_HEAD1            0
#define UART_FRAME_POS_HEAD2            1
#define UART_FRAME_POS_LEN              2
#define UART_FRAME_POS_SRCADDR          3
#define UART_FRAME_POS_DESTADDR         5
#define UART_FRAME_POS_CMD              7
#define UART_FRAME_POS_RFU              8
#define UART_FRAME_POS_PAR              9


typedef struct uartRcvFrame{
    u8 state;
    u16 index;
    u16 dataLen;
    u8 buffer[UART_BUFFER_MAX_LEN];
    u32 idleTime;
}UART_RCVFRAME;

void Uart_ReceiveFrame(u8 byte, UART_RCVFRAME *pRcvFrame);
#define Uart_IncIdleTime(t, rcvFrame)      do{\
                                                if((rcvFrame).state & UART_FLAG_RCV)\
                                                {\
                                                    (rcvFrame).idleTime += (t);\
                                                    if((rcvFrame).idleTime >= (20 * t))\
                                                    {\
                                                        (rcvFrame).state = UART_STAT_TO;\
                                                    }\
                                                }\
                                            }while(0)

BOOL Uart_IsRcvFrameOver(UART_RCVFRAME *pRcvFrame);
#define Uart_IsRcvFrame(rcvFrame)           (Uart_IsRcvFrameOver(&(rcvFrame)))

#define Uart_ResetFrame(rcvFrame)           (memset(&(rcvFrame), 0, sizeof(UART_RCVFRAME)))
#define Uart_GetFrameCrc(p, len)            ((p)[(len) - 2] + (p)[(len) - 1] * 256)
extern UART_RCVFRAME g_sUartTempRcvFrame;
#endif

