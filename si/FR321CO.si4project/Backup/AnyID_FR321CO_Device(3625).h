#ifndef ANYID_FR321CO_DEVICE_H_
#define ANYID_FR321CO_DEVICE_H_

#include "AnyID_FR321CO_FRam.h"
#include "AnyID_FR321CO_ST25R391x_HL.h"
#include "AnyID_ST25R391x_ISO15693.h"
#include "AnyID_FR321CO_SysTick_HL.h"
#include "AnyId_FR321CO_Slave.h"
#include "AnyID_FR321CO_Can_HL.h"


extern u32 g_nDeviceState;

#define DEVICE_NMT_NODEID_NUM			127

#define DEVICE_STAT_TAG_EXIXT			0x00000001
#define DEVICE_STAT_TAG_OP_ERR			0x00000002
#define DEVICE_STAT_TAG_OP_SUC			0x00000004
#define DEVICE_STAT_WORKING				0x00000008
#define DEVICE_STAT_WOKE_WRITE			0x00000010
#define DEVICE_STAT_BUSY				0x00000020
#define DEVICE_STAT_BOOT				0x00000040
#define DEVICE_STAT_REST				0x00000080

#define DEVICE_STAT_TAG_NO_EXIXT		0x00000001

#define DEVICE_STAT_ALARMLED            0x10000000
#define DEVICE_STAT_ALARMDLY			0x20000000
#define DEVICE_STAT_ALARMOFF			0x40000000
#define DEVICE_STAT_ALARMON  			0x80000000

#define Device_SetSate(sate)			(g_nDeviceState |= sate)
#define Device_ClaSate(sate)			(g_nDeviceState &= ~sate)
#define Device_ChkSate(sate)			((g_nDeviceState & sate) != 0)

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

#define DEVICE_CAN_BUD_1000K_PSCVALUE		4
#define DEVICE_CAN_BUD_800K_PSCVALUE		5
#define DEVICE_CAN_BUD_500K_PSCVALUE		8
#define DEVICE_CAN_BUD_250K_PSCVALUE		16
#define DEVICE_CAN_BUD_125K_PSCVALUE		32
#define DEVICE_CAN_BUD_50K_PSCVALUE			80
#define DEVICE_CAN_BUD_20K_PSCVALUE			200
#define DEVICE_CAN_BUD_10K_PSCVALUE			400

#define DEVICE_CANOPEN_DEFAULT_ADDR			0x00

#define DEVICE_RFID_WORK_READ_UID			0x01
#define DEVICE_RFID_WORK_READ_BLOCK			0x02
#define DEVICE_RFID_WORK_WRITE_BLOCK		0x04

#define DEVICE_PARAMS_RFU_BOOT				0x01

#define DEVICE_RFID_WORK_PROTOCOL_15693		0x01

#define DEVICE_RFID_BLOCK_NUM				8
typedef struct deviceTagInfo15693
{
	u8 uid[ISO15693_SIZE_UID];
	u8 sampleUid[ISO15693_SIZE_UID];
	u8 block[DEVICE_RFID_BLOCK_NUM * ISO15693_SIZE_BLOCK];
}DEVICE_TAGINFO15693;

typedef struct deviceRfidPars15693{
	u8 afiCtrl;
	u8 afiValue;
	u8 rfCtrl;
	u8 readAddr;
	u8 readNum;
	u8 writeAddr;
	u8 writeNum;
	u8 block[DEVICE_RFID_BLOCK_NUM * ISO15693_SIZE_BLOCK]; 
	u16 rfu1;
	u16 rfu2;
}DEVICE_RFIDPARS15693;

typedef struct deviceRfParams{
   /* u16 protocol:1; 
    u16 rBlkNum:2;                      
    u16 rfu1:13;*/  
	u8 protocol; 
    u8 rBlkNum;                      
    u8 rfu;
}DEVICE_RFPARAMS;


#define DEVICE_INVENTORY_TO_DFT         8		//盘点超时默认值20ms
#define DEVICE_INTERVAL_TIM_DFT         0		//自动操作间隔时间，默认0ms，基本射频一直开启
#define DEVICE_IDLE_TIM_DFT         	0		//空闲，默认0ms，基本射频一直开启
#define DEVICE_KEEPLIVE_TO_DFT          0		//存活时间500ms
#define DEVICE_INVENTORY_TO_TICK		5
typedef struct deviceWorkTimParaments{
    u8 invtTim;                         //盘点超时时间
    u8 idleTim;                         //盘点间隔
    u8 keepTim;                         //保持时间
    u8 rfu;   
}DEVICE_WTPARAMS;

