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
	//canӲ������
	//v ����ֵ��1000K��800k��500K��250k��125k��50k��20k��10k
	//v = PLCK1 / (psc * (tbs1 + 1 + tbs2 + 1 + 1)) = 60/(psc *(1 + 1 + 1 + 1+ 1)) �� Ĭ�� 500k
    CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;
	CAN_InitStructure.CAN_Prescaler = psc;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_ABOM = ENABLE;   								        // Ӳ�����Զ���λ
	CAN_InitStructure.CAN_AWUM = DISABLE;   								// ˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
	CAN_InitStructure.CAN_NART = DISABLE;   								    // ʹ�ñ����Զ����� 
	CAN_InitStructure.CAN_RFLM = DISABLE;   								// ���Ĳ�����,�µĸ��Ǿɵ� 
	CAN_InitStructure.CAN_TTCM = DISABLE;									// ��ʱ�䴥��ͨ��ģʽ  
	CAN_InitStructure.CAN_TXFP = DISABLE;   								// ���ȼ��ɱ��ı�ʶ������ 
	
	if(CAN_Init(CAN_HARDPORT, &CAN_InitStructure) == CANINITOK)		
	{ 	
        CAN_FilterInitStructure.CAN_FilterNumber = 0;                               // ������0	
        CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;                          // 32λID
        CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;                      // 32λMASK
        CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;   
        CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;             // ����ģʽ
        CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;            // 32λ 
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;         // ������0������FIFO0
        CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;				        // ���������0

		CAN_FilterInit(&CAN_FilterInitStructure);  	
		
		CAN_ITConfig(CAN_HARDPORT, CAN_IT_FMP0 | CAN_IT_ERR, ENABLE);                            //����ϵ���λFLAG
		
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

	txMasg.StdId = txMessage->cob_id;   									// ��׼��ʶ��
	txMasg.IDE = CAN_ID_STD;   												// ʹ�ñ�׼��ʶ��
	txMasg.RTR = (txMessage->rtr == CAN_RTR_DATA ? 0 : 1);	
	txMasg.DLC = txMessage->len;
	memcpy(txMasg.Data, txMessage->data, txMessage->len);
	mailBox = CAN_Transmit(CAN_HARDPORT, &txMasg);   						// ���ͱ���,CAN������Ҫ���շ��ͷ��ͷ�ͬʱ���ڣ�����ȷ�����ݴ���ɹ�   120����Ӱ���
	// �ȴ����ͽ���
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

	txMasg.StdId = txMessage->cob_id;   									// ��׼��ʶ��
	txMasg.IDE = CAN_ID_STD;   												// ʹ�ñ�׼��ʶ��
	txMasg.RTR = (txMessage->rtr == CAN_RTR_DATA ? 0 : 1);	
	txMasg.DLC = txMessage->len;
	memcpy(txMasg.Data, txMessage->data, txMessage->len);
	mailBox = CAN_Transmit(CAN_HARDPORT, &txMasg);   						// ���ͱ���,CAN������Ҫ���շ��ͷ��ͷ�ͬʱ���ڣ�����ȷ�����ݴ���ɹ�   
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
