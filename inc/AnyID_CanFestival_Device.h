#ifndef ANYID_CANFESTIVAL_DEVICE_H_
#define ANYID_CANFESTIVAL_DEVICE_H_

#include "AnyID_CanFestival_FRam.h"
#include "AnyID_CanFestival_SysTick_HL.h"
#include "AnyID_CanFestival_Periph_HL.h"
#include "AnyID_CanFestival_Can_HL.h"
#include "AnyID_CanFestival_RS485.h"


extern u32 g_nDeviceState;

#define DEVICE_NMT_NODEID_NUM			127
#define DEVICE_VERSION_SIZE 			50

//典型通信参数
#define DEVICE_CAN_BUD_1000K			0x00
#define DEVICE_CAN_BUD_800K				0x01
#define DEVICE_CAN_BUD_500K				0x02
#define DEVICE_CAN_BUD_250K				0x03
#define DEVICE_CAN_BUD_125K				0x04
#define DEVICE_CAN_BUD_50K				0x05
#define DEVICE_CAN_BUD_20K				0x06
#define DEVICE_CAN_BUD_10K				0x07

#define DEVICE_CAN_BUD_1000K_PSCVALUE   4
#define DEVICE_CAN_BUD_800K_PSCVALUE    5
#define DEVICE_CAN_BUD_500K_PSCVALUE    8
#define DEVICE_CAN_BUD_250K_PSCVALUE    16
#define DEVICE_CAN_BUD_125K_PSCVALUE    32
#define DEVICE_CAN_BUD_50K_PSCVALUE     80
#define DEVICE_CAN_BUD_20K_PSCVALUE     200
#define DEVICE_CAN_BUD_10K_PSCVALUE     400

#define DEVICE_CANOPEN_DEFAULT_ADDR			0x00

//canopen相关--------------------------------------------------------------
#define CANOPEN_PDO_NUM                     4
#define CANOPEN_PDO_COM_PARAMS_NUM          6
#define CANOPEN_PDO_MAP_PARAMS_NUM          8

#define CANOPEN_POD_INDEX_0		        0x00
#define CANOPEN_POD_INDEX_1		        0x01
#define CANOPEN_POD_INDEX_2		        0x02
#define CANOPEN_POD_INDEX_3		        0x03

#define CANOPEN_POD_MAP_INDEX_0		0x00
#define CANOPEN_POD_MAP_INDEX_1		0x01
#define CANOPEN_POD_MAP_INDEX_2		0x02
#define CANOPEN_POD_MAP_INDEX_3		0x03
#define CANOPEN_POD_MAP_INDEX_4		0x04
#define CANOPEN_POD_MAP_INDEX_5		0x05
#define CANOPEN_POD_MAP_INDEX_6		0x06
#define CANOPEN_POD_MAP_INDEX_7		0x07

#define CANOPEN_COB_ID_TPDO1        0x180
#define CANOPEN_COB_ID_TPDO2        0x280
#define CANOPEN_COB_ID_TPDO3        0x380
#define CANOPEN_COB_ID_TPDO4        0x480

#define CANOPEN_COB_ID_RPDO1        0x200
#define CANOPEN_COB_ID_RPDO2        0x300
#define CANOPEN_COB_ID_RPDO3        0x400
#define CANOPEN_COB_ID_RPDO4        0x500

#define CANOPEN_PDO_TRANSIONTYPE_ACYCLIC_SYNCHRONIZATION    					0x00	//非循环同步      异步输出，即接受到发送请求或者主动发送(不定时)
#define CANOPEN_PDO_TRANSIONTYPE_LOOP_SYNCHRONIZATION_1     					0x01	//循环同步一次      sync个数发送  
#define CANOPEN_PDO_TRANSIONTYPE_REMOTE_SYNCHRONIZATION   						0xFC	//远程同步          接收远程请求并SYBC
#define CANOPEN_PDO_TRANSIONTYPE_REMOTE_ASYNCHRONOUS   							0xFD	//远程异步          接收远程请求立即发送
#define CANOPEN_PDO_TRANSIONTYPE_MAKER_SPECIFIC_ASYNCHRONOUS   					0xFE	//制造商特定异步     由sendPDOevent（）监控，数据发生变更发送（可定时限制）
#define CANOPEN_PDO_TRANSIONTYPE_DEVICE_SUBPROTOCOL_SPECIFIC_ASYNCHRONOUS 		0xFF	//设备子协议异步    事件变化，变化时立即上报，否则按照事件事件上报   

#define CANOPEN_PDO_INHIBIT_TIME		0x00
#define CANOPEN_PDO_COMRNTRY     		0x00
#define CANOPEN_PDO_EVENT_TIME     		0x00
#define CANOPEN_PDO_SYNC_VALUE     		0x00