typedef struct deviceReadBlockParams{                        
    u16 addr;                           //
    u16 len;                            //   
	u16 rfu;
}DEVICE_RBLKPARAMS;

//canopen相关--------------------------------------------------------------
#define CANOPEN_PDO_NUM      4
#define CANOPEN_PDO_COM_PARAMS_NUM  6
#define CANOPEN_PDO_MAP_PARAMS_NUM  8

#define CANOPEN_POD_INDEX_0		0x00
#define CANOPEN_POD_INDEX_1		0x01
#define CANOPEN_POD_INDEX_2		0x02
#define CANOPEN_POD_INDEX_3		0x03

#define CANOPEN_POD_MAP_INDEX_0		0x00
#define CANOPEN_POD_MAP_INDEX_1		0x01
#define CANOPEN_POD_MAP_INDEX_2		0x02
#define CANOPEN_POD_MAP_INDEX_3		0x03
#define CANOPEN_POD_MAP_INDEX_4		0x04
#define CANOPEN_POD_MAP_INDEX_5		0x05
#define CANOPEN_POD_MAP_INDEX_6		0x06
#define CANOPEN_POD_MAP_INDEX_7		0x07

#define CANOPEN_COB_ID_TPDO1    0x180
#define CANOPEN_COB_ID_TPDO2    0x280
#define CANOPEN_COB_ID_TPDO3    0x380
#define CANOPEN_COB_ID_TPDO4    0x480

#define CANOPEN_COB_ID_RPDO1    0x200
#define CANOPEN_COB_ID_RPDO2    0x300
#define CANOPEN_COB_ID_RPDO3    0x400
#define CANOPEN_COB_ID_RPDO4    0x500

#define CANOPEN_PDO_TRANSIONTYPE_ACYCLIC_SYNCHRONIZATION    					0x00	//非循环同步
#define CANOPEN_PDO_TRANSIONTYPE_LOOP_SYNCHRONIZATION_1     					0x01	//循环同步一次
#define CANOPEN_PDO_TRANSIONTYPE_REMOTE_SYNCHRONIZATION   						0xFC	//远程同步
#define CANOPEN_PDO_TRANSIONTYPE_REMOTE_ASYNCHRONOUS   							0xFD	//远程异步
#define CANOPEN_PDO_TRANSIONTYPE_MAKER_SPECIFIC_ASYNCHRONOUS   					0xFE	//制造商特定异步
#define CANOPEN_PDO_TRANSIONTYPE_DEVICE_SUBPROTOCOL_SPECIFIC_ASYNCHRONOUS 		0xFF	//设备子协议异步

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
    u8 subNum;                     //参数条目数量    subIndexNum - 1
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

typedef struct deviceCanOpenParams
{
	PERIPH_PARAMS periphParams;
    DEVICE_PDOPARAMS rPdoParams[CANOPEN_PDO_NUM];
    DEVICE_PDOPARAMS tPdoParams[CANOPEN_PDO_NUM];
}DEVICE_CANOPENPARAMS;


//-----------------------------------------------------------------
typedef struct deviceParamenter{
	u8 addr;
	u8 bud;
	u16 flag;
	u16 rfu;
	//---------------
	CAN_InitTypeDef canPara;
	CAN_FilterInitTypeDef canFilterPara;
	DEVICE_RFPARAMS rfParams;
	DEVICE_WTPARAMS workTim;
	DEVICE_RBLKPARAMS autoBlockParams;
    DEVICE_CANOPENPARAMS canOpneParams;
	u32 crc;
}DEVICE_PARAMS;
extern DEVICE_PARAMS g_sDeviceParams;

#define DEVICE_FLAG_RESET					0x01
#define DEVICE_UID_15693_LEN				8
#define DEVICE_TAG_15693_NUM				12
#define DEVICE_RFID_MASK_TICK				2

#define DEVICE_RFID_STATE_HAVE_TAG			0x01
#define DEVICE_RFID_STATE_BLOCK_READ_OK		0x02
#define DEVICE_RFID_STATE_BLOCK_WRITE_OK	0x04


#define DEVICE_TEST_MODE_ENABLE				0x01
typedef struct deviceTestInfo{
	u8 testEnable;
	u64 okTick;
	u64 allTick;
}DEVICE_TESTINFO;

