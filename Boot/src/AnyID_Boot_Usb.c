#include "AnyID_Boot_Usb.h"

u32 g_nUsbState = USB_STAT_IDLE;
USB_RXFRAME g_sUsbRxFrame = {0};

const PORT_INF USB_PORT = {GPIOA, GPIO_Pin_11 | GPIO_Pin_12};
void USB_InitInterface(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin = USB_PORT.Pin;
    GPIO_Init(USB_PORT.Port, &GPIO_InitStructure);
}

void USB_ConfigInt(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

BOOL USB_CheckFrame(u8 *pFrame, u8 len)
{
    u8 frameLen = 0;
    u8 pos = 0;
    BOOL b = FALSE;
    while(pos < len)
    {
        if(pFrame[pos++] == USB_FRAME_HEAD1_FLAG)
        {
            if(pFrame[pos++] == USB_FRAME_HEAD2_FLAG)
            {
                frameLen = pFrame[pos];     //不包含长度字节，但是包含CRC字节
                if(frameLen + pos < len)
                {
                    u16 crc1 = 0, crc2 = 0;
                    
                    crc2 = (pFrame[pos + frameLen] << 8) | (pFrame[pos + frameLen - 1] << 0);
                    crc1 = a_GetCrc(pFrame + pos, frameLen - 1); //算上长度字节，但是剔除CRC两个字节
                    if(crc2 == crc1)
                    {
                        b = TRUE;
                        break;
                    }
                }
            }
        }
    }
    
    return b;
}

u16 USB_ProcessFrame(u8 *pFrame, u16 len)
{
    u16 frameLen = 0;
    u16 sectorLen = 0;
    u16 pos = 0;
    while(pos < len)
    {
        sectorLen = pFrame[pos];
        memcpy(pFrame + frameLen, pFrame + pos + 1, sectorLen);
        frameLen += sectorLen;
        pos += USB_FRAME_LEN;
    }
    
    return frameLen;
}

void USB_RxFrame(void)
{
    //空闲状态或接收状态才可以接收数据
    if(a_CheckStateBit(g_nUsbState, USB_STAT_RX | USB_STAT_IDLE))
    {
        //u8 usbBuffer[USB_FRAME_LEN] = {0};
        //PMAToUserBufferCopy(usbBuffer, ENDP1_RXADDR, USB_FRAME_LEN);
        PMAToUserBufferCopy(g_sUsbRxFrame.frame + g_sUsbRxFrame.len, ENDP1_RXADDR, USB_FRAME_LEN);

        //不完整表示发送完成
        if(g_sUsbRxFrame.frame[g_sUsbRxFrame.len] < (USB_FRAME_LEN - 1))
        {
            a_SetStateBit(g_nUsbState, USB_STAT_OP);
        }
        //继续接收
        else
        {
            g_sUsbRxFrame.time = g_nSysTick;
            a_SetStateBit(g_nUsbState, USB_STAT_RX);
        }
        
        g_sUsbRxFrame.len += USB_FRAME_LEN;
        if(g_sUsbRxFrame.len > USB_FRAME_BUF_MAX_LEN)
        {
            g_sUsbRxFrame.len = USB_FRAME_BUF_MAX_LEN;
            a_SetStateBit(g_nUsbState, USB_STAT_OP);
        }
    }
    
    SetEPRxStatus(ENDP1, EP_RX_VALID);
}

void USB_TxFrame(u8 *pFrame, u16 len)
{
    u16 pos = 0;
    a_SetStateBit(g_nUsbState, USB_STAT_TX);
    while(len > 0)
    {
        u8 usbBuffer[USB_FRAME_LEN] = {0};
        while(GetEPTxStatus(ENDP2) == EP_TX_VALID);
        if(len > (USB_FRAME_LEN - 1))
        {
            usbBuffer[0] = USB_FRAME_LEN - 1;
            memcpy(usbBuffer + 1, pFrame + pos, USB_FRAME_LEN - 1);
            UserToPMABufferCopy(usbBuffer, ENDP2_TXADDR, USB_FRAME_LEN);
            len -= (USB_FRAME_LEN - 1);
            pos += (USB_FRAME_LEN - 1);
        }
        else
        {
            usbBuffer[0] = len;
            memcpy(usbBuffer + 1, pFrame + pos, len);
            UserToPMABufferCopy(usbBuffer, ENDP2_TXADDR, USB_FRAME_LEN);
            len = 0;
            pos += len;
        }
        SetEPTxValid(ENDP2);
        USB_Delayms(5);
    }
    a_SetStateBit(g_nUsbState, USB_STAT_IDLE);
}

void USB_SendUid(u8 len, u8 *pUid)
{
    u8 usbHid[64] = {0};
    usbHid[2] = 0x15;

    USB_TxFrame(usbHid, 64);
     usbHid[2] = 0;

    USB_TxFrame(usbHid, 64);
}

void USB_Delayms(u32 n)             //系统延时n毫秒
{
    n *= 0x3800;
    n++;
    while(n--);
}


