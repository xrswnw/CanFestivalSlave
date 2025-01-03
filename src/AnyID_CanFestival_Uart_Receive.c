#include "AnyID_CanFestival_Uart_Receive.h"

UART_RCVFRAME g_sUartRcvFrame = {0};

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
                pRcvFrame->length = (byte & 0xFF) + 3;
                pRcvFrame->buffer[2] = byte;
                pRcvFrame->index = 3;
            }
            else if(byte == 0)
            {
                pRcvFrame->state = UART_STAT_DATA;
                pRcvFrame->length = UART_FRAME_DATA_MIN_LEN + 3;
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
                if(pRcvFrame->index == 9)
                {
                    pRcvFrame->length += byte;
                }
                else if(pRcvFrame->index == 10)
                {
                    pRcvFrame->length += (byte << 8);
                    pRcvFrame->length += 2;
                    if(pRcvFrame->length > UART_BUFFER_MAX_LEN)
                    {
                        pRcvFrame->state = UART_STAT_IDLE;
                    }
                }
            }
            pRcvFrame->buffer[pRcvFrame->index++] = byte;
            {
                if(pRcvFrame->index >= pRcvFrame->length)
                {
                    pRcvFrame->state = UART_STAT_END;
                }
            }
            break;
    }

    pRcvFrame->idleTime = 0;
}

u16 Uart_UsrCheckFrame(u8 *pBuffer, u16 len, u16 *pStartPos)
{
    u16 pos = 0;
    u16 frameLen = 0;
    if(len >= UART_FRAME_MIN_LEN)
    {
        while(pos + UART_FRAME_MIN_LEN <= len)
        {
            if(pBuffer[pos + 0] == UART_FRAME_FLAG_HEAD1 && pBuffer[pos + 1] == UART_FRAME_FLAG_HEAD2)
            {
                u16 crc1 = 0, crc2 = 0;
                frameLen = 0;
                if(pBuffer[pos + UART_FRAME_POS_LEN])
                {
                    frameLen = pBuffer[pos + UART_FRAME_POS_LEN] + 3;
                }
                else
                {
                    frameLen = pBuffer[pos + UART_FRAME_POS_PAR + 1];
                    frameLen <<= 8;
                    frameLen |= pBuffer[pos + UART_FRAME_POS_PAR + 0];

                    frameLen += UART_FRAME_MIN_LEN + 2;
                }

                crc1 = a_GetCrc(pBuffer + pos + UART_FRAME_POS_LEN, frameLen - 4);
                crc2 = pBuffer[pos + frameLen - 1];
                crc2 <<= 8;
                crc2 |= pBuffer[pos + frameLen - 2];
                if(crc1 == crc2)
                {
                    break;
                }
                else
                {
                    frameLen = 0;
                }
            }
            pos++;
        }
    }

    return frameLen;
}