#define DEVICE_BLOCK_LEN_15693			64

typedef struct tagOpInfo{
	u8 state;
	u8 op;
	u8 rspLen;
	u8 errCode;
	u16 addr;
	u16 len;
	u8 rcvBuffer[CANFESTIVAL_COM_LEN];
	u8 rspBuffer[CANFESTIVAL_COM_LEN];
}TAG_OPINFO;



#define DEVICE_RF_FLAG_CLOSE                 0x00
#define DEVICE_RF_FLAG_OPEN                  0x01
#define DEVICE_RF_FLAG_RESET                 0x02


typedef struct deviceInfo{
	u8 ctr;
	u8 flag;
	u8 rfu;
	u8 rfidFlag;
	TAG_OPINFO opInfo;
	DEVICE_TESTINFO testInfo;
}DEVICE_INFO;	
extern DEVICE_INFO g_sDeviceInfo;


#define DEVICE_OP_TAG_15693             0x00
#define DEVICE_OP_TAG_14443A             0x01

#define DEVICE_OP_MAX_NUM               16
#define DEVICE_STAT_IDLE                0x00000001
#define DEVICE_STAT_TX                  0x00000002
#define DEVICE_STAT_WAIT                0x00000004
#define DEVICE_STAT_RX                  0x00000008
#define DEVICE_STAT_STEP                0x00000010
#define DEVICE_STAT_RSP                 0x00000020
#define DEVICE_RX_RSP_TIME              20 //100ms

#define DEVICE_OP_TAG_INVT              0x0000
#define DEVICE_OP_TAG_READ              0x0001
#define DEVICE_OP_TAG_WRITE             0x0002

#define DEVICE_OPERATION_INVT			0x0000
#define DEVICE_OPERATION_READ			0x0001
#define DEVICE_OPERATION_WRITE			0x0002


#define DEVICE_RF_CLOSE                 0x00
#define DEVICE_RF_OPEN                  0x01
#define DEVICE_RF_RESET                 0x02

typedef struct deviceOperation{
    u32 delay;
    u32 state;
    union tagProtocol{
        ISO15693_INFO iso15693;
        //ISO14443A_INFO iso14443a;
    }protocol;
    u8 operation[DEVICE_OP_MAX_NUM];
    u8 step;
    u8 num;
    u8 index;
    u16 memAddr;
    u16 memLen;
    u8 *pRegValue;
    u16 opTagMode;
    BOOL bAutoOp;
    BOOL bOpOk;
}DEVICE_OP;
extern DEVICE_OP g_sDeviceOpInfo;
#define Device_ResetOp()                memset(&g_sDeviceOpInfo, 0, sizeof(DEVICE_OP))


#define DEVICE_OP_TAG_WBLK              1
#define DEVICE_OP_TAG_OVER              1
#define DEVICE_OP_TAG_ERR               1
#define DEVICE_OP_TAG_BUSY              1
#define DEVICE_OP_TAG_START             1
#define DEVICE_OP_TAG_OK                1
#define DEVICE_OP_TAG_FRAME_ERR			1
typedef struct deviceOpTagState{
    u8 tagFlag: 1;		// RFID写入    1-标签在位 0-标签离开
    u8 start: 1;		// MST写入     1-开始操作 0-停止操作
    u8 busy: 1;			// RFID写入    1-正在操作 0-操作完成
    u8 err: 1;			// RFID写入    1-操作失败 0-操作成功
    u8 over: 1;			// RFID写入    1-操作完成 0-操作未完成
    u8 rw: 1;			//	1-写Block   0-读Block
    u8 frame: 1;		// 
    u8 autoBlock: 1;	//	
}DEVICE_OPTAGSTATE;


#define DEVICE_TAG_OP_READ_UID			0x20
#define DEVICE_TAG_OP_READ_BLOCK		0x21
#define DEVICE_TAG_OP_WRITE_BLOCK		0x22


#define DEVICE_TAG_OP_RCV_LEN			16
#define DEVICE_TAG_OP_ERR_SUCC			0x00

#define MAX_PDIN_LEN					128
#define DEVICE_OPTAGINFO_HEAD_LEN       4
#define DEVICE_OPTAGINFO_MEM_LEN        (MAX_PDIN_LEN - DEVICE_OPTAGINFO_HEAD_LEN)
#pragma pack(push)			//保存对齐状态
#pragma pack(1)				//设定为1字节对齐
typedef struct deviceOpTagInfo{
    DEVICE_OPTAGSTATE state;
    u16 addr;
    u16 len;
    u8 tagMem[DEVICE_OPTAGINFO_MEM_LEN];
}DEVICE_OPTAGINFO;
#pragma pack(pop)			//恢复对齐状态
extern DEVICE_OPTAGINFO g_sDeviceOpTagInfo;

