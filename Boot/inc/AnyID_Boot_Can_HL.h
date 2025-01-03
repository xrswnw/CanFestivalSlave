#ifndef _ANYID_BOOT_CAN_HL_H
#define _ANYID_BOOT_CAN_HL_H

#include "AnyID_Boot_Config.h"

#define CAN_HARDPORT				CAN1
#define CAN_INT_CHANNEL				USB_LP_CAN1_RX0_IRQn
#define CAN_IRQHandler   			USB_LP_CAN1_RX0_IRQHandler
	
#define CAN_SEND_TIMEOUT    			0x000000FFFF * 2.5						//超时时间是否以最低速为基准值？？？？？？？？？？

#define CAN_SENT_TIMEOUT_TICK			100

#define CAN_HARD_REAST_MAX_TICK			10
#define CAN_RGE_ESR_BOFF_ON				0x00000004			//设备can离线？？设备自动离开网络

#define CAN_STATE_IDLE					0x00
#define CAN_STATE_RX_OVER				0x01
#define CAN_STATE_ERR					0x02
#define CAN_STATE_RX_CMD				0x08
#define CAN_STATE_TX					0x10

#define CAN_COM_FRAME_LEN				0x08
#define CAN_COM_FRAME_MAX_LEN			2048

#define CAN_FRAME_CODE_SUCC			0x00000000
#define CAN_FRAME_CODE_FAIL			0xFFFFFFFF

#define CAN_BUD_1000K_PSCVALUE		4
#define CAN_BUD_800K_PSCVALUE		5
#define CAN_BUD_500K_PSCVALUE		8
#define CAN_BUD_250K_PSCVALUE		16
#define CAN_BUD_125K_PSCVALUE		32
#define CAN_BUD_50K_PSCVALUE		80
#define CAN_BUD_20K_PSCVALUE		200
#define CAN_BUD_10K_PSCVALUE		400

#define CAN_OD_VALUE_SUCC			0x00000000			//有效
#define CAN_OD_VALUE_RANGE_EXCEEDED	0x06090030			//参数无效（仅下载）
#define CAN_RSP_INFO_LEN			4

#define Can_ClrInfo()				do{a_ClearStateBit(g_sCanFrame.state, CAN_STATE_RX_OVER);memcpy(&g_sCanFrame.rxMasg, &g_sRcvTempMsg, sizeof(CanRxMsg));memset(&g_sRcvTempMsg, 0, sizeof(CanRxMsg));}while(0)

typedef struct canFrame{
	u8 state;
	u8 repat;
	u8 step;													//分段传输
	u8 index;													//连续判断
	u16 lenth;
	u32 waitTimes;												
	CanTxMsg txMasg;
	CanRxMsg rxMasg;
	u8 rcvBuffer[CAN_COM_FRAME_MAX_LEN];						//缓存数据
}CAN_FRAME;

extern CAN_FRAME g_sCanFrame;
extern CanRxMsg g_sRcvTempMsg;


BOOL Can_InitInterface(u16 psc);
u8 CanSend(CanTxMsg *TxMessage);
void Can_Receive_Msg();
void Can_FormatFrame(u8 len, u8 *code);
#endif 

