#ifndef _ANYID_CAN_HL_H
#define _ANYID_CAN_HL_H

#include "AnyId_FR321CO_Slave.h"		//协议栈中直接映射此文件部分函数	

#define CAN_HARDPORT				CAN1
#define CAN_INT_CHANNEL				USB_LP_CAN1_RX0_IRQn
#define CAN_IRQHandler   			USB_LP_CAN1_RX0_IRQHandler
	
#define CAN_SEND_TIMEOUT    			0x00000000FF * 2.5						//超时时间是否以最低速为基准值？？？？？？？？？？

#define CAN_SENT_TIMEOUT_TICK			100

#define CAN_HARD_REAST_MAX_TICK			10
#define CAN_RGE_ESR_BOFF_ON				0x00000004			//设备can离线？？设备自动离开网络

#define CAN_STATE_IDLE					0x00
#define CAN_STATE_RX_OVER				0x01
#define CAN_STATE_ERR					0x02
#define CAN_STATE_RX_CMD				0x08
#define CAN_STATE_TX					0x10

typedef struct canFrame{
	u8 state;
	u8 repat;
	u32 waitTimes;
	CanTxMsg txMasg;
	Message rxFrame;
}CAN_FRAME;

extern CAN_FRAME g_sCanFrame;

BOOL Can_InitInterface(CAN_InitTypeDef *pCanPara, CAN_FilterInitTypeDef *pCanFilterPara);
u8 canSend(CAN_PORT notused, Message *TxMessage);
void Can_Receive_Msg();

#endif 