#define Device_ClrTagOpInfo(p)		do{memset((p), 0, sizeof(TAG_OPINFO));}while(0)
typedef struct deviceBackUpUid{
    u8 u[ISO15693_SIZE_UID];
    u32 keepTick;
}DEVICE_BACKUPUID;
extern DEVICE_BACKUPUID g_sDeviceBackupUid;

#define DEVICE_BLK_MEM_SIZE             4096 

#define DEVICE_RFID_OP_AUTO				0x01
#define DEVICE_RFID_OP_READ_BLOCK		0x02
#define DEVICE_RFID_OP_WRITE_BLOCK		0x04

typedef struct iso15693Info
{
	u16 addr;
	u16 len;
	u16 rfu;
	u8 uid[ISO15693_SIZE_UID];
	u8 block[DEVICE_BLK_MEM_SIZE / 4];
}PROTOCOL_15693INFO;

typedef struct deviceTagMemory{
	u8 flag;
	u8 op;
	u8 state;
    //u8 uid[ISO15693_SIZE_UID];
    u8 block[DEVICE_BLK_MEM_SIZE];
	PROTOCOL_15693INFO tag15693Info;
	TAG_OPINFO opInfo;
}DEVICE_TAGMEM;
extern DEVICE_TAGMEM g_sDeviceTagMemory;

#define DEVICE_VERSION_DEVICE_NAME_POS			0
#define DEVICE_VERSION_DEVICE_NAME_LEN			7
#define DEVICE_VERSION_DEVICE_SOFTWARE_POS		DEVICE_VERSION_DEVICE_NAME_LEN + 1
#define DEVICE_VERSION_DEVICE_SOFTWARE_LEN		8
#define DEVICE_VERSION_DEVICE_HARDWARE_POS		DEVICE_VERSION_DEVICE_SOFTWARE_POS + DEVICE_VERSION_DEVICE_SOFTWARE_LEN + 1
#define DEVICE_VERSION_DEVICE_HARDWARE_LEN		7
extern const u8 DEVICE_VERSION[DEVICE_VERSION_SIZE];


#define Device_TagFrameClear()					do{\
													DeviceParams_OpFrame1 = DeviceParams_OpFrame2 = DeviceParams_OpFrame3 = DeviceParams_OpFrame4 = 0;\
													DeviceParams_OpFrame5 = DeviceParams_OpFrame6 = DeviceParams_OpFrame7 = DeviceParams_OpFrame0 = 0;\
													}while(0)
#define Device_ParamsUpdata(u, v)						do{\
															if(u ^ v)\
															{\
																u = v;\
															}\
														  }while(0)


#define DEVICE_CAN_RCV_BUFFER_LEN				512
#define DEVICE_CAN_FLAG_RCV						0x01
#define DEVICE_CAN_FLAG_SEND					0x02
#define DEVICE_CAN_FLAG_WAIT					0x04
#define DEVICE_CAN_FLAG_RSP  					0x08
	
															  
#define DEVICE_TAG_ONE_BLOCK_LEN				0x04
															  
#define DEVICE_CAN_FRAME_OP						0x00
#define DEVICE_CAN_FRAME_ADDR					0x01
#define DEVICE_CAN_FRAME_LENTH					0x03
#define DEVICE_CAN_FRAME_DATE					0x05
	
#define DEVICE_CAN_CUSTOM_FRAME_INDEX			0x00
#define DEVICE_CAN_CUSTOM_FRAME_SUBINDEX		0x02
#define DEVICE_CAN_CUSTOM_FRAME_LEN				0x03
#define DEVICE_CAN_CUSTOM_FRAME_DATA			0x04
															  
#define DEVICE_CAN_FRAME_LEN_1					0x01
#define DEVICE_CAN_FRAME_LEN_2					0x02
#define DEVICE_CAN_FRAME_LEN_4					0x04
	
#define DEVICE_CAN_FEAME_PSPTYPE_STD			0x00
#define DEVICE_CAN_FEAME_PSPTYPE_EXT			0x01
															  
