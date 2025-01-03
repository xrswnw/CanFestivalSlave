#include "AnyID_Boot_Can_HL.h"

const PORT_INF CAN_RX_PORT = {GPIOA, GPIO_Pin_11};
const PORT_INF CAN_TX_PORT = {GPIOA, GPIO_Pin_12};

CAN_FRAME g_sCanFrame = {0};
CanRxMsg g_sRcvTempMsg = {0};

BOOL Can_InitInterface(u16 psc)
{
	BOOL bOk = FALSE;
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	CAN_InitTypeDef  CAN_InitStructure = {0};
	CAN_FilterInitTypeDef CAN_FilterInitStructure = {0};
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PORT.Pin;
	
	GPIO_Init(CAN_RX_PORT.Port, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = CAN_TX_PORT.Pin;
	
	GPIO_Init(CAN_TX_PORT.Port, &GPIO_InitStructure);

	CAN_InitStructure.CAN_ABOM = DISABLE;			
	CAN_InitStructure.CAN_AWUM = DISABLE;   			
	CAN_InitStructure.CAN_NART = ENABLE;			
	CAN_InitStructure.CAN_RFLM = DISABLE;   			
	CAN_InitStructure.CAN_TTCM = DISABLE;			
	CAN_InitStructure.CAN_TXFP = DISABLE;   			
	CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;	
	CAN_InitStructure.CAN_SJW =  CAN_SJW_2tq;	
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_Prescaler = psc;
	
	if(CAN_Init(CAN_HARDPORT, &CAN_InitStructure) == CANINITOK)		
	{
		CAN_FilterInitStructure.CAN_FilterNumber = 0;  					
		CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;   
		CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;   
		CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;   			
		CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;   			
		CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;  
		CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;  	
		CAN_FilterInit(&CAN_FilterInitStructure);  	
		
		CAN_ITConfig(CAN_HARDPORT, CAN_IT_FMP0, ENABLE);
        //CAN_HARDPORT->RF0R |= CAN_RF0R_RFOM0;
		
		NVIC_InitStructure.NVIC_IRQChannel = CAN_INT_CHANNEL;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	else
	{
		bOk = TRUE;
	}
	
	return bOk;
}


u8 CanSend(CanTxMsg *txMessage)
{
	u8 mailBox = 0, relust = CANTXFAILED;

	g_sCanFrame.waitTimes = 0;
	mailBox = CAN_Transmit(CAN_HARDPORT, txMessage);   						// 发送报文,CAN组网需要接收方和发送方同时存在，才能确认数据传输成功   等待发送结束
	
    while(CAN_TransmitStatus(CAN_HARDPORT, mailBox) != CANTXOK && 
	g_sCanFrame.waitTimes < CAN_SEND_TIMEOUT)
    {
        g_sCanFrame.waitTimes++;
    }
    
    if(g_sCanFrame.waitTimes < CAN_SEND_TIMEOUT)
    {
		relust = CANTXOK;
    }

	return relust;		
}


void Can_Receive_Msg()
{	
	// 没有接收到数据,直接退出 
	if(CAN_MessagePending(CAN_HARDPORT,CAN_FIFO0))
	{
		CAN_Receive(CAN_HARDPORT, CAN_FIFO0, &g_sRcvTempMsg);   // 读取数据		
		a_SetState(g_sCanFrame.state, CAN_STATE_RX_OVER);
	}
}


void Can_FormatFrame(u8 len, u8 *code)
{
	g_sCanFrame.txMasg.ExtId = g_sCanFrame.rxMasg.ExtId;
	g_sCanFrame.txMasg.IDE = g_sCanFrame.rxMasg.IDE;
	g_sCanFrame.txMasg.RTR = g_sCanFrame.rxMasg.RTR;
	
	memset(g_sCanFrame.txMasg.Data, 0, 8);
	memcpy(g_sCanFrame.txMasg.Data, code, len);
	g_sCanFrame.txMasg.DLC = 8;
}