#define CAMOPEN_VERSION_LEN				10
typedef struct periphParams
{
	u32 deviceType;
	u8 errReg;
	u32 errFiled;
	u32 syncCobId;
	u32 cycleCom;
	u32 syncWindowsLenth;
	u8 deviceName[10];
	u8 hardVersion[10];
	u8 softVersion[10];
	u16 guardTime;
	u8 lifeTime;
	u32 timeCobId;
	u32 timesTMap;
	u32 emncCobId;
	u32 producerTime;
}PERIPH_PARAMS;

typedef struct pdoComParams
{
    u8 subNum;                          //参数条目数量    subIndexNum - 1
    u32 cobId;                          //发送/接收此PDO的帧ID
    u8 transmissionType;                //传输类型
    u16 inhibitTime;                    //生产禁止约束时间， 0.1ms
    u8 compatibilityEntry;              //兼容性？？？大多不使用
    u16 eventTimer;                    //事件触发时间             ms 
    u8 syncValue;                      //同步起始值
}PDO_COMPARAMS;

typedef struct pdoMapParams
{
    u8 subNum;                     //参数条目
    u32 obData[CANOPEN_PDO_MAP_PARAMS_NUM];
}PDO_MAPPARAMS;

typedef struct pdoParams
{
    PDO_COMPARAMS comParams;
    PDO_MAPPARAMS mapParams;
}DEVICE_PDOPARAMS;

#define DEVICE_TPOD_MAX_NUM                     12
#define DEVICE_RPOD_MAX_NUM                     4
typedef struct deviceCanOpenParams
{
	PERIPH_PARAMS periphParams;
    DEVICE_PDOPARAMS rPdoParams[DEVICE_RPOD_MAX_NUM];
    DEVICE_PDOPARAMS tPdoParams[DEVICE_TPOD_MAX_NUM];
}DEVICE_CANOPENPARAMS;

#define Device_ChkCallBack(value)		({\
                                            u8 result = 0;\
                                            if(value == OD_SUCCESSFUL)\
                                            {\
                                               result = 1;\
                                            }\
                                            (result);\
                                          })

//-----------------------------------------------------------------
typedef struct wokeInfo{
    u8 type;
}WORK_INFO;

typedef struct pemaceInfo{
    u8 pwr;
}PEMACE_INFO;

typedef struct rfWokeInfo{
    u8 mode;
}RF_WOKEINFO;

typedef struct comInfo{
    u32 mode;
}COM_INFO;

typedef struct reportInfo{
    u32 keepTime;
}REPORT_INFO;

typedef struct recordInfo{
    u8 mode;
}RECORD_INFO;

typedef struct infraredInfo{
    u8 mode;
}INFRARED_INFO;

typedef struct ioInfo{
    u8 mode;
}IO_INFO;

#define DEVICE_SUBDEVICE_RF_MAX_NUM             4
#define DEVICE_SUBDEVICE_MX_MAX_NUM             12

typedef struct subDeviceInfo{
    u8 rfNum;
    u8 mxNum;
    u8 rfId[DEVICE_SUBDEVICE_RF_MAX_NUM];
    u8 mxId[DEVICE_SUBDEVICE_MX_MAX_NUM];
    u32 rfu;
}SUBDEVICE_INFO;






//
typedef struct deviceParamenter{
	u8 addr;
	u8 bud;
	u16 flag;
    DEVICE_CANOPENPARAMS canOpneParams;
    u32 rfu;
	u32 crc;
}DEVICE_PARAMS;
extern DEVICE_PARAMS g_sDeviceParams;
                                       
                                                            
#define DEVICE_FRAME_BROADCAST_ADDR             0xFFFF

#define READER_RSPFRAME_LEN                 UART_BUFFER_MAX_LEN

#define READER_COM_TYPE_485                     0x01
typedef struct readerRspFrame{
    u16 len;
    u8 buffer[READER_RSPFRAME_LEN];
    u8 flag;
    u8 err;
    u16 destAddr;
    u8 cmd;
    u8 comType;
}READER_RSPFRAME;
extern READER_RSPFRAME g_sReaderRspFrame;

void Device_Delayms();
void Device_Delay100us(u32 t);
void Device_Init();

void Device_ReadDeviceParamenter();
BOOL Device_WriteDeviceParamenter(void);	
void Device_ResetDeviceParamenter();

u16 Reader_ProcessUartFrame(u8 *pFrame, u16 len);
u16 Device_ResponseFrame(u8 *pParam, u16 len, READER_RSPFRAME *pRspFrame);
void Device_CanopenParamsUpDate(DEVICE_CANOPENPARAMS *pParams);
u8 Device_CanProcessFrame(Message *pMassage);

u16 Device_BudGet(u8 bud);
u32 Device_CanFestivalCallBackInit(void);



//----------------
void Device_LedTask();
#endif