#ifndef _ANYID_CANFESTIVAL_CAN_HL_H
#define _ANYID_CANFESTIVAL_CAN_HL_H

#include "slaveNormal.h"		                                                //协议栈中直接映射此文件部分函数	

#define CAN_HARDPORT				CAN1
#define CAN_INT_CHANNEL				USB_LP_CAN1_RX0_IRQn
#define CAN_IRQHandler   			USB_LP_CAN1_RX0_IRQHandler
	
#define CAN_SEND_TIMEOUT    			0x1FFF * 1						                //超时时间是否以最低速为基准值？？？？？？？？？？

#define CAN_SENT_TIMEOUT_TICK			100

#define CAN_FRAME_ID_SCREEN				0xFF00
#define CAN_FRAME_ID_CMD_SCREEN			0x00FF
#define CAN_FRAME_STD_ID				0x07
			
	
#define CAN_HARD_REAST_MAX_TICK			10
#define CAN_RGE_ESR_BOFF_ON				0x00000004			                                //设备can离线？？设备自动离开网络

#define CAN_STATE_IDLE					0x00
#define CAN_STATE_RX_OVER				0x01
#define CAN_STATE_ERR					0x02
#define CAN_STATE_RX_CMD				0x08
#define CAN_STATE_TX					0x10

#define CAN_PSC_BUD_1M                  12
#define CAN_PSC_BUD_800K                15
#define CAN_PSC_BUD_500K                24
#define CAN_PSC_BUD_250K                48
#define CAN_PSC_BUD_125K                96
#define CAN_PSC_BUD_50K                 240
#define CAN_PSC_BUD_20K                 600
#define CAN_PSC_BUD_10K                 1200

typedef struct canFrame{
	u8 state;
	u8 subIndex;
	u16 cobId;
	u16 extId;
	CanTxMsg txMasg;
	Message rxFrame;
	Message txFrame;
}CAN_FRAME;

extern CAN_FRAME g_sCanFrame;
void Can_InitFloating();

BOOL Can_InitInterface(u8 psc);
void Can_Delayms(u32 n);
u8 canSend(CAN_PORT notused, Message *TxMessage);
u8 CanRspBuf(CAN_FRAME *pFrame);
void Can_Receive_Msg();
u8 canSendRepat(CAN_PORT notused, Message *txMessage);
#endif 

