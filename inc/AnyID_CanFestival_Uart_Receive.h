#ifndef _ANYID_CANFESTIVAL_UART_RECEIVE_H
#define _ANYID_CANFESTIVAL_UART_RECEIVE_H

#include "AnyID_Type.h"
#include "AnyID_Stdlib.h"


#define UART_BUFFER_MAX_LEN             (1024)
#define UART_FRAME_DATA_MAX_LEN         252
#define UART_FRAME_PARAM_MAX_LEN        240
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

#define UART_FRAME_POS_SOUCER_ADD       2



#define UART_FRAME_POS_MQTT_CMD         0
#define UART_FRAME_POS_MQTT_NORMAL_LEN  0x20 

#define UART_RFRAME_POS_FLAG            7
#define UART_RFRAME_POS_CMD             8
#define UART_RFRAME_POS_RFU             9
#define UART_RFRAME_POS_PAR             10


#define UART_FRAME_MIN_MQTT_LEN         0

#define UART_STAT_RX_END                0x02
#define UART_STAT_RX_OVR                0x01
#define UART_STAT_RX_IDLE               0x00


#define UART_FRAME_LONG_LEN             0x00
#define UART_FRAME_MIN_LEN              11
#define UART_RFRAME_MIN_LEN             14

#define Uart_GetFrameLength(p)          (((p)[UART_FRAME_POS_LEN]) + 3)
#define Uart_GetFrameCrc(p, len)        (*((u16 *)(p + len - 2)))

#define UART_FRAME_RESPONSE_FLAG        0x1F
#define UART_FRAME_PARAM_RFU            0x00
#define UART_FRAME_PARAM_REMAIN         0x80

#define UART_FRAME_FLAG_OK              0x00
#define UART_FRAME_FLAG_FAIL            0x01
#define UART_FRAME_RSP_NOERR            0x00
#define UART_FRAME_RSP_TAGERR           0x01
#define UART_FRAME_RSP_CRCERR           0x02
#define UART_FRAME_RSP_NORSP            0x03
#define UART_FRAME_RSP_PARAMSERR        0x04
#define UART_COM_IDLE                   0x00
#define UART_COM_RS232                  0x01
#define UART_COM_TCP232                 0x02
#define UART_COM_R485                   0x03
typedef struct uartRcvFrame{
    u8 state;
    u16 index;
    u16 length;
    u8 buffer[UART_BUFFER_MAX_LEN];
    u32 idleTime;
    u8 com;
}UART_RCVFRAME;


extern UART_RCVFRAME g_sRfidRcvFrame;

void Uart_ReceiveFrame(u8 byte, UART_RCVFRAME *pRcvFrame);
#define Uart_IncIdleTime(t, rcvFrame)      do{\
                                                if((rcvFrame).state & UART_FLAG_RCV)\
                                                {\
                                                    (rcvFrame).idleTime++;\
                                                    if((rcvFrame).idleTime >= 5)\
                                                    {\
                                                        (rcvFrame).state = UART_STAT_TO;\
                                                    }\
                                                }\
                                            }while(0)

BOOL Uart_IsRcvFrameOver(UART_RCVFRAME *pRcvFrame);
#define Uart_IsRcvFrame(rcvFrame)           ((rcvFrame).state == UART_STAT_TO || (rcvFrame).state == UART_STAT_END)
#define Uart_ResetFrame(rcvFrame)           do{(rcvFrame)->state = UART_STAT_IDLE; (rcvFrame)->length = 0; (rcvFrame)->index = 0; (rcvFrame)->idleTime = 0;}while(0)


#define Gate_IsRcvFrame(rcvFrame)               ((rcvFrame).state == UART_STAT_RX_END || (rcvFrame).state == UART_STAT_RX_OVR)
#define Gate_ResetFrame(rcvFrame)           ((rcvFrame).state = UART_STAT_RX_IDLE, (rcvFrame).len = 0)
#define Uart_ResetFrame0(rcvFrame)           (memset(&(rcvFrame), 0, sizeof(UART_RCVFRAME)))

#define UART_FRAME_BROADCAST_ADDR           0xFFFF
extern UART_RCVFRAME g_sUartRcvFrame;
extern UART_RCVFRAME g_sR485RcvFrame;
u16 Uart_UsrCheckFrame(u8 *pBuffer, u16 len, u16 *pStartPos);
#endif

