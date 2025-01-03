#include "AnyID_Uart_Receive.h"
UART_RCVFRAME g_sUartTempRcvFrame = {0};

void Uart_ReceiveFrame(u8 byte, UART_RCVFRAME *pRcvFrame)
{
    switch(pRcvFrame->state)
    {
        case UART_STAT_IDLE:
            if(byte == UART_FRAME_FLAG_HEAD1)
            {
                pRcvFrame->state = UART_STAT_HEAD1;
                pRcvFrame->buffer[0] = UART_FRAME_FLAG_HEAD1;
            }
            break;
        case UART_STAT_HEAD1:
            if(byte == UART_FRAME_FLAG_HEAD2)
            {
                pRcvFrame->state = UART_STAT_HEAD2;
                pRcvFrame->buffer[1] = UART_FRAME_FLAG_HEAD2;
            }
            else if(byte == UART_FRAME_FLAG_HEAD1)
            {
                pRcvFrame->state = UART_STAT_HEAD1;
            }
            else
            {
                pRcvFrame->state = UART_STAT_IDLE;
            }
            break;
        case UART_STAT_HEAD2:
            if(UART_FRAME_DATA_MIN_LEN <= byte && byte <= UART_FRAME_DATA_MAX_LEN)
            {
                pRcvFrame->state = UART_STAT_DATA;
                pRcvFrame->dataLen = (byte & 0xFF) + 3;
                pRcvFrame->buffer[2] = byte;
                pRcvFrame->index = 3;
            }
            else if(byte == 0)
            {
                pRcvFrame->state = UART_STAT_DATA;
                pRcvFrame->dataLen = UART_FRAME_DATA_MIN_LEN + 3 + 9;
                pRcvFrame->buffer[2] = byte;
                pRcvFrame->index = 3;
            }
            else
            {
                pRcvFrame->state = UART_STAT_IDLE;
            }
            break;
        case UART_STAT_DATA:
            if(pRcvFrame->buffer[2] == 0)
            {
                if(pRcvFrame->index == 14)
                {
                    pRcvFrame->dataLen += byte;
                }
                else if(pRcvFrame->index == 15)
                {
                    pRcvFrame->dataLen += (byte << 8);
                }
                else if(pRcvFrame->index == 16)
                {
                    pRcvFrame->dataLen += (byte << 16);
                }
                else if(pRcvFrame->index == 17)
                {
                    pRcvFrame->dataLen += (byte << 24);
                }
            }
            pRcvFrame->buffer[pRcvFrame->index++] = byte;
            if(pRcvFrame->index >= pRcvFrame->dataLen)
            {
                pRcvFrame->state = UART_STAT_END;
            }
            break;
    }
    pRcvFrame->idleTime = 0;
}


BOOL Uart_IsRcvFrameOver(UART_RCVFRAME *pRcvFrame)
{
    BOOL bOver = FALSE;
    if(pRcvFrame->state == UART_STAT_TO)
    {
        if(pRcvFrame->index > UART_FRAME_DATA_MIN_LEN)
        {
            bOver = TRUE;
        }
        else
        {
            pRcvFrame->state = UART_STAT_IDLE;
        }
    }
    else if(pRcvFrame->state == UART_STAT_END)
    {
        bOver = TRUE;
    }

    return bOver;
}
