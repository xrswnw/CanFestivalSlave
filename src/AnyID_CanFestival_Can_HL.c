#include "AnyID_CanFestival_Can_HL.h"

const PORT_INF CAN_RX_PORT = {GPIOA, GPIO_Pin_11};
const PORT_INF CAN_TX_PORT = {GPIOA, GPIO_Pin_12};

CAN_FRAME g_sCanFrame = {0};

BOOL Can_InitInterface(u8 psc)
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
	//can硬件参数
	//v 典型值：1000K、800k、500K、250k、125k、50k、20k、10k
	//v = PLCK1 / (psc * (tbs1 + 1 + tbs2 + 1 + 1)) = 60/(psc *(1 + 1 + 1 + 1+ 1)) 。 默认 500k
    CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;
	CAN_InitStructure.CAN_Prescaler = psc;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_ABOM = ENABLE;   								        // 硬件层自动复位
	CAN_InitStructure.CAN_AWUM = DISABLE;   								// 睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART = DISABLE;   								    // 使用报文自动传送 
	CAN_InitStructure.CAN_RFLM = DISABLE;   								// 报文不锁定,新的覆盖旧的 
	CAN_InitStructure.CAN_TTCM = DISABLE;									// 非时间触发通信模式  
	CAN_InitStructure.CAN_TXFP = DISABLE;   								// 优先级由报文标识符决定 
	
	if(CAN_Init(CAN_HARDPORT, &CAN_InitStructure) == CANINITOK)		
	{ 	
        CAN_FilterInitStructure.CAN_FilterNumber = 0;                               // 过滤器0	
        CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;                          // 32位ID
        CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;                      // 32位MASK
        CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;   
        CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;             // 掩码模式
        CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;            // 32位 
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;         // 过滤器0关联到FIFO0
        CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;				        // 激活过滤器0

		CAN_FilterInit(&CAN_FilterInitStructure);  	
		
		CAN_ITConfig(CAN_HARDPORT, CAN_IT_FMP0 | CAN_IT_ERR, ENABLE);                            //清除上电置位FLAG
		
		NVIC_InitStructure.NVIC_IRQChannel = CAN_INT_CHANNEL;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	else
	{
		bOk = TRUE;
	}
	
	return bOk;
}

void Can_Delayms(u32 n)
{
    n *= 0x3800;
    n++;
    while(n--);
}


void Can_InitFloating()
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PORT.Pin | CAN_TX_PORT.Pin;
	
	GPIO_Init(CAN_RX_PORT.Port, &GPIO_InitStructure);	
}

u8 canSend(CAN_PORT notused, Message *txMessage)
{
	u8 mailBox = 0, relust = CANTXFAILED;
    u8 errTick = 0;
    u32 waitTimes = 0;
    CanTxMsg txMasg = {0};

	txMasg.StdId = txMessage->cob_id;   									// 标准标识符
	txMasg.IDE = CAN_ID_STD;   												// 使用标准标识符
	txMasg.RTR = (txMessage->rtr == CAN_RTR_DATA ? 0 : 1);	
	txMasg.DLC = txMessage->len;
	memcpy(txMasg.Data, txMessage->data, txMessage->len);
	mailBox = CAN_Transmit(CAN_HARDPORT, &txMasg);   						// 发送报文,CAN组网需要接收方和发送方同时存在，才能确认数据传输成功   120电阻影响打
	// 等待发送结束
    while(CAN_TransmitStatus(CAN_HARDPORT, mailBox) != CANTXOK && 
	waitTimes < CAN_SEND_TIMEOUT)
    {
        waitTimes++;
    }
    
    if(waitTimes >= CAN_SEND_TIMEOUT)
    {
        do{
            errTick ++;
            canSendRepat(notused, txMessage);
            Can_Delayms(1);
        }while(relust == CANTXFAILED && (errTick < 3));
    }
    
	return relust;		
}

u8 canSendRepat(CAN_PORT notused, Message *txMessage)
{
	u8 mailBox = 0, relust = CANTXFAILED;
    u32 waitTimes = 0;
    CanTxMsg txMasg = {0};

	txMasg.StdId = txMessage->cob_id;   									// 标准标识符
	txMasg.IDE = CAN_ID_STD;   												// 使用标准标识符
	txMasg.RTR = (txMessage->rtr == CAN_RTR_DATA ? 0 : 1);	
	txMasg.DLC = txMessage->len;
	memcpy(txMasg.Data, txMessage->data, txMessage->len);
	mailBox = CAN_Transmit(CAN_HARDPORT, &txMasg);   						// 发送报文,CAN组网需要接收方和发送方同时存在，才能确认数据传输成功   
    while(CAN_TransmitStatus(CAN_HARDPORT, mailBox) != CANTXOK && 
	waitTimes < CAN_SEND_TIMEOUT)
    {
        waitTimes++;
    }
    
    if(waitTimes < CAN_SEND_TIMEOUT)
    {
        relust = CANTXOK;
    }
	
	return relust;		
}
	
u8 CanRspBuf(CAN_FRAME *pFrame)
{
	//u8 mailBox = 0, relust = CANTXFAILED;


	return 0;		
}
