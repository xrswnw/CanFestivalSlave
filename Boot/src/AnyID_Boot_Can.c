#include "AnyID_FR321CO_Can_HL.h"

const PORT_INF CAN_RX_PORT = {GPIOA, GPIO_Pin_11};
const PORT_INF CAN_TX_PORT = {GPIOA, GPIO_Pin_12};

CAN_FRAME g_sCanFrame = {0};
BOOL Can_InitInterface(CAN_InitTypeDef *pCanPara, CAN_FilterInitTypeDef *pCanFilterPara)
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
	CAN_InitStructure.CAN_ABOM = pCanPara->CAN_ABOM;			
	CAN_InitStructure.CAN_AWUM = pCanPara->CAN_AWUM;   			
	CAN_InitStructure.CAN_NART = pCanPara->CAN_NART;			
	CAN_InitStructure.CAN_RFLM = pCanPara->CAN_RFLM;   			
	CAN_InitStructure.CAN_TTCM = pCanPara->CAN_TTCM;			
	CAN_InitStructure.CAN_TXFP = pCanPara->CAN_TXFP;   			
	CAN_InitStructure.CAN_BS1 = pCanPara->CAN_BS1;
	CAN_InitStructure.CAN_BS2 = pCanPara->CAN_BS2;	
	CAN_InitStructure.CAN_SJW =  pCanPara->CAN_SJW;	
	CAN_InitStructure.CAN_Mode = pCanPara->CAN_Mode;
	CAN_InitStructure.CAN_Prescaler = pCanPara->CAN_Prescaler;
	
	if(CAN_Init(CAN_HARDPORT, &CAN_InitStructure) == CANINITOK)		
	{
		CAN_FilterInitStructure.CAN_FilterNumber = pCanFilterPara->CAN_FilterMode;  					
		CAN_FilterInitStructure.CAN_FilterMode = pCanFilterPara->CAN_FilterMode;   
		CAN_FilterInitStructure.CAN_FilterScale = pCanFilterPara->CAN_FilterScale;   
		CAN_FilterInitStructure.CAN_FilterIdHigh = pCanFilterPara->CAN_FilterIdHigh;   			
		CAN_FilterInitStructure.CAN_FilterIdLow = pCanFilterPara->CAN_FilterIdLow;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh = pCanFilterPara->CAN_FilterMaskIdHigh;   			
		CAN_FilterInitStructure.CAN_FilterMaskIdLow = pCanFilterPara->CAN_FilterMaskIdLow;
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment = pCanFilterPara->CAN_FilterFIFOAssignment;  
		CAN_FilterInitStructure.CAN_FilterActivation = pCanFilterPara->CAN_FilterActivation;  	
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


u8 canSend(CAN_PORT notused, Message *TxMessage)
{
	u8 mailBox = 0, relust = CANTXFAILED;

	g_sCanFrame.waitTimes = 0;
	g_sCanFrame.txMasg.StdId = TxMessage->cob_id;   									// 标准标识符
	g_sCanFrame.txMasg.IDE = CAN_ID_STD;   												// 使用标准标识符
	g_sCanFrame.txMasg.RTR = (TxMessage->rtr == CAN_RTR_DATA ? 0 : 1);	
	g_sCanFrame.txMasg.DLC = TxMessage->len;
	memcpy(g_sCanFrame.txMasg.Data, TxMessage->data, TxMessage->len);
	mailBox = CAN_Transmit(CAN_HARDPORT, &g_sCanFrame.txMasg);   						// 发送报文,CAN组网需要接收方和发送方同时存在，才能确认数据传输成功   
	// 等待发送结束
    while(CAN_TransmitStatus(CAN_HARDPORT, mailBox) != CANTXOK && 
	g_sCanFrame.waitTimes < CAN_SEND_TIMEOUT)
    {
        g_sCanFrame.waitTimes++;
    }
    
    if(g_sCanFrame.waitTimes < CAN_SEND_TIMEOUT)
    {
		relust = CANTXOK;
		g_sCanFrame.repat = 0;
		if(a_CheckStateBit(g_sCanFrame.state, CAN_STATE_ERR))
		{
			a_ClearStateBit(g_sCanFrame.state, CAN_STATE_ERR);		//重试次数？？？？外接指示
		}
    }

	/*
	while(relust = CAN_TransmitStatus(CAN_HARDPORT, mailBox) != CANTXOK && g_sCanFrame.waitTimes + CAN_SEND_TIMEOUT <= g_nSysTick);
		
	if(g_sCanFrame.waitTimes + CAN_SEND_TIMEOUT > g_nSysTick)
	{
		relust = CANTXOK;
	}
	*/
	return relust;		
}


void Can_Receive_Msg()
{	
	CanRxMsg rxMsg = {0};
	
	// 没有接收到数据,直接退出 
	if(CAN_MessagePending(CAN_HARDPORT,CAN_FIFO0))
	{
		
		CAN_Receive(CAN_HARDPORT, CAN_FIFO0, &rxMsg);   // 读取数据	
		g_sCanFrame.rxFrame.cob_id = rxMsg.StdId;
		g_sCanFrame.rxFrame.rtr = rxMsg.RTR == (CAN_RTR_DATA ? 0 : 1);
		g_sCanFrame.rxFrame.len = rxMsg.DLC;
		a_SetState(g_sCanFrame.state, CAN_STATE_RX_OVER);
		memcpy(g_sCanFrame.rxFrame.data, rxMsg.Data, rxMsg.DLC);	
	}
	/*
	Message rxMsg;
	// 没有接收到数据,直接退出 	
	
	if(CAN_MessagePending(CAN_HARDPORT,CAN_FIFO0))							//上电后第一帧数据判定失败，若不加校验，是否引起其它问题
	{
		CAN_Receive(CAN_HARDPORT, CAN_FIFO0, &g_sCanFrame.rxMasg);   		// 读取数据	
		rxMsg.cob_id = g_sCanFrame.rxMasg.StdId;
		rxMsg.rtr = g_sCanFrame.rxMasg.RTR == (CAN_RTR_DATA ? 0 : 1);
		rxMsg.len = g_sCanFrame.rxMasg.DLC;
		memcpy(rxMsg.data, g_sCanFrame.rxMasg.Data, g_sCanFrame.rxMasg.DLC);	
		canDispatch(&AnyId_FR321CO_Slave_Data, &rxMsg);						//can帧调度函数,此处可做处理，选择是否接入Can Open
	}*/
	/*
	if(CAN_GetITStatus(CAN_HARDPORT, CAN_IT_FMP0))							//监控标志位？？？？？？？？？？？？？？？？？
	{	
		CAN_ClearITPendingBit(CAN_HARDPORT, CAN_IT_FMP0);
		CAN_Receive(CAN_HARDPORT, CAN_FIFO0, &g_sCanFrame.rxMasg);   // 读取数据	

		rxMsg.cob_id = g_sCanFrame.rxMasg.StdId;
		rxMsg.rtr = g_sCanFrame.rxMasg.RTR == (CAN_RTR_DATA ? 0 : 1);
		rxMsg.len = g_sCanFrame.rxMasg.DLC;
		memcpy(rxMsg.data, g_sCanFrame.rxMasg.Data, g_sCanFrame.rxMasg.DLC);	
		canDispatch(&AnyId_Canopen_Slave_Data, &rxMsg);					//can帧调度函数,此处可做处理，选择是否接入Can Open
	}*/
}