#define DEVICE_FRAME_CMD_RESET					0x804
#define DEVICE_FRAME_CMD_GET_PARAMS				0x8E0
#define DEVICE_FRAME_CMD_SET_PARAMS				0x8E1
#define DEVICE_FRAME_CMD_GET_TVER				0x8E2
#define DEVICE_FRAME_CMD_GET_SVER				0x8E3
#define DEVICE_FRAME_CMD_GET_HVER				0x8E4
				  

#define DEVICE_FRAME_CMD_ENABLE_READ_BLOCK		0xF2
#define DEVICE_FRAME_CMD_ENABLE_WRITE_BLOCK		0xF3
#define DEVICE_FRAME_CMD_CLEAR_OPFRAME			0xF4
															  
typedef struct deviceCanRcvFrame{
	u8 flag;
	u8 state;
	u8 buffer[DEVICE_CAN_RCV_BUFFER_LEN];
	u16 lenth;
	u32 time;
}DEVICE_CANRCVFRAME;

extern DEVICE_CANRCVFRAME g_sDeviceCanRcvFrame;
extern DEVICE_CANRCVFRAME g_sDeviceCanTempRcvFrame;


void Device_ReadDeviceParamenter();
void Device_Delayms();
void Device_Delay100us(u32 t);
void Device_RfCtrl(u8 mode);
void Device_RfidTask();
void Device_Init();
void Device_Iso15693AutoOpTag(DEVICE_OP *pOpInfo);
void Device_AutoOpTag(DEVICE_OP *pOpInfo);
void Device_Tranceive(DEVICE_OP *pOpInfo);
void Device_Iso15693SaveUid(u8 *pUid, DEVICE_TAGMEM *pTagMem);
void Device_Iso15693FmtBlockSize(ISO15693_INFO *pInfo);
void Device_Iso15693MergeBlock(DEVICE_OP *pDeviceOp);
void Device_Iso15693GetReadBlockAddr(ISO15693_BLOCK *pBlockInfo, u16 memAddr, u16 memLen);
void Device_Iso15693GetWriteBlockAddr(ISO15693_BLOCK *pBlockInfo, u16 memAddr, u16 memLen);
void ISO15693_AddContinueAddr(u8 *pAddr, u16 start, u16 num);
void Device_UpdateBackupTagInfo(DEVICE_OP *pOpInfo);
void Device_UpdateOpTagInfo(DEVICE_OP *pOpInfo, DEVICE_OPTAGINFO *pOpTagInfo);
void Device_ResetDeviceParamenter();
void Device_Iso15693GetBlock(u16 blockSize, u8 addr, u8 num, u8 *pBlock);
void Device_FormatFrame(u8 *pBuffer, TAG_OPINFO *pOpInfo);
void Device_Iso15693ReadTagMemory(DEVICE_OP *pOpInfo, DEVICE_OPTAGINFO *pOpTagInfo);
void Device_Iso15693WriteTagMemory(DEVICE_OP *pOpInfo, DEVICE_OPTAGINFO *pOpTagInfo);
void Device_ReadTagMemory(DEVICE_OP *pOpInfo, DEVICE_OPTAGINFO *pOpTagInfo);
void Device_WriteTagMemory(DEVICE_OP *pOpInfo, DEVICE_OPTAGINFO *pOpTagInfo);
void Device_DataUpdate();
void Device_TagReadFormat(DEVICE_OP *pOpInfo);
void Device_TagWriteFormat(DEVICE_OPTAGINFO *pOpTagInfo);
void Device_TestSlove();
void Device_FormatCanFrame(u8 type, u8 len, void *value, CAN_FRAME *pFrame);
void Device_CanopenParamsUpDate(DEVICE_CANOPENPARAMS *pParams);


u8 Device_Iso15693FmtWriteBlock(ISO15693_INFO *pInfo);
u8 Device_Iso15693FmtReadBlock(ISO15693_INFO *pInfo);
u8 Device_CanProcessFrame(Message *pMassage);
u8 Device_CanGetParams(u16 index, u8 subIndex, void **pValue);

u16 Device_BudGet(u8 bud);
u32 Device_CanFestivalCallBackInit(void);

BOOL Device_WriteDeviceParamenter(void);
BOOL Device_Step(DEVICE_OP *pOpInfo);
BOOL Device_Iso15693Step(DEVICE_OP *pOpInfo);	
BOOL Device_ProcessCanFrame();	
BOOL Device_CanSetParams(u16 index, u8 subIndex, void *pValue);
#endif