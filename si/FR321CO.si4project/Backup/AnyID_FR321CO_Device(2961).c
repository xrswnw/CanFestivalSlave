#include "AnyID_FR321CO_Device.h"

const u8 DEVICE_VERSION[DEVICE_VERSION_SIZE]@0x08005000 = "FR321CO 24042200 G231200";
#define DEVICE_NAME				"FR321CO"
#define DEVICE_SOFT_VERSION		"V1.0"
#define DEVICE_HARD_VERSION		"V1.0"

u32 g_nDeviceState = 0;
DEVICE_PARAMS g_sDeviceParams = {0};
DEVICE_INFO g_sDeviceInfo = {0};
DEVICE_OP g_sDeviceOpInfo = {0};
DEVICE_TAGMEM g_sDeviceTagMemory = {0};
DEVICE_BACKUPUID g_sDeviceBackupUid = {0};
DEVICE_OPTAGINFO g_sDeviceOpTagInfo = {0};
DEVICE_CANRCVFRAME g_sDeviceCanRcvFrame = {0};
DEVICE_CANRCVFRAME g_sDeviceCanTempRcvFrame = {0};

void Device_Delayms(u32 t)
{
    t *= 0x3800;
    t++;
    while(t--);
}

void Device_Delay100us(u32 t)
{
    t *= 0x600; 
    t++;
    while(t--);
}

void Device_Init()
{
	Device_CanFestivalCallBackInit();			//回调函数注册

}

u16 Device_BudGet(u8 bud)
{
	u16 value = 0;
	
	if(bud == DEVICE_CAN_BUD_1000K)
	{
		value = DEVICE_CAN_BUD_1000K_PSCVALUE;
	}
	else if(bud == DEVICE_CAN_BUD_800K)
	{
		value = DEVICE_CAN_BUD_800K_PSCVALUE;
	}
	else if(bud == DEVICE_CAN_BUD_250K)
	{
		value = DEVICE_CAN_BUD_250K_PSCVALUE;
	}	
	else if(bud == DEVICE_CAN_BUD_125K)
	{
		value = DEVICE_CAN_BUD_125K_PSCVALUE;
	}
	else if(bud == DEVICE_CAN_BUD_50K)
	{
		value = DEVICE_CAN_BUD_50K_PSCVALUE;
	}
	else if(bud == DEVICE_CAN_BUD_20K)
	{
		value = DEVICE_CAN_BUD_20K_PSCVALUE;
	}
	else if(bud == DEVICE_CAN_BUD_10K)
	{
		value = DEVICE_CAN_BUD_10K_PSCVALUE;
	}
	else 
	{
		value = DEVICE_CAN_BUD_500K_PSCVALUE;
	}
	return value;
}

void Device_ResetDeviceParamenter()
{
    memset(&g_sDeviceParams, 0, sizeof(DEVICE_PARAMS));
		
	//系统参数
	g_sDeviceParams.addr = 0x01;
	g_sDeviceParams.bud = DEVICE_CAN_BUD_1000K;
	
	g_sDeviceParams.rfParams.protocol = DEVICE_OP_TAG_15693;
    g_sDeviceParams.rfParams.rBlkNum = 1;                               //一次读取n+1个数据块
	
    g_sDeviceParams.autoBlockParams.addr = 0x00;                         
    g_sDeviceParams.autoBlockParams.len = 0x00;
    g_sDeviceParams.workTim.invtTim = DEVICE_INVENTORY_TO_DFT;
    g_sDeviceParams.workTim.idleTim = DEVICE_IDLE_TIM_DFT;
    g_sDeviceParams.workTim.keepTim = DEVICE_KEEPLIVE_TO_DFT;
	
	//can硬件参数
	//v 典型值：1000K、800k、500K、250k、125k、50k、20k、10k
	//v = PLCK1 / (psc * (tbs1 + 1 + tbs2 + 1 + 1)) = 36/(8 *(3 + 1 + 3 + 1+ 1)) 
	//v = 4000 / psc，默认1000K
	g_sDeviceParams.canPara.CAN_BS1 = CAN_BS1_4tq;
	g_sDeviceParams.canPara.CAN_BS2 = CAN_BS2_4tq;
	g_sDeviceParams.canPara.CAN_SJW = CAN_SJW_2tq;
	g_sDeviceParams.canPara.CAN_Prescaler = Device_BudGet(g_sDeviceParams.bud);
	g_sDeviceParams.canPara.CAN_Mode = CAN_Mode_Normal;
	g_sDeviceParams.canPara.CAN_ABOM = DISABLE;   								// 软件自动离线管理
	g_sDeviceParams.canPara.CAN_AWUM = DISABLE;   								// 睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	g_sDeviceParams.canPara.CAN_NART = ENABLE;   								// 使用报文自动传送 
	g_sDeviceParams.canPara.CAN_RFLM = DISABLE;   								// 报文不锁定,新的覆盖旧的 
	g_sDeviceParams.canPara.CAN_TTCM = DISABLE;									// 非时间触发通信模式  
	g_sDeviceParams.canPara.CAN_TXFP = DISABLE;   								// 优先级由报文标识符决定 
	//can过滤器 CAN_FilterIdHigh
	g_sDeviceParams.canFilterPara.CAN_FilterNumber = 0;  					 	// 过滤器0	
	g_sDeviceParams.canFilterPara.CAN_FilterIdHigh = 0x0000;   					// 32位ID
	g_sDeviceParams.canFilterPara.CAN_FilterIdLow = 0x0000;
	g_sDeviceParams.canFilterPara.CAN_FilterMaskIdHigh = 0x0000;   				// 32位MASK
	g_sDeviceParams.canFilterPara.CAN_FilterMaskIdLow = 0x0000;
	g_sDeviceParams.canFilterPara.CAN_FilterMode = CAN_FilterMode_IdMask;   	// 掩码模式
	g_sDeviceParams.canFilterPara.CAN_FilterScale = CAN_FilterScale_32bit;   	// 32位 
	g_sDeviceParams.canFilterPara.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;   // 过滤器0关联到FIFO0
	g_sDeviceParams.canFilterPara.CAN_FilterActivation = ENABLE;				// 激活过滤器0
	//索引参数 

	//canopen参数
	g_sDeviceParams.canOpneParams.periphParams.deviceType = 0x74747410;				//设备型号
	g_sDeviceParams.canOpneParams.periphParams.errReg = 0x00;
	g_sDeviceParams.canOpneParams.periphParams.errFiled = 0x00;
	g_sDeviceParams.canOpneParams.periphParams.syncCobId = 0x00;
	g_sDeviceParams.canOpneParams.periphParams.cycleCom = 0;
	g_sDeviceParams.canOpneParams.periphParams.syncWindowsLenth = 0x00;

	memcpy(&g_sDeviceParams.canOpneParams.periphParams.deviceName, DEVICE_NAME, 7);
	memcpy(&g_sDeviceParams.canOpneParams.periphParams.softVersion, DEVICE_SOFT_VERSION, 4);
	memcpy(&g_sDeviceParams.canOpneParams.periphParams.hardVersion, DEVICE_HARD_VERSION, 4);
	g_sDeviceParams.canOpneParams.periphParams.guardTime = 0x00;
	g_sDeviceParams.canOpneParams.periphParams.timeCobId = 0x00;
	g_sDeviceParams.canOpneParams.periphParams.timesTMap = 0x00;
	g_sDeviceParams.canOpneParams.periphParams.emncCobId = 0x80 + 0x00;
	g_sDeviceParams.canOpneParams.periphParams.producerTime = 30000;		//生产者心跳时间			，默认30s上报一次当前状态

	
	//RPDO1
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.subNum = CANOPEN_PDO_COM_PARAMS_NUM;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.cobId = 0x200;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.transmissionType = CANOPEN_PDO_TRANSIONTYPE_DEVICE_SUBPROTOCOL_SPECIFIC_ASYNCHRONOUS;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.inhibitTime = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.compatibilityEntry = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.eventTimer = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.syncValue = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.subNum = CANOPEN_PDO_MAP_PARAMS_NUM;
    g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[0] = 0x20010310; 
    g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[1] = 0x20010410; 
    
	//RPDO2
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.subNum = CANOPEN_PDO_COM_PARAMS_NUM;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.cobId = 0x300;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.transmissionType = CANOPEN_PDO_TRANSIONTYPE_DEVICE_SUBPROTOCOL_SPECIFIC_ASYNCHRONOUS;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.inhibitTime = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.compatibilityEntry = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.eventTimer = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.syncValue = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.subNum = CANOPEN_PDO_MAP_PARAMS_NUM;
    g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[0] = 0x20020310; 
    g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[1] = 0x20020410; 
	//RPDO3
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.subNum = CANOPEN_PDO_COM_PARAMS_NUM;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.cobId = 0x400;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.transmissionType = CANOPEN_PDO_TRANSIONTYPE_ACYCLIC_SYNCHRONIZATION;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.inhibitTime = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.compatibilityEntry = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.eventTimer = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.syncValue = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.subNum = CANOPEN_PDO_MAP_PARAMS_NUM;
    g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[0] = 0x20020520; 
    g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[1] = 0x20020620;

	//RPDO4
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.subNum = CANOPEN_PDO_COM_PARAMS_NUM;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.cobId = 0x500;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.transmissionType = CANOPEN_PDO_TRANSIONTYPE_ACYCLIC_SYNCHRONIZATION;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.inhibitTime = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.compatibilityEntry = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.eventTimer = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.syncValue = 0x00;
	g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.subNum = CANOPEN_PDO_MAP_PARAMS_NUM;

	//TPDO1
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.subNum = CANOPEN_PDO_COM_PARAMS_NUM;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.cobId = 0x180;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.transmissionType = CANOPEN_PDO_TRANSIONTYPE_MAKER_SPECIFIC_ASYNCHRONOUS;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.inhibitTime = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.compatibilityEntry = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.eventTimer = 500;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.syncValue = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.subNum = CANOPEN_PDO_MAP_PARAMS_NUM;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[0] = 0x20020108;
    g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[1] = 0x20010310; 
    g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[2] = 0x20010410; 

	//TPDO2
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.subNum = CANOPEN_PDO_COM_PARAMS_NUM;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.cobId = 0x280;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.transmissionType = CANOPEN_PDO_TRANSIONTYPE_DEVICE_SUBPROTOCOL_SPECIFIC_ASYNCHRONOUS;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.inhibitTime = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.compatibilityEntry = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.eventTimer = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.syncValue = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.subNum = CANOPEN_PDO_MAP_PARAMS_NUM;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[0] = 0x20030120;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[1] = 0x20030220;

	//TPDO3
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.subNum = CANOPEN_PDO_COM_PARAMS_NUM;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.cobId = 0x380;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.transmissionType = CANOPEN_PDO_TRANSIONTYPE_ACYCLIC_SYNCHRONIZATION;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.inhibitTime = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.compatibilityEntry = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.eventTimer = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.syncValue = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.subNum = CANOPEN_PDO_MAP_PARAMS_NUM;

	//TPDO4
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.subNum = CANOPEN_PDO_COM_PARAMS_NUM;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.cobId = 0x480;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.transmissionType = CANOPEN_PDO_TRANSIONTYPE_ACYCLIC_SYNCHRONIZATION;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.inhibitTime = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.compatibilityEntry = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.eventTimer = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.syncValue = 0x00;
	g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.subNum = CANOPEN_PDO_MAP_PARAMS_NUM;

		
}

void Device_ReadDeviceParamenter(void)
{
	BOOL b = FALSE, bBackup = FALSE;
     
    b = FRam_ReadBuffer(FRAME_INFO_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    if(b)
    {
        u32 crc1 = 0, crc2 = 0;

        crc1 = a_GetCrc((u8 *)(&g_sDeviceParams), (sizeof(DEVICE_PARAMS)) - 4);
        crc2 = g_sDeviceParams.crc;

        //检测参数是否正确，如果不正确，使用默认参数操作
        if(crc1 != crc2)
        {
            b = FALSE;
        }
    }  
    bBackup = FRam_ReadBuffer(FRAME_INFO_BACKUP_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    if(bBackup)
    {
        u32 crc1 = 0, crc2 = 0;

        crc1 = a_GetCrc((u8 *)(&g_sDeviceParams), (sizeof(DEVICE_PARAMS)) - 4);
        crc2 = g_sDeviceParams.crc;

        //检测参数是否正确，如果不正确，使用默认参数操作                        .
        if(crc1 != crc2)
        {
            bBackup = FALSE;
        }
    }
    if(b == FALSE && bBackup == FALSE)
    {
		//参数如何恢复默认？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
		Device_ResetDeviceParamenter();
        Device_WriteDeviceParamenter();
    }
    else if(b == TRUE && bBackup == FALSE)
    {
        FRam_ReadBuffer(FRAME_INFO_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
        FRam_WriteBuffer(FRAME_INFO_BACKUP_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    }
    else if(b == FALSE && bBackup == TRUE)
    {
        FRam_ReadBuffer(FRAME_INFO_BACKUP_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
        FRam_WriteBuffer(FRAME_INFO_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    }
    
    Fram_ReadBootParamenter();
	if((g_sFramBootParamenter.appState != FRAM_BOOT_APP_OK) || 
	   (g_sFramBootParamenter.br ^ g_sDeviceParams.bud)	||
		(g_sFramBootParamenter.addr ^ g_sDeviceParams.addr))
    {
        g_sFramBootParamenter.appState = FRAM_BOOT_APP_OK;
		g_sFramBootParamenter.br = g_sDeviceParams.bud;
		g_sFramBootParamenter.addr = g_sDeviceParams.addr;
        Fram_WriteBootParamenter();
    }
    Device_CanopenParamsUpDate(&g_sDeviceParams.canOpneParams);
}

void Device_CanopenParamsUpDate(DEVICE_CANOPENPARAMS *pParams)
{//canopen部分参数更新

    u8 index = 0;

	AnyId_FR321CO_Slave_obj1000 = pParams->periphParams.deviceType;
	AnyId_FR321CO_Slave_obj1001 = pParams->periphParams.errReg;
	AnyId_FR321CO_Slave_highestSubIndex_obj1003 = pParams->periphParams.errFiled;
	AnyId_FR321CO_Slave_obj1005 = pParams->periphParams.syncCobId;
	AnyId_FR321CO_Slave_obj1006 = pParams->periphParams.cycleCom;
	AnyId_FR321CO_Slave_obj1007 = pParams->periphParams.syncWindowsLenth;
	memcpy(&AnyId_FR321CO_Slave_obj1008, pParams->periphParams.deviceName, 10);
	memcpy(&AnyId_FR321CO_Slave_obj1009, pParams->periphParams.softVersion, 10);
	memcpy(&AnyId_FR321CO_Slave_obj100A, pParams->periphParams.hardVersion, 10);
	AnyId_FR321CO_Slave_obj100C = pParams->periphParams.guardTime;
	AnyId_FR321CO_Slave_obj100D = pParams->periphParams.lifeTime;
	AnyId_FR321CO_Slave_obj1012 = pParams->periphParams.timeCobId;
	AnyId_FR321CO_Slave_obj1013= pParams->periphParams.timesTMap;
	AnyId_FR321CO_Slave_obj1014 = pParams->periphParams.emncCobId;
	AnyId_FR321CO_Slave_obj1017 = pParams->periphParams.producerTime;

//RPDO1		
    AnyId_FR321CO_Slave_highestSubIndex_obj1400 = pParams->rPdoParams[CANOPEN_POD_INDEX_0].comParams.subNum;				
    AnyId_FR321CO_Slave_obj1400_COB_ID_used_by_PDO = pParams->rPdoParams[CANOPEN_POD_INDEX_0].comParams.cobId;	
    AnyId_FR321CO_Slave_obj1400_Transmission_Type = pParams->rPdoParams[CANOPEN_POD_INDEX_0].comParams.transmissionType;
    AnyId_FR321CO_Slave_obj1400_Inhibit_Time = pParams->rPdoParams[CANOPEN_POD_INDEX_0].comParams.inhibitTime;
    AnyId_FR321CO_Slave_obj1400_Compatibility_Entry = pParams->rPdoParams[CANOPEN_POD_INDEX_0].comParams.compatibilityEntry; 
    AnyId_FR321CO_Slave_obj1400_Event_Timer = pParams->rPdoParams[CANOPEN_POD_INDEX_0].comParams.eventTimer;
    AnyId_FR321CO_Slave_obj1400_SYNC_start_value = pParams->rPdoParams[CANOPEN_POD_INDEX_0].comParams.syncValue;
	AnyId_FR321CO_Slave_highestSubIndex_obj1600 = pParams->rPdoParams[CANOPEN_POD_INDEX_0].mapParams.subNum;
	for(index = 0; index < CANOPEN_PDO_MAP_PARAMS_NUM; index++)
	{
		AnyId_FR321CO_Slave_obj1600[index] = pParams->rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[index];
	}

//RPDO2 	
	AnyId_FR321CO_Slave_highestSubIndex_obj1401 = pParams->rPdoParams[CANOPEN_POD_INDEX_1].comParams.subNum;
    AnyId_FR321CO_Slave_obj1401_COB_ID_used_by_PDO = pParams->rPdoParams[CANOPEN_POD_INDEX_1].comParams.cobId;	
    AnyId_FR321CO_Slave_obj1401_Transmission_Type = pParams->rPdoParams[CANOPEN_POD_INDEX_1].comParams.transmissionType;
    AnyId_FR321CO_Slave_obj1401_Inhibit_Time = pParams->rPdoParams[CANOPEN_POD_INDEX_1].comParams.inhibitTime;
    AnyId_FR321CO_Slave_obj1401_Compatibility_Entry = pParams->rPdoParams[CANOPEN_POD_INDEX_1].comParams.compatibilityEntry; 
    AnyId_FR321CO_Slave_obj1401_Event_Timer = pParams->rPdoParams[CANOPEN_POD_INDEX_1].comParams.eventTimer;
    AnyId_FR321CO_Slave_obj1401_SYNC_start_value = pParams->rPdoParams[CANOPEN_POD_INDEX_1].comParams.syncValue;
	AnyId_FR321CO_Slave_highestSubIndex_obj1601 = pParams->rPdoParams[CANOPEN_POD_INDEX_1].mapParams.subNum;
	for(index = 0; index < CANOPEN_PDO_MAP_PARAMS_NUM; index++)
	{
		AnyId_FR321CO_Slave_obj1601[index] = pParams->rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[index];
	}

//RPDO3 
	AnyId_FR321CO_Slave_highestSubIndex_obj1402 = pParams->rPdoParams[CANOPEN_POD_INDEX_2].comParams.subNum;
    AnyId_FR321CO_Slave_obj1402_COB_ID_used_by_PDO = pParams->rPdoParams[CANOPEN_POD_INDEX_2].comParams.cobId;	
    AnyId_FR321CO_Slave_obj1402_Transmission_Type = pParams->rPdoParams[CANOPEN_POD_INDEX_2].comParams.transmissionType;
    AnyId_FR321CO_Slave_obj1402_Inhibit_Time = pParams->rPdoParams[CANOPEN_POD_INDEX_2].comParams.inhibitTime;
    AnyId_FR321CO_Slave_obj1402_Compatibility_Entry = pParams->rPdoParams[CANOPEN_POD_INDEX_2].comParams.compatibilityEntry; 
    AnyId_FR321CO_Slave_obj1402_Event_Timer = pParams->rPdoParams[CANOPEN_POD_INDEX_2].comParams.eventTimer;
    AnyId_FR321CO_Slave_obj1402_SYNC_start_value = pParams->rPdoParams[CANOPEN_POD_INDEX_2].comParams.syncValue;
	AnyId_FR321CO_Slave_highestSubIndex_obj1602 = pParams->rPdoParams[CANOPEN_POD_INDEX_2].mapParams.subNum;
	for(index = 0; index < CANOPEN_PDO_MAP_PARAMS_NUM; index++)
	{
		AnyId_FR321CO_Slave_obj1602[index] = pParams->rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[index];
	}	

//RPDO4 
	AnyId_FR321CO_Slave_highestSubIndex_obj1403 = pParams->rPdoParams[CANOPEN_POD_INDEX_3].comParams.subNum;
    AnyId_FR321CO_Slave_obj1403_COB_ID_used_by_PDO = pParams->rPdoParams[CANOPEN_POD_INDEX_3].comParams.cobId;	
    AnyId_FR321CO_Slave_obj1403_Transmission_Type = pParams->rPdoParams[CANOPEN_POD_INDEX_3].comParams.transmissionType;
    AnyId_FR321CO_Slave_obj1403_Inhibit_Time = pParams->rPdoParams[CANOPEN_POD_INDEX_3].comParams.inhibitTime;
    AnyId_FR321CO_Slave_obj1403_Compatibility_Entry = pParams->rPdoParams[CANOPEN_POD_INDEX_3].comParams.compatibilityEntry; 
    AnyId_FR321CO_Slave_obj1403_Event_Timer = pParams->rPdoParams[CANOPEN_POD_INDEX_3].comParams.eventTimer;
    AnyId_FR321CO_Slave_obj1403_SYNC_start_value = pParams->rPdoParams[CANOPEN_POD_INDEX_3].comParams.syncValue;
	AnyId_FR321CO_Slave_highestSubIndex_obj1600 = pParams->rPdoParams[CANOPEN_POD_INDEX_3].mapParams.subNum;
	for(index = 0; index < CANOPEN_PDO_MAP_PARAMS_NUM; index++)
	{
		AnyId_FR321CO_Slave_obj1603[index] = pParams->rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[index];
	}

//TPDO1		
    AnyId_FR321CO_Slave_highestSubIndex_obj1800 = pParams->tPdoParams[CANOPEN_POD_INDEX_0].comParams.subNum;
    AnyId_FR321CO_Slave_obj1800_COB_ID_used_by_PDO = pParams->tPdoParams[CANOPEN_POD_INDEX_0].comParams.cobId;	
    AnyId_FR321CO_Slave_obj1800_Transmission_Type = pParams->tPdoParams[CANOPEN_POD_INDEX_0].comParams.transmissionType;
    AnyId_FR321CO_Slave_obj1800_Inhibit_Time = pParams->tPdoParams[CANOPEN_POD_INDEX_0].comParams.inhibitTime;
    AnyId_FR321CO_Slave_obj1800_Compatibility_Entry = pParams->tPdoParams[CANOPEN_POD_INDEX_0].comParams.compatibilityEntry; 
    AnyId_FR321CO_Slave_obj1800_Event_Timer = pParams->tPdoParams[CANOPEN_POD_INDEX_0].comParams.eventTimer;
    AnyId_FR321CO_Slave_obj1800_SYNC_start_value = pParams->tPdoParams[CANOPEN_POD_INDEX_0].comParams.syncValue;
	AnyId_FR321CO_Slave_highestSubIndex_obj1A00 = pParams->tPdoParams[CANOPEN_POD_INDEX_0].mapParams.subNum;
	for(index = 0; index < CANOPEN_PDO_MAP_PARAMS_NUM; index++)
	{
		AnyId_FR321CO_Slave_obj1A00[index] = pParams->tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[index];
	}

//TPDO2 	
	AnyId_FR321CO_Slave_highestSubIndex_obj1801 = pParams->tPdoParams[CANOPEN_POD_INDEX_1].comParams.subNum;
    AnyId_FR321CO_Slave_obj1801_COB_ID_used_by_PDO = pParams->tPdoParams[CANOPEN_POD_INDEX_1].comParams.cobId;	
    AnyId_FR321CO_Slave_obj1801_Transmission_Type = pParams->tPdoParams[CANOPEN_POD_INDEX_1].comParams.transmissionType;
    AnyId_FR321CO_Slave_obj1801_Inhibit_Time = pParams->tPdoParams[CANOPEN_POD_INDEX_1].comParams.inhibitTime;
    AnyId_FR321CO_Slave_obj1801_Compatibility_Entry = pParams->tPdoParams[CANOPEN_POD_INDEX_1].comParams.compatibilityEntry; 
    AnyId_FR321CO_Slave_obj1801_Event_Timer = pParams->tPdoParams[CANOPEN_POD_INDEX_1].comParams.eventTimer;
    AnyId_FR321CO_Slave_obj1801_SYNC_start_value = pParams->tPdoParams[CANOPEN_POD_INDEX_1].comParams.syncValue;
	AnyId_FR321CO_Slave_highestSubIndex_obj1A01 = pParams->tPdoParams[CANOPEN_POD_INDEX_1].mapParams.subNum;
	for(index = 0; index < CANOPEN_PDO_MAP_PARAMS_NUM; index++)
	{
		AnyId_FR321CO_Slave_obj1A01[index] = pParams->tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[index];
	}

//TPDO3 
	AnyId_FR321CO_Slave_highestSubIndex_obj1802 = pParams->tPdoParams[CANOPEN_POD_INDEX_2].comParams.subNum;
    AnyId_FR321CO_Slave_obj1802_COB_ID_used_by_PDO = pParams->tPdoParams[CANOPEN_POD_INDEX_2].comParams.cobId;	
    AnyId_FR321CO_Slave_obj1802_Transmission_Type = pParams->tPdoParams[CANOPEN_POD_INDEX_2].comParams.transmissionType;
    AnyId_FR321CO_Slave_obj1802_Inhibit_Time = pParams->tPdoParams[CANOPEN_POD_INDEX_2].comParams.inhibitTime;
    AnyId_FR321CO_Slave_obj1802_Compatibility_Entry = pParams->tPdoParams[CANOPEN_POD_INDEX_2].comParams.compatibilityEntry; 
    AnyId_FR321CO_Slave_obj1802_Event_Timer = pParams->tPdoParams[CANOPEN_POD_INDEX_2].comParams.eventTimer;
    AnyId_FR321CO_Slave_obj1802_SYNC_start_value = pParams->tPdoParams[CANOPEN_POD_INDEX_2].comParams.syncValue;
	AnyId_FR321CO_Slave_highestSubIndex_obj1A02 = pParams->tPdoParams[CANOPEN_POD_INDEX_2].mapParams.subNum;
	for(index = 0; index < CANOPEN_PDO_MAP_PARAMS_NUM; index++)
	{
		AnyId_FR321CO_Slave_obj1A02[index] = pParams->tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[index];
	}	

//TPDO4 
	AnyId_FR321CO_Slave_highestSubIndex_obj1803 = pParams->tPdoParams[CANOPEN_POD_INDEX_3].comParams.subNum;
    AnyId_FR321CO_Slave_obj1803_COB_ID_used_by_PDO = pParams->tPdoParams[CANOPEN_POD_INDEX_3].comParams.cobId;	
    AnyId_FR321CO_Slave_obj1803_Transmission_Type = pParams->tPdoParams[CANOPEN_POD_INDEX_3].comParams.transmissionType;
    AnyId_FR321CO_Slave_obj1803_Inhibit_Time = pParams->tPdoParams[CANOPEN_POD_INDEX_3].comParams.inhibitTime;
    AnyId_FR321CO_Slave_obj1803_Compatibility_Entry = pParams->tPdoParams[CANOPEN_POD_INDEX_3].comParams.compatibilityEntry; 
    AnyId_FR321CO_Slave_obj1803_Event_Timer = pParams->tPdoParams[CANOPEN_POD_INDEX_3].comParams.eventTimer;
    AnyId_FR321CO_Slave_obj1803_SYNC_start_value = pParams->tPdoParams[CANOPEN_POD_INDEX_3].comParams.syncValue;
	AnyId_FR321CO_Slave_highestSubIndex_obj1A00 = pParams->tPdoParams[CANOPEN_POD_INDEX_3].mapParams.subNum;
	for(index = 0; index < CANOPEN_PDO_MAP_PARAMS_NUM; index++)
	{
		AnyId_FR321CO_Slave_obj1A03[index] = pParams->tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[index];
	}
 
//others 
    HardParams_DeviceAddr = g_sDeviceParams.addr;
    HardParams_Bud = g_sDeviceParams.bud;
    HardParams_CanFilterMode = g_sDeviceParams.canFilterPara.CAN_FilterMode;
    HardParams_CanFilterIdLow = g_sDeviceParams.canFilterPara.CAN_FilterIdLow;
    HardParams_CanFilterIdHigh = g_sDeviceParams.canFilterPara.CAN_FilterIdHigh;
    HardParams_CanFilterMaskIdLow = g_sDeviceParams.canFilterPara.CAN_FilterMaskIdLow;
    HardParams_CanFilterMaskIdHigh = g_sDeviceParams.canFilterPara.CAN_FilterMaskIdHigh;
    HardParams_RfidRfu = g_sDeviceParams.rfu;

    RfidParams_Protocol = g_sDeviceParams.rfParams.protocol;
    RfidParams_RBlkNum = g_sDeviceParams.rfParams.rBlkNum;
    RfidParams_AutoAddr = g_sDeviceParams.autoBlockParams.addr;
    RfidParams_AutoLen = g_sDeviceParams.autoBlockParams.len;
    RfidParams_InvtTim = g_sDeviceParams.workTim.invtTim;
    RfidParams_IdleTim = g_sDeviceParams.workTim.idleTim;
    RfidParams_KeepTim = g_sDeviceParams.workTim.keepTim;
    RfidParams_RfidRfu = g_sDeviceParams.rfParams.rfu;
}

BOOL Device_WriteDeviceParamenter(void)
{
    BOOL b = FALSE;

    g_sDeviceParams.crc = 0;
    g_sDeviceParams.crc = a_GetCrc((u8 *)(&g_sDeviceParams), (sizeof(DEVICE_PARAMS)) - 4);

    b = FRam_WriteBuffer(FRAME_INFO_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    b = FRam_WriteBuffer(FRAME_INFO_BACKUP_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    
    return b;
}


/*
	1.直接映射参数，在修改参数前加入校验，如校验不过，不允修改, 返回错误。节省资源，快速。


	2.间接映射参数，在回调函数中进行数据操作。数据修改完再校验，不过则恢复，结构相对整洁，但资源消耗过大，且需高频更新间接参数值。
*/
UNS32 index0x1017_0x00_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1017 ^ g_sDeviceParams.canOpneParams.periphParams.producerTime)
	{
		g_sDeviceParams.canOpneParams.periphParams.producerTime = AnyId_FR321CO_Slave_obj1017;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}




UNS32 index0x1400_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1400_COB_ID_used_by_PDO ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.cobId)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.cobId = AnyId_FR321CO_Slave_obj1400_COB_ID_used_by_PDO;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1400_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1400_Transmission_Type ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.transmissionType)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.transmissionType = AnyId_FR321CO_Slave_obj1400_Transmission_Type;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1400_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1400_Inhibit_Time ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.inhibitTime)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.inhibitTime = AnyId_FR321CO_Slave_obj1400_Inhibit_Time;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1400_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1400_Compatibility_Entry ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.compatibilityEntry)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.compatibilityEntry = AnyId_FR321CO_Slave_obj1400_Compatibility_Entry;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1400_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1400_Event_Timer ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.eventTimer)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.eventTimer = AnyId_FR321CO_Slave_obj1400_Event_Timer;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1400_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1400_SYNC_start_value ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.syncValue)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].comParams.syncValue = AnyId_FR321CO_Slave_obj1400_SYNC_start_value;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1401_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1401_COB_ID_used_by_PDO ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.cobId)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.cobId = AnyId_FR321CO_Slave_obj1401_COB_ID_used_by_PDO;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1401_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1401_Transmission_Type ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.transmissionType)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.transmissionType = AnyId_FR321CO_Slave_obj1401_Transmission_Type;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1401_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1401_Inhibit_Time ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.inhibitTime)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.inhibitTime = AnyId_FR321CO_Slave_obj1401_Inhibit_Time;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1401_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1401_Compatibility_Entry ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.compatibilityEntry)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.compatibilityEntry = AnyId_FR321CO_Slave_obj1401_Compatibility_Entry;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1401_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1401_Event_Timer ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.eventTimer)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.eventTimer = AnyId_FR321CO_Slave_obj1401_Event_Timer;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1401_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1401_SYNC_start_value ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.syncValue)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].comParams.syncValue = AnyId_FR321CO_Slave_obj1401_SYNC_start_value;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1402_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1402_COB_ID_used_by_PDO ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.cobId)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.cobId = AnyId_FR321CO_Slave_obj1402_COB_ID_used_by_PDO;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1402_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1402_Transmission_Type ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.transmissionType)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.transmissionType = AnyId_FR321CO_Slave_obj1402_Transmission_Type;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1402_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1402_Inhibit_Time ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.inhibitTime)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.inhibitTime = AnyId_FR321CO_Slave_obj1402_Inhibit_Time;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1402_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1402_Compatibility_Entry ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.compatibilityEntry)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.compatibilityEntry = AnyId_FR321CO_Slave_obj1402_Compatibility_Entry;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1402_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1402_Event_Timer ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.eventTimer)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.eventTimer = AnyId_FR321CO_Slave_obj1402_Event_Timer;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1402_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1402_SYNC_start_value ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.syncValue)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].comParams.syncValue = AnyId_FR321CO_Slave_obj1402_SYNC_start_value;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1403_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1403_COB_ID_used_by_PDO ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.cobId)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.cobId = AnyId_FR321CO_Slave_obj1403_COB_ID_used_by_PDO;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1403_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1403_Transmission_Type ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.transmissionType)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.transmissionType = AnyId_FR321CO_Slave_obj1403_Transmission_Type;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1403_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{

	if(AnyId_FR321CO_Slave_obj1403_Inhibit_Time ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.inhibitTime)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.inhibitTime = AnyId_FR321CO_Slave_obj1403_Inhibit_Time;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1403_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1403_Compatibility_Entry ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.compatibilityEntry)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.compatibilityEntry = AnyId_FR321CO_Slave_obj1403_Compatibility_Entry;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1403_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1403_Event_Timer ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.eventTimer)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.eventTimer = AnyId_FR321CO_Slave_obj1403_Event_Timer;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1403_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1403_SYNC_start_value ^ g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.syncValue)
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].comParams.syncValue = AnyId_FR321CO_Slave_obj1403_SYNC_start_value;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1600_0x00_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] ^ AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_0])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] = AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_0];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1600_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] ^ AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_1])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] = AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_1];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1600_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] ^ AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_2])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] = AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_2];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;

}

UNS32 index0x1600_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] ^ AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_3])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] = AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_3];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;

}

UNS32 index0x1600_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] ^ AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_4])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] = AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_4];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;

}

UNS32 index0x1600_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] ^ AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_5])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] = AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_5];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;

}

UNS32 index0x1600_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] ^ AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_6])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] = AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_6];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;

}

UNS32 index0x1600_0x07_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] ^ AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_7])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] = AnyId_FR321CO_Slave_obj1600[CANOPEN_POD_MAP_INDEX_7];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1601_0x00_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] ^ AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_0])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] = AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_0];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1601_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] ^ AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_1])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] = AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_1];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1601_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] ^ AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_2])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] = AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_2];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1601_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] ^ AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_3])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] = AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_3];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1601_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] ^ AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_4])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] = AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_4];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;

}

UNS32 index0x1601_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] ^ AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_5])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] = AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_5];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1601_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] ^ AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_6])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] = AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_6];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1601_0x07_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] ^ AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_7])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] = AnyId_FR321CO_Slave_obj1601[CANOPEN_POD_MAP_INDEX_7];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1602_0x00_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] ^ AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_0])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] = AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_0];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1602_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] ^ AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_1])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] = AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_1];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1602_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] ^ AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_2])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] = AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_2];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1602_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] ^ AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_3])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] = AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_3];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1602_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] ^ AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_4])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] = AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_4];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1602_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] ^ AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_5])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] = AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_5];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1602_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] ^ AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_6])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] = AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_6];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1602_0x07_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] ^ AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_7])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] = AnyId_FR321CO_Slave_obj1602[CANOPEN_POD_MAP_INDEX_7];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1603_0x00_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] ^ AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_0])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] = AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_0];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1603_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] ^ AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_1])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] = AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_1];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1603_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] ^ AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_2])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] = AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_2];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;

}

UNS32 index0x1603_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] ^ AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_3])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] = AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_3];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;

}

UNS32 index0x1603_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] ^ AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_4])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] = AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_4];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1603_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] ^ AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_5])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] = AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_5];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1603_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] ^ AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_6])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] = AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_6];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1603_0x07_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] ^ AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_7])
	{
		g_sDeviceParams.canOpneParams.rPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] = AnyId_FR321CO_Slave_obj1603[CANOPEN_POD_MAP_INDEX_7];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}


UNS32 index0x1800_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1800_COB_ID_used_by_PDO ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.cobId)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.cobId = AnyId_FR321CO_Slave_obj1800_COB_ID_used_by_PDO;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1800_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1800_Transmission_Type ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.transmissionType)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.transmissionType = AnyId_FR321CO_Slave_obj1800_Transmission_Type;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1800_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1800_Inhibit_Time ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.inhibitTime)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.inhibitTime = AnyId_FR321CO_Slave_obj1800_Inhibit_Time;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1800_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1800_Compatibility_Entry ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.compatibilityEntry)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.compatibilityEntry = AnyId_FR321CO_Slave_obj1800_Compatibility_Entry;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1800_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1800_Event_Timer ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.eventTimer)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.eventTimer = AnyId_FR321CO_Slave_obj1800_Event_Timer;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1800_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1800_SYNC_start_value ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.syncValue)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.syncValue = AnyId_FR321CO_Slave_obj1800_SYNC_start_value;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1801_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1801_COB_ID_used_by_PDO ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.cobId)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.cobId = AnyId_FR321CO_Slave_obj1801_COB_ID_used_by_PDO;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1801_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1801_Transmission_Type ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.transmissionType)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.transmissionType = AnyId_FR321CO_Slave_obj1801_Transmission_Type;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1801_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
		if(AnyId_FR321CO_Slave_obj1801_Inhibit_Time ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.inhibitTime)
		{
			g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.inhibitTime = AnyId_FR321CO_Slave_obj1801_Inhibit_Time;
			Device_WriteDeviceParamenter();
		}
	return OD_SUCCESSFUL;
}

UNS32 index0x1801_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1801_Compatibility_Entry ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.compatibilityEntry)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.compatibilityEntry = AnyId_FR321CO_Slave_obj1801_Compatibility_Entry;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1801_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1801_Event_Timer ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.eventTimer)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.eventTimer = AnyId_FR321CO_Slave_obj1801_Event_Timer;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1801_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1801_SYNC_start_value ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.syncValue)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.syncValue = AnyId_FR321CO_Slave_obj1801_SYNC_start_value;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1802_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1802_COB_ID_used_by_PDO ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.cobId)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.cobId = AnyId_FR321CO_Slave_obj1802_COB_ID_used_by_PDO;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1802_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1802_Transmission_Type ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.transmissionType)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.transmissionType = AnyId_FR321CO_Slave_obj1802_Transmission_Type;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1802_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1802_Inhibit_Time ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.inhibitTime)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.inhibitTime = AnyId_FR321CO_Slave_obj1802_Inhibit_Time;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1802_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1802_Compatibility_Entry ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.compatibilityEntry)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.compatibilityEntry = AnyId_FR321CO_Slave_obj1802_Compatibility_Entry;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1802_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1802_Event_Timer ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.eventTimer)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.eventTimer = AnyId_FR321CO_Slave_obj1802_Event_Timer;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1802_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1802_SYNC_start_value ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.syncValue)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.syncValue = AnyId_FR321CO_Slave_obj1802_SYNC_start_value;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1803_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1803_COB_ID_used_by_PDO ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.cobId)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.cobId = AnyId_FR321CO_Slave_obj1803_COB_ID_used_by_PDO;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1803_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1803_Transmission_Type ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.transmissionType)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.transmissionType = AnyId_FR321CO_Slave_obj1803_Transmission_Type;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1803_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1803_Inhibit_Time ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.inhibitTime)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.inhibitTime = AnyId_FR321CO_Slave_obj1803_Inhibit_Time;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1803_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1803_Compatibility_Entry ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.compatibilityEntry)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.compatibilityEntry = AnyId_FR321CO_Slave_obj1803_Compatibility_Entry;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1803_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1803_Event_Timer ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.eventTimer)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.eventTimer = AnyId_FR321CO_Slave_obj1803_Event_Timer;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1803_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(AnyId_FR321CO_Slave_obj1803_SYNC_start_value ^ g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.syncValue)
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.syncValue = AnyId_FR321CO_Slave_obj1803_SYNC_start_value;
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A00_0x00_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] ^ AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_0])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] = AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_0];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A00_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] ^ AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_1])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] = AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_1];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A00_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] ^ AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_2])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] = AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_2];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A00_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] ^ AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_3])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] = AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_3];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A00_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] ^ AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_4])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] = AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_4];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A00_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] ^ AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_5])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] = AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_5];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A00_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] ^ AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_6])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] = AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_6];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A00_0x07_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] ^ AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_7])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] = AnyId_FR321CO_Slave_obj1A00[CANOPEN_POD_MAP_INDEX_7];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A01_0x00_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] ^ AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_0])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] = AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_0];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A01_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] ^ AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_1])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] = AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_1];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A01_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] ^ AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_2])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] = AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_2];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A01_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] ^ AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_3])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] = AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_3];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A01_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] ^ AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_4])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] = AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_4];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A01_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] ^ AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_5])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] = AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_5];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A01_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] ^ AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_6])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] = AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_6];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A01_0x07_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] ^ AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_7])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] = AnyId_FR321CO_Slave_obj1A01[CANOPEN_POD_MAP_INDEX_7];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A02_0x00_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] ^ AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_0])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] = AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_0];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A02_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] ^ AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_1])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] = AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_1];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A02_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] ^ AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_2])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] = AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_2];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A02_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] ^ AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_3])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] = AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_3];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A02_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] ^ AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_4])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] = AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_4];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A02_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] ^ AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_5])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] = AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_5];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A02_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] ^ AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_6])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] = AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_6];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A02_0x07_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] ^ AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_7])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] = AnyId_FR321CO_Slave_obj1A02[CANOPEN_POD_MAP_INDEX_7];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A03_0x00_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] ^ AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_0])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_0] = AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_0];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A03_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] ^ AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_1])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_1] = AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_1];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A03_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] ^ AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_2])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_2] = AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_2];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A03_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] ^ AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_3])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_3] = AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_3];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A03_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] ^ AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_4])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_4] = AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_4];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A03_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] ^ AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_5])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_5] = AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_5];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A03_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] ^ AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_6])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].mapParams.obData[CANOPEN_POD_MAP_INDEX_6] = AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_6];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}

UNS32 index0x1A03_0x07_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	if(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] ^ AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_7])
	{
		g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].mapParams.obData[CANOPEN_POD_MAP_INDEX_7] = AnyId_FR321CO_Slave_obj1A03[CANOPEN_POD_MAP_INDEX_7];
		Device_WriteDeviceParamenter();
	}
	return OD_SUCCESSFUL;
}





UNS32 index0x2000_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if(HardParams_DeviceAddr > DEVICE_NMT_NODEID_NUM || HardParams_DeviceAddr == 0)
	{
		errorCode = OD_VALUE_RANGE_EXCEEDED;
        HardParams_DeviceAddr = g_sDeviceParams.addr;
	}
	else
	{	
		errorCode = OD_SUCCESSFUL;
		if(g_sDeviceParams.addr ^ HardParams_DeviceAddr)
		{
			g_sDeviceParams.addr = HardParams_DeviceAddr;
			g_sFramBootParamenter.addr = g_sDeviceParams.addr;
			Fram_WriteBootParamenter();
			Device_WriteDeviceParamenter();
		}
	}
	
	return errorCode;
}

UNS32 index0x2000_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if(HardParams_Bud > DEVICE_CAN_BUD_10K)
	{
		errorCode = OD_VALUE_RANGE_EXCEEDED;
        HardParams_Bud = g_sDeviceParams.bud; 
	}
	else
	{	
		errorCode = OD_SUCCESSFUL;
		if(g_sDeviceParams.bud ^ HardParams_Bud)
		{
            g_sDeviceParams.bud = HardParams_Bud;
            g_sFramBootParamenter.br = g_sDeviceParams.bud;
            Fram_WriteBootParamenter();
			Device_WriteDeviceParamenter();
		}
	}
	
	return errorCode;
}

UNS32 index0x2000_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if((HardParams_CanFilterMode != CAN_FilterMode_IdList) && (HardParams_CanFilterMode != CAN_FilterMode_IdMask))
	{
		errorCode = OD_VALUE_RANGE_EXCEEDED;
        HardParams_CanFilterMode = g_sDeviceParams.canFilterPara.CAN_FilterMode;
	}
	else
	{	
		errorCode = OD_SUCCESSFUL;
		if(g_sDeviceParams.canFilterPara.CAN_FilterMode ^ HardParams_CanFilterMode)
		{
			g_sDeviceParams.canFilterPara.CAN_FilterMode = HardParams_CanFilterMode;
			Device_WriteDeviceParamenter();
		}
	}
	
	return errorCode;
}

UNS32 index0x2000_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if(g_sDeviceParams.canFilterPara.CAN_FilterIdLow ^ HardParams_CanFilterIdLow)
	{
		g_sDeviceParams.canFilterPara.CAN_FilterIdLow = HardParams_CanFilterIdLow;
		Device_WriteDeviceParamenter();
	}
	
	return errorCode;
}

UNS32 index0x2000_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if(g_sDeviceParams.canFilterPara.CAN_FilterIdHigh ^ HardParams_CanFilterIdHigh)
	{
		g_sDeviceParams.canFilterPara.CAN_FilterIdHigh = HardParams_CanFilterIdHigh;
		Device_WriteDeviceParamenter();
	}
	
	return errorCode;
}

UNS32 index0x2000_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if(g_sDeviceParams.canFilterPara.CAN_FilterMaskIdLow ^ HardParams_CanFilterMaskIdLow)
	{
		g_sDeviceParams.canFilterPara.CAN_FilterMaskIdLow = HardParams_CanFilterMaskIdLow;
		Device_WriteDeviceParamenter();
	}
	
	return errorCode;
}

UNS32 index0x2000_0x07_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if(g_sDeviceParams.canFilterPara.CAN_FilterMaskIdHigh ^ HardParams_CanFilterMaskIdHigh)
	{
		g_sDeviceParams.canFilterPara.CAN_FilterMaskIdHigh = HardParams_CanFilterMaskIdHigh;
		Device_WriteDeviceParamenter();
	}
	
	return errorCode;
}
UNS32 index0x2000_0x08_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	return errorCode;
}


UNS32 index0x2001_0x01_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if((RfidParams_Protocol != DEVICE_OP_TAG_15693) && (RfidParams_Protocol != DEVICE_OP_TAG_14443A))
	{
		errorCode = OD_VALUE_RANGE_EXCEEDED;
        RfidParams_Protocol = g_sDeviceParams.rfParams.protocol;
	}
	else
	{	
		errorCode = OD_SUCCESSFUL;
		if(g_sDeviceParams.rfParams.protocol ^ RfidParams_Protocol)
		{
			g_sDeviceParams.rfParams.protocol = RfidParams_Protocol;
			Device_WriteDeviceParamenter();
		}
	}
	
	return errorCode;
}

UNS32 index0x2001_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if(g_sDeviceParams.rfParams.rBlkNum ^ RfidParams_RBlkNum)
	{
		g_sDeviceParams.rfParams.rBlkNum = RfidParams_RBlkNum;
		Device_WriteDeviceParamenter();
	}
	
	return errorCode;
}

UNS32 index0x2001_0x03_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if(g_sDeviceParams.autoBlockParams.addr ^ RfidParams_AutoAddr)
	{
		g_sDeviceParams.autoBlockParams.addr = RfidParams_AutoAddr;
		Device_WriteDeviceParamenter();
	}
	
	return errorCode;
}

UNS32 index0x2001_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if(g_sDeviceParams.autoBlockParams.len ^ RfidParams_AutoLen)
	{
		g_sDeviceParams.autoBlockParams.len = RfidParams_AutoLen;
		Device_WriteDeviceParamenter();
	}
	
	return errorCode;
}

UNS32 index0x2001_0x05_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if(g_sDeviceParams.workTim.invtTim ^ RfidParams_InvtTim)
	{
		g_sDeviceParams.workTim.invtTim = RfidParams_InvtTim;
		Device_WriteDeviceParamenter();
	}
	
	return errorCode;
}

UNS32 index0x2001_0x06_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if(g_sDeviceParams.workTim.idleTim ^ RfidParams_IdleTim)
	{
		g_sDeviceParams.workTim.idleTim = RfidParams_IdleTim;
		Device_WriteDeviceParamenter();
	}
	
	return errorCode;
}


UNS32 index0x2001_0x07_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if(g_sDeviceParams.workTim.keepTim ^ RfidParams_KeepTim)
	{
		g_sDeviceParams.workTim.keepTim = RfidParams_KeepTim;
		Device_WriteDeviceParamenter();
	}
	return errorCode;
}

UNS32 index0x2002_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	u8 cmdRest = 0, cmdGetTver = 0, cmdGetSver =0, cmdGetHver = 0;
	
	cmdRest = DEVICE_FRAME_CMD_RESET & 0x00FF;
	cmdGetTver = DEVICE_FRAME_CMD_GET_TVER & 0x00FF;
	cmdGetSver = DEVICE_FRAME_CMD_GET_SVER & 0x00FF;
	cmdGetHver = DEVICE_FRAME_CMD_GET_HVER & 0x00FF;
	//控制命令接收
	if(DeviceParams_OpDevice == DEVICE_FRAME_CMD_ENABLE_READ_BLOCK || 
		DeviceParams_OpDevice == DEVICE_FRAME_CMD_ENABLE_WRITE_BLOCK ||
		DeviceParams_OpDevice == cmdRest ||
        DeviceParams_OpDevice == DEVICE_FRAME_CMD_RESET_DEFULTPARAMS||
		DeviceParams_OpDevice == cmdGetTver ||
		DeviceParams_OpDevice == cmdGetSver ||
		DeviceParams_OpDevice == cmdGetHver ||
		DeviceParams_OpDevice == DEVICE_FRAME_CMD_CLEAR_OPFRAME)
	{
		a_SetStateBit(g_sDeviceCanRcvFrame.flag, DEVICE_CAN_FLAG_RCV);
	}
	else
	{
		DeviceParams_OpDevice = 0;
		errorCode = OD_VALUE_RANGE_EXCEEDED;
	}
	
	return errorCode;
}

UNS32 index0x2002_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	//控制命令接收
	if(DeviceParams_OpLen > 32)													//最大预留三十二字节
	{
		DeviceParams_OpLen = 0;
		errorCode = OD_VALUE_RANGE_EXCEEDED;
	}
	
	return errorCode;
}


UNS32 index0x2100_0x02_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if((ComData_RcvBuffer[DEVICE_CAN_FRAME_OP] == DEVICE_TAG_OP_READ_BLOCK) || (ComData_RcvBuffer[DEVICE_CAN_FRAME_OP] == DEVICE_TAG_OP_WRITE_BLOCK))
	{
		if(ComData_RcvBuffer[DEVICE_CAN_FRAME_LENTH] <= CANFESTIVAL_COM_LEN)
		{
			a_SetStateBit(g_sDeviceCanRcvFrame.flag, DEVICE_CAN_FLAG_RCV);	
			memcpy(&g_sDeviceCanRcvFrame.buffer, ComData_RcvBuffer, CANFESTIVAL_COM_LEN);	
		}
		else
		{
			errorCode = OD_VALUE_RANGE_EXCEEDED;
		}
	}
	else
	{
		errorCode = OD_VALUE_RANGE_EXCEEDED;
	}
	
	return errorCode;
}

UNS32 index0x2100_0x04_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
	UNS32 errorCode = OD_SUCCESSFUL;
	
	if(g_sDeviceInfo.testInfo.testEnable ^ ComData_TestState)
	{
		g_sDeviceInfo.testInfo.testEnable = ComData_TestState;
	}
	return errorCode;
}

#define Device_ChkCallBack(value)		({\
                                            u8 result = 0;\
                                            if(value == OD_SUCCESSFUL)\
                                            {\
                                               result = 1;\
                                            }\
                                            (result);\
                                          })

u32 Device_CanFestivalCallBackInit(void)				//回调函数注册
{
	UNS32 errorCode = OD_VALUE_RANGE_EXCEEDED;
    u8 num = 0, pos = 0;;
  
    num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1017, 0x00, index0x1017_0x00_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1400, 0x01, index0x1400_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1400, 0x02, index0x1400_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1400, 0x03, index0x1400_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1400, 0x04, index0x1400_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1400, 0x05, index0x1400_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1400, 0x06, index0x1400_0x06_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1401, 0x01, index0x1401_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1401, 0x02, index0x1401_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1401, 0x03, index0x1401_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1401, 0x04, index0x1401_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1401, 0x05, index0x1401_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1401, 0x06, index0x1401_0x06_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1402, 0x01, index0x1402_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1402, 0x02, index0x1402_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1402, 0x03, index0x1402_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1402, 0x04, index0x1402_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1402, 0x05, index0x1402_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1402, 0x06, index0x1402_0x06_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1403, 0x01, index0x1403_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1403, 0x02, index0x1403_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1403, 0x03, index0x1403_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1403, 0x04, index0x1403_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1403, 0x05, index0x1403_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1403, 0x06, index0x1403_0x06_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1600, 0x00, index0x1600_0x00_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1600, 0x01, index0x1600_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1600, 0x02, index0x1600_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1600, 0x03, index0x1600_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1600, 0x04, index0x1600_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1600, 0x05, index0x1600_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1600, 0x06, index0x1600_0x06_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1600, 0x07, index0x1600_0x07_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1601, 0x00, index0x1601_0x00_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1601, 0x01, index0x1601_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1601, 0x02, index0x1601_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1601, 0x03, index0x1601_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1601, 0x04, index0x1601_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1601, 0x05, index0x1601_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1601, 0x06, index0x1601_0x06_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1601, 0x07, index0x1601_0x07_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1602, 0x00, index0x1602_0x00_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1602, 0x01, index0x1602_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1602, 0x02, index0x1602_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1602, 0x03, index0x1602_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1602, 0x04, index0x1602_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1602, 0x05, index0x1602_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1602, 0x06, index0x1602_0x06_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1602, 0x07, index0x1602_0x07_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1603, 0x00, index0x1603_0x00_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1603, 0x01, index0x1603_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1603, 0x02, index0x1603_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1603, 0x03, index0x1603_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1603, 0x04, index0x1603_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1603, 0x05, index0x1603_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1603, 0x06, index0x1603_0x06_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1603, 0x07, index0x1603_0x07_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1800, 0x01, index0x1800_0x01_callback));pos++;		
	//num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1800, 0x02, index0x1800_0x02_callback));pos++;				//PDO初始化会处理
	//num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1800, 0x03, index0x1800_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1800, 0x04, index0x1800_0x04_callback));pos++;
	//num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1800, 0x05, index0x1800_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1800, 0x06, index0x1800_0x06_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1801, 0x01, index0x1801_0x01_callback));pos++;		
	//num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1801, 0x02, index0x1801_0x02_callback));pos++;
	//num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1801, 0x03, index0x1801_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1801, 0x04, index0x1801_0x04_callback));pos++;
	//num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1801, 0x05, index0x1801_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1801, 0x06, index0x1801_0x06_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1802, 0x01, index0x1802_0x01_callback));pos++;		
	//num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1802, 0x02, index0x1802_0x02_callback));pos++;
	//num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1802, 0x03, index0x1802_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1802, 0x04, index0x1802_0x04_callback));pos++;
	//num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1802, 0x05, index0x1802_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1802, 0x06, index0x1802_0x06_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1803, 0x01, index0x1803_0x01_callback));pos++;		
	//num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1803, 0x02, index0x1803_0x02_callback));pos++;
	//num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1803, 0x03, index0x1803_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1803, 0x04, index0x1803_0x04_callback));pos++;
	//num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1803, 0x05, index0x1803_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1803, 0x06, index0x1803_0x06_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A00, 0x00, index0x1A00_0x00_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A00, 0x01, index0x1A00_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A00, 0x02, index0x1A00_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A00, 0x03, index0x1A00_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A00, 0x04, index0x1A00_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A00, 0x05, index0x1A00_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A00, 0x06, index0x1A00_0x06_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A00, 0x07, index0x1A00_0x07_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A01, 0x00, index0x1A01_0x00_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A01, 0x01, index0x1A01_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A01, 0x02, index0x1A01_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A01, 0x03, index0x1A01_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A01, 0x04, index0x1A01_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A01, 0x05, index0x1A01_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A01, 0x06, index0x1A01_0x06_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A01, 0x07, index0x1A01_0x07_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A02, 0x00, index0x1A02_0x00_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A02, 0x01, index0x1A02_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A02, 0x02, index0x1A02_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A02, 0x03, index0x1A02_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A02, 0x04, index0x1A02_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A02, 0x05, index0x1A02_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A02, 0x06, index0x1A02_0x06_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A02, 0x07, index0x1A02_0x07_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A03, 0x00, index0x1A03_0x00_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A03, 0x01, index0x1A03_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A03, 0x02, index0x1A03_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A03, 0x03, index0x1A03_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A03, 0x04, index0x1A03_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A03, 0x05, index0x1A03_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A03, 0x06, index0x1A03_0x06_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x1A03, 0x07, index0x1A03_0x07_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2000, 0x01, index0x2000_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2000, 0x02, index0x2000_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2000, 0x03, index0x2000_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2000, 0x04, index0x2000_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2000, 0x05, index0x2000_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2000, 0x06, index0x2000_0x06_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2000, 0x07, index0x2000_0x07_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2000, 0x08, index0x2000_0x08_callback));pos++;
	
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2001, 0x01, index0x2001_0x01_callback));pos++;		
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2001, 0x02, index0x2001_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2001, 0x03, index0x2001_0x03_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2001, 0x04, index0x2001_0x04_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2001, 0x05, index0x2001_0x05_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2001, 0x06, index0x2001_0x06_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2001, 0x07, index0x2001_0x07_callback));pos++;
	
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2002, 0x02, index0x2002_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2002, 0x04, index0x2002_0x04_callback));pos++;

	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2100, 0x02, index0x2100_0x02_callback));pos++;
	num += Device_ChkCallBack(RegisterSetODentryCallBack(&AnyId_FR321CO_Slave_Data, 0x2100, 0x04, index0x2100_0x04_callback));pos++;
    
    if(num == pos)      //校验回调函数注册
    {
        errorCode = OD_SUCCESSFUL;
    }
    else
    {
        errorCode = 0x74747410;
    }

	return errorCode;
}

void Device_AutoOpTag(DEVICE_OP *pOpInfo)
{
	if(g_sDeviceParams.rfParams.protocol == DEVICE_OP_TAG_15693)
	{
		Device_Iso15693AutoOpTag(pOpInfo);
	}
	else
	{
	
	}


}

void Device_Iso15693AutoOpTag(DEVICE_OP *pOpInfo)
{
	short temp = 0;
    Device_ResetOp();
	
    St25r3916_CloseAntenna();
    Device_Delayms(2);
    St25r3916_OpenAntenna();
    Device_Delayms(2);
	a_SetState(g_sDeviceInfo.rfidFlag, DEVICE_RF_FLAG_OPEN); 
	
    pOpInfo->opTagMode = DEVICE_OP_TAG_INVT;
    pOpInfo->operation[pOpInfo->num++] = DEVICE_OPERATION_INVT;

	temp = (short)g_sDeviceParams.autoBlockParams.addr;
	if(temp < 0)
	{
		pOpInfo->memAddr = 0;
        pOpInfo->memLen = g_sDeviceParams.autoBlockParams.addr + g_sDeviceParams.autoBlockParams.len;
        temp = (short)pOpInfo->memLen;
        if(temp < 0)
        {
            pOpInfo->memLen = 0;
        }
	}
	else
	{
        pOpInfo->memAddr = g_sDeviceParams.autoBlockParams.addr;
        pOpInfo->memLen = g_sDeviceParams.autoBlockParams.len;
	}
	
	if(pOpInfo->memLen)
	{
		pOpInfo->operation[pOpInfo->num++] = DEVICE_OPERATION_READ;
        pOpInfo->opTagMode = DEVICE_OP_TAG_READ;
	}
	
	pOpInfo->index = 0;     
    pOpInfo->step = 0;
    pOpInfo->bAutoOp = TRUE;
    pOpInfo->delay = g_nSysTick;
	a_SetState(pOpInfo->state, DEVICE_STAT_TX);
	
	if(g_sDeviceInfo.testInfo.testEnable == DEVICE_TEST_MODE_ENABLE)
	{
		g_sDeviceInfo.testInfo.allTick ++;
	}
}

void Device_Tranceive(DEVICE_OP *pOpInfo)
{
	u8 rlt = ISO15693_ERR_NORSP, op = 0, state = ISO15693_ERR_SUC;;
	ISO15693_INFO *pInfo = NULL;
	DEVICE_TAGMEM *pTagMem = NULL;
	
	pInfo = &pOpInfo->protocol.iso15693;
    op = pOpInfo->operation[pOpInfo->index];
    pTagMem = &g_sDeviceTagMemory;
	switch(op)
	{
		case DEVICE_OPERATION_INVT:
			if(!ISO15693_Inventory(0x00, 0x00, pInfo->u))
			{
				rlt = ISO15693_ERR_SUC;
				Device_Iso15693SaveUid(pInfo->u, pTagMem);
				Device_Iso15693FmtBlockSize(pInfo);
			}
			break;
		case DEVICE_OPERATION_READ:
           	state = Device_Iso15693FmtReadBlock(pInfo);
            if(state == ISO15693_ERR_SUC)
            {
				if(pOpInfo->opTagMode == DEVICE_OP_TAG_WRITE)
                {
                    Device_Iso15693MergeBlock(pOpInfo);
                }
                else if(pOpInfo->opTagMode == DEVICE_OP_TAG_READ)
                {
					//数据块映射区域
					 memcpy(pTagMem->block + pInfo->b.rAddr[pInfo->b.rIndex] * pInfo->b.blockSize, pInfo->b.block, pInfo->b.opNum * pInfo->b.blockSize);
					 if(pOpInfo->memLen <= DEVICE_BLOCK_LEN_15693)
					 {	//数据块上报缓存区域
					 	memcpy(pTagMem->tag15693Info.block, pTagMem->block + pOpInfo->memAddr, pOpInfo->memLen);
						memcpy(Tag15693Block, pTagMem->block + pOpInfo->memAddr, pOpInfo->memLen);
					 }
                }
                rlt = ISO15693_ERR_SUC;
            }
            break;
        case DEVICE_OPERATION_WRITE:
            state = Device_Iso15693FmtWriteBlock(pInfo);
            if(state == ISO15693_ERR_SUC)
            {
                rlt = ISO15693_ERR_SUC;
            }
            break;
		default:
			break;
	}
	
	pInfo->rlt = rlt;
}

BOOL Device_Step(DEVICE_OP *pOpInfo)
{
	BOOL rlt = FALSE;
	if(g_sDeviceParams.rfParams.protocol == DEVICE_OP_TAG_15693)
    {
        rlt = Device_Iso15693Step(pOpInfo);
    }
    else
    {
       // rlt = Reader_Iso14443aStep(pOpInfo);
    }
	return rlt;
}

BOOL Device_Iso15693Step(DEVICE_OP *pOpInfo)
{
	BOOL b = TRUE;
	u8 op = 0, rlt = 0;
	static u32 oldState = 0;;
    ISO15693_INFO *pInfo = NULL;
    
    pInfo = &pOpInfo->protocol.iso15693;
    op = pOpInfo->operation[pOpInfo->index];
    rlt = pInfo->rlt;
	
	switch(op)
    {
		case DEVICE_OPERATION_INVT:
			if(rlt == ISO15693_ERR_SUC)
            {
                pOpInfo->index++;
				//-----
				pOpInfo->delay = g_nSysTick;
				if(oldState != DEVICE_STAT_TAG_EXIXT)
				{
					g_sDeviceTagMemory.flag = 0x01;
					a_SetStateBit(g_nDeviceState, DEVICE_STAT_ALARMLED);
					oldState = DEVICE_STAT_TAG_EXIXT;
				}
				Device_SetSate(DEVICE_STAT_TAG_EXIXT);
				//-----
                if(pOpInfo->opTagMode == DEVICE_OP_TAG_READ)
                {
                    Device_Iso15693GetReadBlockAddr(&pInfo->b, pOpInfo->memAddr, pOpInfo->memLen);
                }
                else if(pOpInfo->opTagMode == DEVICE_OP_TAG_WRITE)
                {
                    Device_Iso15693GetWriteBlockAddr(&pInfo->b, pOpInfo->memAddr, pOpInfo->memLen);
                    if(pInfo->b.rNum == 0)                                  //不需要读数据块
                    {
                        pOpInfo->index++;                                 //跳过读操作
                    }
                }
            }
            else
            {
                //盘点超时，即结束，否则继续盘点
                if(pOpInfo->delay + (g_sDeviceParams.workTim.invtTim << 2) <= g_nSysTick)
                {
                    b = FALSE;
					if(Device_ChkSate(DEVICE_STAT_TAG_EXIXT))
					{
						oldState = DEVICE_STAT_TAG_NO_EXIXT;
						Device_ClaSate(DEVICE_STAT_TAG_EXIXT);
						memset(&g_sDeviceTagMemory.tag15693Info, 0, sizeof(PROTOCOL_15693INFO));
						memset(&g_sDeviceTagMemory.block, 0, DEVICE_BLK_MEM_SIZE);     //不样的标签
					}
                }
            }
			break;
		case DEVICE_OPERATION_READ:
            if(rlt == ISO15693_ERR_SUC)
            {
                pInfo->repeat = 0;
                pInfo->b.rIndex += pInfo->b.opNum;
                if(pInfo->b.rIndex >= pInfo->b.rNum)
                {
                    pOpInfo->index++;
                }
            }
            else
            {
                pInfo->repeat++;
                if(pInfo->repeat >= ISO15693_OP_REPEAT)
                {
                    pInfo->repeat = 0;
                    b = FALSE;
                }
            }
            break;
		case DEVICE_OPERATION_WRITE:
            if(rlt == ISO15693_ERR_SUC)
            {
                pInfo->repeat = 0;
                pInfo->b.wIndex += pInfo->b.opNum;
                if(pInfo->b.wIndex >= pInfo->b.wNum)
                {
                    pOpInfo->index++;
                }
            }
            else
            {
                pInfo->repeat++;
                if(pInfo->repeat >= ISO15693_OP_REPEAT)
                {
                    pInfo->repeat = 0;
                    b = FALSE;
                }
            }
            break;
		default:
			if(rlt == ISO15693_ERR_SUC)
            {
                pInfo->repeat = 0;
                pOpInfo->index++;
            }
            else
            {
                pInfo->repeat++;
                if(pInfo->repeat >= ISO15693_OP_REPEAT)
                {
                    pInfo->repeat = 0;
                    b = FALSE;
                }
            }
			break;
	}
	
	return b;
}

void Device_TestSlove()
{
	DEVICE_OPTAGINFO *pOpTagInfo;
	
	pOpTagInfo = &g_sDeviceOpTagInfo;
	if(pOpTagInfo->state.tagFlag == DEVICE_OP_TAG_OK /*&& !(DeviceParams_TagOpState & 0x01)*/)
	{
		g_sDeviceInfo.testInfo.okTick ++;	
		g_sDeviceParams.workTim.idleTim = 5;
	}
	else
	{
		g_sDeviceParams.workTim.idleTim = 0;
	}

}
void Device_Iso15693SaveUid(u8 *pUid, DEVICE_TAGMEM *pTagMem)
{
    u16 uidReg[ISO15693_SIZE_UID >> 1] = {0};
	
    memcpy(uidReg, pTagMem->tag15693Info.uid, ISO15693_SIZE_UID);

	pTagMem->tag15693Info.uid[0] = pUid[7]; pTagMem->tag15693Info.uid[1] = pUid[6];
	pTagMem->tag15693Info.uid[2] = pUid[5]; pTagMem->tag15693Info.uid[3] = pUid[4];
    pTagMem->tag15693Info.uid[4] = pUid[3]; pTagMem->tag15693Info.uid[5] = pUid[2];
	pTagMem->tag15693Info.uid[6] = pUid[1]; pTagMem->tag15693Info.uid[7] = pUid[0];
	//--
    //不一样的UID，清空block存储区
    if(memcmp(uidReg, pTagMem->tag15693Info.uid, ISO15693_SIZE_UID) )
    {
		memset(pTagMem->tag15693Info.block, 0, DEVICE_BLOCK_LEN_15693);     //不样的标签
    	memset(Tag15693Block, 0, DEVICE_BLOCK_LEN_15693);
	}
}

void Device_Iso15693FmtBlockSize(ISO15693_INFO *pInfo)
{
    pInfo->b.blockSize = ISO15693_SIZE_BLOCK_4B;
    //富士通的特殊处理
    if(pInfo->u[ISO15693_UID_TYPE_POS] == ISO15693_TAG_TYPE_FST)
    {
        if(pInfo->u[ISO15693_UID_CODE_POS] == 0x01)         //8-byte/block configuration, 256 blocks
        {
            pInfo->b.blockSize = ISO15693_SIZE_BLOCK_8B;
        }
        else if(pInfo->u[ISO15693_UID_CODE_POS] == 0x05)    //32-byte/block configuration, 256 blocks
        {
            pInfo->b.blockSize = ISO15693_SIZE_BLOCK_32B;
        }
    }
}


u8 Device_Iso15693FmtReadBlock(ISO15693_INFO *pInfo)
{
    u8 state = ISO15693_ERR_SUC, blockNum = 0, i = 0, opNum = 0, rAddr = 0;

    blockNum = g_sDeviceParams.rfParams.rBlkNum + 1;
    rAddr = pInfo->b.rAddr[pInfo->b.rIndex];
    opNum = 1;
    for(i = 1; i < blockNum; i++)
    {
        if((pInfo->b.rIndex + i < pInfo->b.rNum) && (pInfo->b.rAddr[pInfo->b.rIndex + i] == rAddr + i))        //地址连续判断
        {
            opNum++;
        }
        else
        {
            break;
        }
    }
    pInfo->b.opNum = opNum;
    state = ISO15693_ReadBlocks(pInfo->u, opNum, rAddr, pInfo->b.block, pInfo->b.blockSize);
    
    return state;
}



void Device_Iso15693MergeBlock(DEVICE_OP *pDeviceOp)
{
    ISO15693_INFO *pInfo = NULL;
    u16 startAddr = 0, endAddr = 0;

    startAddr = pDeviceOp->memAddr;
    endAddr = startAddr + pDeviceOp->memLen;
    
    pInfo = &pDeviceOp->protocol.iso15693;
    if(pInfo->b.blockSize == ISO15693_SIZE_BLOCK_4B) 
    {
        //合拼块
        if(pInfo->b.rIndex == 0)        //第一个块或最后一个块
        {
            memcpy(g_sDeviceTagMemory.block + (startAddr & 0xFFFC), pInfo->b.block, startAddr & 0x0003);
        }
        else if(pInfo->b.rIndex == 1)   //最后一个块
        {
            memcpy(g_sDeviceTagMemory.block + endAddr, pInfo->b.block + (endAddr & 0x0003), ISO15693_SIZE_BLOCK_4B - (endAddr & 0x0003));
        }
    }
    else if(pInfo->b.blockSize == ISO15693_SIZE_BLOCK_8B) 
    {
        //合拼块
        if(pInfo->b.rIndex == 0)        //第一个块或最后一个块
        {
            memcpy(g_sDeviceTagMemory.block + (startAddr & 0xFFF8), pInfo->b.block, startAddr & 0x0007);
        }
        else if(pInfo->b.rIndex == 1)   //最后一个块
        {
            memcpy(g_sDeviceTagMemory.block + endAddr, pInfo->b.block + (endAddr & 0x0007), ISO15693_SIZE_BLOCK_8B - (endAddr & 0x0007));
        }
    }
    else if(pInfo->b.blockSize == ISO15693_SIZE_BLOCK_32B) 
    {
        //合拼块
        if(pInfo->b.rIndex == 0)        //第一个块或最后一个块
        {
            memcpy(g_sDeviceTagMemory.block + (startAddr & 0xFFE0), pInfo->b.block, startAddr & 0x001F);
        }
        else if(pInfo->b.rIndex == 1)   //最后一个块
        {
            memcpy(g_sDeviceTagMemory.block + endAddr, pInfo->b.block + (endAddr & 0x001F), ISO15693_SIZE_BLOCK_32B - (endAddr & 0x001F));
        }
    }
}



void Device_Iso15693GetBlock(u16 blockSize, u8 addr, u8 num, u8 *pBlock)
{
    u16 memAddr = 0;
    
    if(blockSize == ISO15693_SIZE_BLOCK_4B) 
    {
        memAddr = addr * ISO15693_SIZE_BLOCK_4B;
        memcpy(pBlock, g_sDeviceTagMemory.block + memAddr, num * ISO15693_SIZE_BLOCK_4B);
    }
    else if(blockSize == ISO15693_SIZE_BLOCK_8B) 
    {
        memAddr = addr * ISO15693_SIZE_BLOCK_8B;
        memcpy(pBlock, g_sDeviceTagMemory.block + memAddr, num * ISO15693_SIZE_BLOCK_8B);
    }
    else if(blockSize == ISO15693_SIZE_BLOCK_32B) 
    {
        memAddr = addr * ISO15693_SIZE_BLOCK_32B;
        memcpy(pBlock, g_sDeviceTagMemory.block + memAddr, num * ISO15693_SIZE_BLOCK_32B);
    }
}

u8 Device_Iso15693FmtWriteBlock(ISO15693_INFO *pInfo)
{
    u8 i = 0, opNum = 0, wAddr = 0, blockNum = 0, state = ISO15693_ERR_SUC;
    u16 blockSize = 0;
    
    blockSize = pInfo->b.blockSize;
    blockNum = 1;  
    wAddr = pInfo->b.wAddr[pInfo->b.wIndex];
    opNum = 1;
    for(i = 1; i < blockNum; i++)
    {
        if((pInfo->b.wIndex + i < pInfo->b.wNum) && (pInfo->b.wAddr[pInfo->b.wIndex + i] == wAddr + i))        //地址连续判断，并且数量不超过num
        {
            opNum++;
        }
        else
        {
            break;
        }
    }
    pInfo->b.opNum = opNum;
    
    Device_Iso15693GetBlock(blockSize, pInfo->b.wAddr[pInfo->b.wIndex], opNum, pInfo->b.block);

    if(opNum == 1)
    {
        state = ISO15693_WriteBlock(pInfo->u, opNum, wAddr, pInfo->b.block, blockSize);         //WriteBlock
    }
    else
    {
        state = ISO15693_WriteBlocks(pInfo->u, opNum, wAddr, pInfo->b.block, blockSize);         //WriteBlock
    }
    return state;
}



void Device_Iso15693GetReadBlockAddr(ISO15693_BLOCK *pBlockInfo, u16 memAddr, u16 memLen)
{
    u16 blockAddr = 0, blockNum = 0, endAddr = 0;
    
    endAddr = memAddr + memLen;
    pBlockInfo->rIndex = 0;
    pBlockInfo->rNum = 0;

    if(pBlockInfo->blockSize == ISO15693_SIZE_BLOCK_4B)
    {
        blockAddr = (memAddr & 0xFFFC) >> 2;
        blockNum = (endAddr - (memAddr & 0xFFFC)) >> 2;
        if(endAddr & 0x03)
        {
            blockNum += 1;
        }
    }
    else if(pBlockInfo->blockSize == ISO15693_SIZE_BLOCK_8B)
    {
        blockAddr = (memAddr & 0xFFF8) >> 3;
        blockNum = (endAddr - (memAddr & 0xFFF8)) >> 3;
        if(endAddr & 0x0F)
        {
            blockNum += 1;
        }
    }
    else if(pBlockInfo->blockSize == ISO15693_SIZE_BLOCK_32B)
    {
        blockAddr = (memAddr & 0xFFE0) >> 5;
        blockNum = (endAddr - (memAddr & 0xFFE0)) >> 5;
        if(endAddr & 0xFFE0)
        {
            blockNum += 1;
        }
    }
    pBlockInfo->rNum = blockNum;
    ISO15693_AddContinueAddr(pBlockInfo->rAddr, blockAddr, blockNum);
}

void Device_Iso15693GetWriteBlockAddr(ISO15693_BLOCK *pBlockInfo, u16 memAddr, u16 memLen)
{
    u16 blockAddr = 0, blockNum = 0, endAddr = 0;

    endAddr = memAddr + memLen;
    pBlockInfo->rIndex = 0;
    pBlockInfo->rNum = 0;
    pBlockInfo->wIndex = 0;
    pBlockInfo->wNum = 0;
    if(pBlockInfo->blockSize == ISO15693_SIZE_BLOCK_4B) 
    {
        blockAddr = (memAddr & 0xFFFC) >> 2;
        blockNum = (endAddr - (memAddr & 0xFFFC)) >> 2;
        if(endAddr & 0x0003)
        {
            blockNum += 1;
        }
        
        if(memAddr & 0x0003)    //起始块只要写入半个块（一个寄存器），需要先读取
        {
            pBlockInfo->rAddr[pBlockInfo->rNum++] = (memAddr & 0xFFFC) >> 2;
        }
        if(endAddr & 0x0003)    //起始块只要写入半个块（一个寄存器），需要先读取
        {
            pBlockInfo->rAddr[pBlockInfo->rNum++] = (endAddr & 0xFFFC) >> 2;
        }
    }
    else if(pBlockInfo->blockSize == ISO15693_SIZE_BLOCK_8B) 
    {
        blockAddr = (memAddr & 0xFFF8) >> 3;
        blockNum = (endAddr - (memAddr & 0xFFF8)) >> 3;
        if(endAddr & 0x0007)
        {
            blockNum += 1;
        }

        if(memAddr & 0x0007)    //起始块只要写入半个块（一个寄存器），需要先读取
        {
            pBlockInfo->rAddr[pBlockInfo->rNum++] = (memAddr & 0xFFF8) >> 3;
        }
        if(endAddr & 0x0007)    //起始块只要写入半个块（一个寄存器），需要先读取
        {
            pBlockInfo->rAddr[pBlockInfo->rNum++] = (endAddr & 0xFFF8) >> 3;
        }
    }
    else if(pBlockInfo->blockSize == ISO15693_SIZE_BLOCK_32B) 
    {
        blockAddr = (memAddr & 0xFFE0) >> 5;
        blockNum = (endAddr - (memAddr & 0xFFE0)) >> 5;
        if(endAddr & 0xFFE0)
        {
            blockNum += 1;
        }

        if(memAddr & 0x001F)    //起始块只要写入半个块（一个寄存器），需要先读取
        {
            pBlockInfo->rAddr[pBlockInfo->rNum++] = (memAddr & 0xFFE0) >> 5;
        }
        if(endAddr & 0x001F)    //起始块只要写入半个块（一个寄存器），需要先读取
        {
            pBlockInfo->rAddr[pBlockInfo->rNum++] = (endAddr & 0xFFE0) >> 5;
        }
    }
    pBlockInfo->wNum = blockNum;
    ISO15693_AddContinueAddr(pBlockInfo->wAddr, blockAddr, blockNum);
}

void ISO15693_AddContinueAddr(u8 *pAddr, u16 start, u16 num)
{
    u16 i = 0;
    for(i = 0; i < num; i++)
    {
        pAddr[i] = start + i;
    }
}

void Device_UpdateBackupTagInfo(DEVICE_OP *pOpInfo)
{
    if(g_sDeviceParams.rfParams.protocol == DEVICE_OP_TAG_15693)
    {
        if(memcmp(g_sDeviceBackupUid.u, pOpInfo->protocol.iso15693.u, ISO15693_SIZE_UID) != 0)
        {
            memcpy(g_sDeviceBackupUid.u, pOpInfo->protocol.iso15693.u, ISO15693_SIZE_UID);
        }
		g_sDeviceBackupUid.keepTick = g_nSysTick;
	}
    else
    {
       /* if(memcmp(g_sReaderBackupUid.u, pOpInfo->protocol.iso14443a.u.uid, ISO15693_SIZE_UID) != 0)   //14443a的UID也不会超过8字节
        {
            memcpy(g_sReaderBackupUid.u, pOpInfo->protocol.iso14443a.u.uid, ISO15693_SIZE_UID);
        }*/
    	g_sDeviceBackupUid.keepTick = g_nSysTick;
	}
	
}

void Device_UpdateOpTagInfo(DEVICE_OP *pOpInfo, DEVICE_OPTAGINFO *pOpTagInfo)
{
    if(pOpInfo->bAutoOp == FALSE)
    {
        pOpTagInfo->state.busy = 0;
        pOpTagInfo->state.over = DEVICE_OP_TAG_OVER;
        if(pOpInfo->bOpOk)
        {
            pOpTagInfo->state.err = 0;
            if(pOpTagInfo->state.rw != DEVICE_OP_TAG_WBLK)  //读操作
            {
				//组帧
                //memcpy(pOpTagInfo->tagMem, g_sDeviceTagMemory.uid + pOpTagInfo->addr + ISO15693_SIZE_UID, pOpTagInfo->len);
            	//memcpy(&g_sDeviceInfo.opInfo.rspBuffer, &g_sDeviceTagMemory.tag15693Info.block, pOpTagInfo->len);
				//memcpy(&Tag15693Block, g_sDeviceTagMemory.block + pOpInfo->memAddr, pOpInfo->memLen);
				Device_TagFrameClear();
				Device_TagReadFormat(pOpInfo);
				
			}
        }
        else
        {
            pOpTagInfo->state.err = DEVICE_OP_TAG_ERR;
        }
    }
    else
    {
        if(pOpInfo->bOpOk)
        {
            pOpTagInfo->state.tagFlag = DEVICE_OP_TAG_OK;
        }
        else
        {
            pOpTagInfo->state.tagFlag = 0;
        }
    }
}

BOOL Device_ProcessCanFrame()
{
	BOOL bOk = TRUE;
	TAG_OPINFO *pOpInfo = NULL;
	DEVICE_OPTAGINFO *pOpTagInfo;
	
	pOpTagInfo = &g_sDeviceOpTagInfo;
	pOpInfo = &g_sDeviceInfo.opInfo;
	/*pOpInfo->op = rcvFrame->buffer[DEVICE_CAN_FRAME_OP];
	pOpTagInfo->addr = *(u16 *)(rcvFrame->buffer + DEVICE_CAN_FRAME_ADDR);
	pOpTagInfo->len = *(u16 *)(rcvFrame->buffer + DEVICE_CAN_FRAME_LENTH);
	
	if(pOpTagInfo->len <= DEVICE_CAN_COM_LEN)
	{
		pOpTagInfo->state.frame = 0;		
	}
	else
	{
		pOpTagInfo->state.frame = DEVICE_OP_TAG_FRAME_ERR;		//帧错误
	}*/
	
	//		多帧处理
	pOpTagInfo->addr = DeviceParams_OpAddr;
	pOpTagInfo->len = DeviceParams_OpLen;
	pOpInfo->op = DeviceParams_OpDevice;
	//
	switch(pOpInfo->op)
	{
		case DEVICE_FRAME_CMD_ENABLE_READ_BLOCK:
				Device_TagFrameClear();						//读数据钱清空缓冲区
				Device_ReadTagMemory(&g_sDeviceOpInfo, &g_sDeviceOpTagInfo);
			break;
		case DEVICE_FRAME_CMD_ENABLE_WRITE_BLOCK:
				Device_TagWriteFormat(pOpTagInfo);
				Device_WriteTagMemory(&g_sDeviceOpInfo, &g_sDeviceOpTagInfo);
			break;
		case DEVICE_FRAME_CMD_RESET:
				Device_SetSate(DEVICE_STAT_REST);
			break;
		/*case DEVICE_FRAME_CMD_ENABLE_BOOT:
				Device_SetSate(DEVICE_STAT_BOOT);
			break;*/
		case DEVICE_FRAME_CMD_CLEAR_OPFRAME:
				DeviceParams_OpLen = 0;
				DeviceParams_OpAddr = 0;
				Device_TagFrameClear();
			break;
        case DEVICE_FRAME_CMD_RESET_DEFULTPARAMS:
        		Device_ResetDeviceParamenter();
            Device_WriteDeviceParamenter();
        break;
	default:
			 bOk = FALSE;
			break;
	}
	DeviceParams_OpDevice = 0;
	return bOk;
}

void Device_TagReadFormat(DEVICE_OP *pOpInfo)
{
	if(pOpInfo->memLen <= 1 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(&DeviceParams_OpFrame0, g_sDeviceTagMemory.block + pOpInfo->memAddr + 0 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen + 0 * DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpInfo->memLen <= 2 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(&DeviceParams_OpFrame0, g_sDeviceTagMemory.block + pOpInfo->memAddr + 0 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen + 0 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame1, g_sDeviceTagMemory.block + pOpInfo->memAddr + 1 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 1 * DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpInfo->memLen <= 3 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(&DeviceParams_OpFrame0, g_sDeviceTagMemory.block + pOpInfo->memAddr + 0 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen + 0 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame1, g_sDeviceTagMemory.block + pOpInfo->memAddr + 1 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 1 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame2, g_sDeviceTagMemory.block + pOpInfo->memAddr + 2 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 2 * DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpInfo->memLen <= DEVICE_TAG_ONE_BLOCK_LEN * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(&DeviceParams_OpFrame0, g_sDeviceTagMemory.block + pOpInfo->memAddr + 0 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen + 0 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame1, g_sDeviceTagMemory.block + pOpInfo->memAddr + 1 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 1 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame2, g_sDeviceTagMemory.block + pOpInfo->memAddr + 2 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 2 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame3, g_sDeviceTagMemory.block + pOpInfo->memAddr + 3 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 3 * DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpInfo->memLen <= DEVICE_TAG_ONE_BLOCK_LEN * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(&DeviceParams_OpFrame0, g_sDeviceTagMemory.block + pOpInfo->memAddr + 0 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen + 0 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame1, g_sDeviceTagMemory.block + pOpInfo->memAddr + 1 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 1 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame2, g_sDeviceTagMemory.block + pOpInfo->memAddr + 2 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 2 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame3, g_sDeviceTagMemory.block + pOpInfo->memAddr + 3 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 3 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame4, g_sDeviceTagMemory.block + pOpInfo->memAddr + 4 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 4 * DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpInfo->memLen <= 5 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(&DeviceParams_OpFrame0, g_sDeviceTagMemory.block + pOpInfo->memAddr + 0 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen + 0 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame1, g_sDeviceTagMemory.block + pOpInfo->memAddr + 1 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 1 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame2, g_sDeviceTagMemory.block + pOpInfo->memAddr + 2 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 2 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame3, g_sDeviceTagMemory.block + pOpInfo->memAddr + 3 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 3 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame4, g_sDeviceTagMemory.block + pOpInfo->memAddr + 4 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 4 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame5, g_sDeviceTagMemory.block + pOpInfo->memAddr + 5 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 5 * DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpInfo->memLen <= 6 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(&DeviceParams_OpFrame0, g_sDeviceTagMemory.block + pOpInfo->memAddr + 0 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen + 0 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame1, g_sDeviceTagMemory.block + pOpInfo->memAddr + 1 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 1 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame2, g_sDeviceTagMemory.block + pOpInfo->memAddr + 2 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 2 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame3, g_sDeviceTagMemory.block + pOpInfo->memAddr + 3 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 3 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame4, g_sDeviceTagMemory.block + pOpInfo->memAddr + 4 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 4 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame5, g_sDeviceTagMemory.block + pOpInfo->memAddr + 5 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 5 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame6, g_sDeviceTagMemory.block + pOpInfo->memAddr + 6 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 6 * DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpInfo->memLen <= 7 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(&DeviceParams_OpFrame0, g_sDeviceTagMemory.block + pOpInfo->memAddr + 0 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen + 0 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame1, g_sDeviceTagMemory.block + pOpInfo->memAddr + 1 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 1 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame2, g_sDeviceTagMemory.block + pOpInfo->memAddr + 2 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 2 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame3, g_sDeviceTagMemory.block + pOpInfo->memAddr + 3 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 3 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame4, g_sDeviceTagMemory.block + pOpInfo->memAddr + 4 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 4 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame5, g_sDeviceTagMemory.block + pOpInfo->memAddr + 5 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 5 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame6, g_sDeviceTagMemory.block + pOpInfo->memAddr + 6 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 6 * DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(&DeviceParams_OpFrame6, g_sDeviceTagMemory.block + pOpInfo->memAddr + 7 * DEVICE_TAG_ONE_BLOCK_LEN, pOpInfo->memLen - 7 * DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else
	{
	
	}
	
}

void Device_TagWriteFormat(DEVICE_OPTAGINFO *pOpTagInfo)
{
	if(pOpTagInfo->len <= 1 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(pOpTagInfo->tagMem + 0 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame0, DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpTagInfo->len <= 2 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(pOpTagInfo->tagMem + 0 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame0, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 1 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame1, DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpTagInfo->len <= 3 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(pOpTagInfo->tagMem + 0 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame0, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 1 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame1, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 2 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame2, DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpTagInfo->len <= 4 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(pOpTagInfo->tagMem + 0 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame0, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 1 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame1, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 2 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame2, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 3 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame3, DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpTagInfo->len <= 5 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(pOpTagInfo->tagMem + 0 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame0, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 1 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame1, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 2 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame2, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 3 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame3, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 4 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame4, DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpTagInfo->len <= 6 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(pOpTagInfo->tagMem + 0 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame0, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 1 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame1, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 2 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame2, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 3 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame3, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 4 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame4, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 5 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame5, DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpTagInfo->len <= 7 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(pOpTagInfo->tagMem + 0 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame0, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 1 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame1, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 2 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame2, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 3 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame3, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 4 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame4, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 5 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame5, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 6 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame6, DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else if(pOpTagInfo->len <= 8 * DEVICE_TAG_ONE_BLOCK_LEN)
	{
		memcpy(pOpTagInfo->tagMem + 0 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame0, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 1 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame1, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 2 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame2, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 3 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame3, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 4 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame4, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 5 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame5, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 6 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame6, DEVICE_TAG_ONE_BLOCK_LEN);
		memcpy(pOpTagInfo->tagMem + 7 * DEVICE_TAG_ONE_BLOCK_LEN, &DeviceParams_OpFrame7, DEVICE_TAG_ONE_BLOCK_LEN);
	}
	else
	{
	
	}
	
}

void Device_ReadTagMemory(DEVICE_OP *pOpInfo, DEVICE_OPTAGINFO *pOpTagInfo)
{
    if(g_sDeviceParams.rfParams.protocol == DEVICE_OP_TAG_15693)
    {
        Device_Iso15693ReadTagMemory(pOpInfo, pOpTagInfo);
    }
    else
    {
        //Reader_Iso14443aReadTagMemory(pOpInfo, pOpTagInfo);
    }
}

void Device_WriteTagMemory(DEVICE_OP *pOpInfo, DEVICE_OPTAGINFO *pOpTagInfo)
{
    if(g_sDeviceParams.rfParams.protocol == DEVICE_OP_TAG_15693)
    {
        Device_Iso15693WriteTagMemory(pOpInfo, pOpTagInfo);
    }
    else
    {
        //Reader_Iso14443aReadTagMemory(pOpInfo, pOpTagInfo);
    }
}



void Device_Iso15693ReadTagMemory(DEVICE_OP *pOpInfo, DEVICE_OPTAGINFO *pOpTagInfo)
{
    short temp = 0;
    //如果是自动识别标签协议，进入这个函数表示已经读取了15693标签，可以不用复位或读取UID了
    Device_ResetOp();
    St25r3916_CloseAntenna();
    Device_Delayms(1);
    St25r3916_OpenAntenna();
    Device_Delayms(1);
	a_SetState(g_sDeviceInfo.rfidFlag, DEVICE_RF_FLAG_OPEN);

    pOpInfo->opTagMode = DEVICE_OP_TAG_READ;

    pOpInfo->operation[pOpInfo->num++] = DEVICE_OPERATION_INVT;
    pOpInfo->operation[pOpInfo->num++] = DEVICE_OPERATION_READ;

    temp = (short)pOpTagInfo->addr;
    if(temp < 0)
    {
        pOpInfo->memAddr = 0;
        pOpInfo->memLen = pOpTagInfo->addr + pOpTagInfo->len;
    }
    else
    {
        pOpInfo->memAddr = pOpTagInfo->addr;
        pOpInfo->memLen = pOpTagInfo->len;
    }
    pOpTagInfo->state.busy = 1;
    pOpTagInfo->state.over = 0;
    pOpTagInfo->state.err = 0;
    pOpTagInfo->state.rw = 0;
    
    pOpInfo->index = 0; 
    pOpInfo->step = 0;
    
    pOpInfo->bAutoOp = FALSE;
    pOpInfo->delay = g_nSysTick;
    a_SetState(pOpInfo->state, DEVICE_STAT_TX);
}

void Device_Iso15693WriteTagMemory(DEVICE_OP *pOpInfo, DEVICE_OPTAGINFO *pOpTagInfo)
{
    short temp = 0;

    Device_ResetOp();
    St25r3916_CloseAntenna();
    Device_Delayms(1);
    St25r3916_OpenAntenna();
    Device_Delayms(1);
	a_SetState(g_sDeviceInfo.rfidFlag, DEVICE_RF_FLAG_OPEN);

    pOpInfo->opTagMode = DEVICE_OP_TAG_WRITE;

    pOpInfo->operation[pOpInfo->num++] = DEVICE_OPERATION_INVT;
    pOpInfo->operation[pOpInfo->num++] = DEVICE_OPERATION_READ;     //默认读取收尾block
    pOpInfo->operation[pOpInfo->num++] = DEVICE_OPERATION_WRITE;

    temp = (short)pOpTagInfo->addr;
    if(temp < 0)
    {
        pOpInfo->memAddr = 0;
        pOpInfo->memLen = pOpTagInfo->addr + pOpTagInfo->len;
    }
    else
    {
        pOpInfo->memAddr = pOpTagInfo->addr;
        pOpInfo->memLen = pOpTagInfo->len;
    }
    memcpy(g_sDeviceTagMemory.block + pOpInfo->memAddr, pOpTagInfo->tagMem, pOpInfo->memLen);
    
    pOpTagInfo->state.busy = 1;
    pOpTagInfo->state.over = 0;
    pOpTagInfo->state.err = 0;
    pOpTagInfo->state.rw = DEVICE_OP_TAG_WBLK;
    
    pOpInfo->index = 0;  
    pOpInfo->step = 0;
    
    pOpInfo->bAutoOp = FALSE;
    pOpInfo->delay = g_nSysTick;
    a_SetState(pOpInfo->state, DEVICE_STAT_TX);
}

void Device_DataUpdate()				//映射数据更新
{
	//	速度过快是否可能监控不到？？可增加次数监控
    if(a_CheckStateBit(g_nPdoFlag, PDO_FLAG_PARAMS_OP))
    {
        Device_ParamsUpdataSave(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.transmissionType, AnyId_FR321CO_Slave_obj1800_Transmission_Type);
        Device_ParamsUpdataSave(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.compatibilityEntry, AnyId_FR321CO_Slave_obj1800_Compatibility_Entry);
        Device_ParamsUpdataSave(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_0].comParams.eventTimer, AnyId_FR321CO_Slave_obj1800_Event_Timer);

        Device_ParamsUpdataSave(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.transmissionType, AnyId_FR321CO_Slave_obj1801_Transmission_Type);
        Device_ParamsUpdataSave(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.compatibilityEntry, AnyId_FR321CO_Slave_obj1801_Compatibility_Entry);
        Device_ParamsUpdataSave(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_1].comParams.eventTimer, AnyId_FR321CO_Slave_obj1801_Event_Timer);

        Device_ParamsUpdataSave(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.transmissionType, AnyId_FR321CO_Slave_obj1802_Transmission_Type);
        Device_ParamsUpdataSave(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.compatibilityEntry, AnyId_FR321CO_Slave_obj1802_Compatibility_Entry);
        Device_ParamsUpdataSave(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_2].comParams.eventTimer, AnyId_FR321CO_Slave_obj1802_Event_Timer);

        Device_ParamsUpdataSave(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.transmissionType, AnyId_FR321CO_Slave_obj1803_Transmission_Type);
        Device_ParamsUpdataSave(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.compatibilityEntry, AnyId_FR321CO_Slave_obj1803_Compatibility_Entry);
        Device_ParamsUpdataSave(g_sDeviceParams.canOpneParams.tPdoParams[CANOPEN_POD_INDEX_3].comParams.eventTimer, AnyId_FR321CO_Slave_obj1803_Event_Timer);
        
        a_ClearStateBit(g_nPdoFlag, PDO_FLAG_PARAMS_OP);        //参数数据耗时较大？？？？
    }
	//noSave
	Device_ParamsUpdata(g_sDeviceParams.addr, HardParams_DeviceAddr);
	Device_ParamsUpdata(g_sDeviceParams.bud, HardParams_Bud);
	Device_ParamsUpdata(g_sDeviceParams.canFilterPara.CAN_FilterMode, HardParams_CanFilterMode);
	Device_ParamsUpdata(g_sDeviceParams.canFilterPara.CAN_FilterIdLow, HardParams_CanFilterIdLow);
	Device_ParamsUpdata(g_sDeviceParams.canFilterPara.CAN_FilterIdHigh, HardParams_CanFilterIdHigh);
	Device_ParamsUpdata(g_sDeviceParams.canFilterPara.CAN_FilterMaskIdLow, HardParams_CanFilterMaskIdLow);
	Device_ParamsUpdata(g_sDeviceParams.canFilterPara.CAN_FilterMaskIdHigh, HardParams_CanFilterMaskIdHigh);
	Device_ParamsUpdata(g_sDeviceParams.rfu, HardParams_RfidRfu);
	
	Device_ParamsUpdata(g_sDeviceParams.rfParams.protocol, RfidParams_Protocol);
	Device_ParamsUpdata(g_sDeviceParams.rfParams.rBlkNum, RfidParams_RBlkNum);
	Device_ParamsUpdata(g_sDeviceParams.autoBlockParams.addr, RfidParams_AutoAddr);
	Device_ParamsUpdata(g_sDeviceParams.autoBlockParams.len, RfidParams_AutoLen);
	Device_ParamsUpdata(g_sDeviceParams.workTim.invtTim, RfidParams_InvtTim);
	Device_ParamsUpdata(g_sDeviceParams.workTim.idleTim, RfidParams_IdleTim);
	Device_ParamsUpdata(g_sDeviceParams.workTim.keepTim, RfidParams_KeepTim);
	Device_ParamsUpdata(g_sDeviceParams.rfParams.rfu, RfidParams_RfidRfu);

	//Device_ParamsUpdata(DeviceParams_Nodestate, 0x0);
	//Device_ParamsUpdata(DeviceParams_AutoSate, 0x0);
	//Device_ParamsUpdata(DeviceParams_Devicerfu, 0x0);
	
    //运行数据
	Device_ParamsUpdata(DeviceParams_TagOpState, *(u8 *)&g_sDeviceOpTagInfo.state);
	if(memcmp(&Tag15693_UidLow, &g_sDeviceTagMemory.tag15693Info.uid, ISO15693_SIZE_UID / 2) || 
	   memcmp(&Tag15693_UidHigh, &g_sDeviceTagMemory.tag15693Info.uid[4], ISO15693_SIZE_UID / 2))
	{
		memcpy(&Tag15693_UidLow, &g_sDeviceTagMemory.tag15693Info.uid, ISO15693_SIZE_UID / 2);
		memcpy(&Tag15693_UidHigh, &g_sDeviceTagMemory.tag15693Info.uid[4], ISO15693_SIZE_UID / 2);
	}
	if(g_sDeviceParams.autoBlockParams.len)
	{
		Tag15693_Block0 = (g_sDeviceTagMemory.block[0] << 24) | (g_sDeviceTagMemory.block[1] << 16) | 
						  (g_sDeviceTagMemory.block[2] << 8) | (g_sDeviceTagMemory.block[3] << 0);
		Tag15693_Block1 = (g_sDeviceTagMemory.block[4] << 24) | (g_sDeviceTagMemory.block[5] << 16) | 
						  (g_sDeviceTagMemory.block[6] << 8) | (g_sDeviceTagMemory.block[7] << 0);		
		Tag15693_Block2 = (g_sDeviceTagMemory.block[8] << 24) | (g_sDeviceTagMemory.block[9] << 16) | 
						  (g_sDeviceTagMemory.block[10] << 8) | (g_sDeviceTagMemory.block[11] << 0);		
		Tag15693_Block3 = (g_sDeviceTagMemory.block[12] << 24) | (g_sDeviceTagMemory.block[13] << 16) | 
						  (g_sDeviceTagMemory.block[14] << 8) | (g_sDeviceTagMemory.block[15] << 0);		
		Tag15693_Block4 = (g_sDeviceTagMemory.block[16] << 24) | (g_sDeviceTagMemory.block[17] << 16) | 
						  (g_sDeviceTagMemory.block[18] << 8) | (g_sDeviceTagMemory.block[19] << 0);		
		Tag15693_Block5 = (g_sDeviceTagMemory.block[20] << 24) | (g_sDeviceTagMemory.block[21] << 16) | 
						  (g_sDeviceTagMemory.block[22] << 8) | (g_sDeviceTagMemory.block[23] << 0);		
		Tag15693_Block6 = (g_sDeviceTagMemory.block[24] << 24) | (g_sDeviceTagMemory.block[25] << 16) | 
						  (g_sDeviceTagMemory.block[26] << 8) | (g_sDeviceTagMemory.block[27] << 0);		
		Tag15693_Block7 = (g_sDeviceTagMemory.block[28] << 24) | (g_sDeviceTagMemory.block[29] << 16) | 
						  (g_sDeviceTagMemory.block[30] << 8) | (g_sDeviceTagMemory.block[31] << 0);		
	}
	
	Device_ParamsUpdata(ComData_TestState, g_sDeviceInfo.testInfo.testEnable);
	Device_ParamsUpdata(ComData_OkTick, g_sDeviceInfo.testInfo.okTick);
	Device_ParamsUpdata(ComData_AlllTick, g_sDeviceInfo.testInfo.allTick);
}

//自定
void Device_FormatCanFrame(u8 type, u8 len, void *value, CAN_FRAME *pFrame)
{
	u8 pos = 0;
	Message *pTxFrame = NULL;
	
	pTxFrame = &(pFrame->txFrame);

	memset(pTxFrame, 0,sizeof(Message));
	pTxFrame->cob_id = pFrame->extId;
	
	if(type == DEVICE_CAN_FEAME_PSPTYPE_STD)
	{
		memcpy(pTxFrame->data + pos, &pFrame->index, 2);
		pos += 2;
		pTxFrame->data[pos ++] = pFrame->subIndex >> 0;
		pTxFrame->data[pos ++] = len;						//此位置可作为有效数据长度
		
		if(len == 1)
		{
			memcpy(pTxFrame->data + pos, value, 1);
		}
		else if(len == 2)
		{
			memcpy(pTxFrame->data + pos, (u16 *)value, 2);	
		}
		else if(len == 4)
		{
			memcpy(pTxFrame->data + pos, (u32 *)value, 4);
		}
		pos += 4;
	}
	else
	{
		memcpy(pTxFrame->data + pos, value, len);
		pos += len;
	}
	pTxFrame->len = pos;
}

u8 Device_CanProcessFrame(Message *pMassage)
{
	u8 err = 1;
	u16 cmd = 0;
	void *value = NULL;
	u32 tempRsp = OD_SUCCESSFUL;
	CAN_FRAME *pFrame = NULL;
	
	pFrame = &g_sCanFrame;
	
	pFrame->index = *(u16 *)(&(pMassage->data[DEVICE_CAN_CUSTOM_FRAME_INDEX]));
	pFrame->subIndex = pMassage->data[DEVICE_CAN_CUSTOM_FRAME_SUBINDEX];
	value = (void *)(&(pMassage->data[DEVICE_CAN_CUSTOM_FRAME_DATA]));
	
	//if((pFrame->extId & 0x0800) == 0x0800)
	{
		cmd = pFrame->extId;
		//cmd = (pFrame->extId & CAN_FRAME_ID_CMD_SCREEN);
		switch(cmd)
		{
			case DEVICE_FRAME_CMD_SET_PARAMS:
				if(Device_CanSetParams(pFrame->index, pFrame->subIndex, value))
				{
					tempRsp = OD_SUCCESSFUL;
					Device_FormatCanFrame(DEVICE_CAN_FEAME_PSPTYPE_STD, DEVICE_CAN_FRAME_LEN_4, (void *)&tempRsp, pFrame);
				}
				else
				{
					tempRsp = OD_VALUE_RANGE_EXCEEDED;
					Device_FormatCanFrame(DEVICE_CAN_FEAME_PSPTYPE_STD, DEVICE_CAN_FRAME_LEN_4, (void *)&tempRsp, pFrame);
				}
				break;
			case DEVICE_FRAME_CMD_GET_PARAMS:
				Device_FormatCanFrame(DEVICE_CAN_FEAME_PSPTYPE_STD, Device_CanGetParams(pFrame->index, pFrame->subIndex, &value), (void *)value, pFrame);
				break;
			case DEVICE_FRAME_CMD_GET_TVER:
				Device_FormatCanFrame(DEVICE_CAN_FEAME_PSPTYPE_EXT, 8, (void *)&DEVICE_VERSION[DEVICE_VERSION_DEVICE_NAME_POS], pFrame);
				break;
			case DEVICE_FRAME_CMD_GET_SVER:
				Device_FormatCanFrame(DEVICE_CAN_FEAME_PSPTYPE_EXT, 8, (void *)&DEVICE_VERSION[DEVICE_VERSION_DEVICE_SOFTWARE_POS], pFrame);
				break;
			case DEVICE_FRAME_CMD_GET_HVER:
				Device_FormatCanFrame(DEVICE_CAN_FEAME_PSPTYPE_EXT, 8, (void *)&DEVICE_VERSION[DEVICE_VERSION_DEVICE_HARDWARE_POS], pFrame);
				break;
			case DEVICE_FRAME_CMD_RESET:
				tempRsp = OD_SUCCESSFUL;			a_SetStateBit(g_nDeviceState, DEVICE_STAT_REST);
				Device_FormatCanFrame(DEVICE_CAN_FEAME_PSPTYPE_STD, DEVICE_CAN_FRAME_LEN_4, (void *)&tempRsp, pFrame);
				break;
			default:
				tempRsp = OD_VALUE_RANGE_EXCEEDED;
				Device_FormatCanFrame(DEVICE_CAN_FEAME_PSPTYPE_STD, DEVICE_CAN_FRAME_LEN_4, (void *)&tempRsp, pFrame);
				break;
		}
	}
	
	return err;
}

u8 Device_CanGetParams(u16 index, u8 subIndex, void **pValue)
{
	u8 len = 0;
	static u32 errInfo = 0;
	DEVICE_PARAMS *pParams = NULL;
	
	pParams = &g_sDeviceParams;
	switch (index) 
	{
		case 0x2000:
			if(subIndex == 0x01)
			{	
				len = DEVICE_CAN_FRAME_LEN_1;
				*pValue = &(pParams->addr);
			}
			else if(subIndex == 0x02)
			{
				len = DEVICE_CAN_FRAME_LEN_1;
				*pValue = &(pParams->bud);
			}
			else if(subIndex == 0x03)
			{
				len = DEVICE_CAN_FRAME_LEN_1;
				*pValue = &(pParams->canFilterPara.CAN_FilterMode);
			}
			else if(subIndex == 0x04)
			{
				len = DEVICE_CAN_FRAME_LEN_2;
				*pValue = (void *)&(pParams->canFilterPara.CAN_FilterIdLow);
			}
			else if(subIndex == 0x05)
			{
				len = DEVICE_CAN_FRAME_LEN_2;
				*pValue = (void *)&(pParams->canFilterPara.CAN_FilterIdHigh);
			}
			else if(subIndex == 0x06)
			{
				len = DEVICE_CAN_FRAME_LEN_2;
				*pValue = (void *)&(pParams->canFilterPara.CAN_FilterMaskIdLow);
			}
			else if(subIndex == 0x07)
			{
				len = DEVICE_CAN_FRAME_LEN_2;
				*pValue = (void *)&(pParams->canFilterPara.CAN_FilterMaskIdHigh);
			}
			else
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				errInfo = OD_VALUE_RANGE_EXCEEDED;
				*pValue = (void *)&errInfo;
			}
			break;
		case 0x2001:
			if(subIndex == 0x01)
			{
				len = DEVICE_CAN_FRAME_LEN_1;
				*pValue = (void *)&(pParams->rfParams.protocol);
			}
			else if(subIndex == 0x02)
			{
				len = DEVICE_CAN_FRAME_LEN_1;
				*pValue =(void *)&(pParams->rfParams.rBlkNum);
			}
			else if(subIndex == 0x03)
			{
				len = DEVICE_CAN_FRAME_LEN_2;
				*pValue = (void *)&(pParams->autoBlockParams.addr);
			}
			else if(subIndex == 0x04)
			{
				len = DEVICE_CAN_FRAME_LEN_2;
				*pValue = (void *)&(pParams->autoBlockParams.len);
			}
			else if(subIndex == 0x05)
			{
				len = DEVICE_CAN_FRAME_LEN_1;
				*pValue = (void *)&(pParams->workTim.invtTim);
			}
			else if(subIndex == 0x06)
			{
				len = DEVICE_CAN_FRAME_LEN_1;
				*pValue = (void *)&(pParams->workTim.invtTim);
			}
			else if(subIndex == 0x07)
			{
				len = DEVICE_CAN_FRAME_LEN_1;
				*pValue = (void *)&(pParams->workTim.keepTim);
			}
			else
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				errInfo = OD_VALUE_RANGE_EXCEEDED;
				*pValue = (void *)&errInfo;
			}
			break;
		case 0x2002:
			if(subIndex == 0x01)
			{
				len = DEVICE_CAN_FRAME_LEN_1;
				*pValue = (void *)&(DeviceParams_TagOpState);
			}
			else if(subIndex == 0x02)
			{
				len = DEVICE_CAN_FRAME_LEN_1;
				*pValue =(void *)&(DeviceParams_OpDevice);
			}
			else if(subIndex == 0x03)
			{
				len = DEVICE_CAN_FRAME_LEN_2;
				*pValue = (void *)&(DeviceParams_OpAddr);
			}
			else if(subIndex == 0x04)
			{
				len = DEVICE_CAN_FRAME_LEN_2;
				*pValue = (void *)&(DeviceParams_OpLen);
			}
			else if(subIndex == 0x05)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(DeviceParams_OpFrame0);
			}
			else if(subIndex == 0x06)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(DeviceParams_OpFrame1);
			}
			else if(subIndex == 0x07)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(DeviceParams_OpFrame2);
			}
			else if(subIndex == 0x08)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(DeviceParams_OpFrame3);
			}
			else if(subIndex == 0x09)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(DeviceParams_OpFrame4);
			}
			else if(subIndex == 0x0A)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(DeviceParams_OpFrame5);
			}
			else if(subIndex == 0x0B)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(DeviceParams_OpFrame6);
			}
			else if(subIndex == 0x0C)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(DeviceParams_OpFrame7);
			}
			else if(subIndex == 0x0D)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(DeviceParams_Rfu);
			}
			else
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				errInfo = OD_VALUE_RANGE_EXCEEDED;
				*pValue = (void *)&errInfo;
			}
			break;
		case 0x2003:
			if(subIndex == 0x01)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(Tag15693_UidLow);
			}
			else if(subIndex == 0x02)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue =(void *)&(Tag15693_UidHigh);
			}
			else if(subIndex == 0x03)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(Tag15693_Block0);
			}
			else if(subIndex == 0x04)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(Tag15693_Block1);
			}
			else if(subIndex == 0x05)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(Tag15693_Block2);
			}
			else if(subIndex == 0x06)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(Tag15693_Block3);
			}
			else if(subIndex == 0x07)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(Tag15693_Block4);
			}
			else if(subIndex == 0x08)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(Tag15693_Block5);
			}
			else if(subIndex == 0x09)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(Tag15693_Block6);
			}
			else if(subIndex == 0x0A)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(Tag15693_Block7);
			}
			else if(subIndex == 0x0B)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(DeviceParams_OpFrame6);
			}
			else if(subIndex == 0x0C)
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				*pValue = (void *)&(DeviceParams_OpFrame7);
			}
			else
			{
				len = DEVICE_CAN_FRAME_LEN_4;
				errInfo = OD_VALUE_RANGE_EXCEEDED;
				*pValue = (void *)&errInfo;
			}
			break;
		case 0x2005:
		case 0x2100:
		default:
			len = DEVICE_CAN_FRAME_LEN_4;
			errInfo = OD_VALUE_RANGE_EXCEEDED;
			*pValue = (void *)&errInfo;
			break;
	}
	return len;
}

BOOL Device_CanSetParams(u16 index, u8 subIndex, void *pValue)
{
	BOOL bOk = FALSE, chagFlag = FALSE;
	DEVICE_PARAMS *pParams = NULL;
	
	pParams = &g_sDeviceParams;
	switch (index) 
	{
		case 0x2000:
			if(subIndex == 0x01)
			{
				if((*(u8 *)pValue) > DEVICE_NMT_NODEID_NUM || (*(u8 *)pValue) == 0)		//不支持地址00
				{
					bOk = FALSE;
				}
				else
				{
					if((pParams->addr) ^ (*(u8 *)pValue))
					{
						pParams->addr = (*(u8 *)pValue);
						chagFlag = TRUE;
						g_sFramBootParamenter.addr = g_sDeviceParams.addr;
						Fram_WriteBootParamenter();
					}
					bOk = TRUE;
				}
			}
			else if(subIndex == 0x02)
			{
				if((*(u8 *)pValue) > DEVICE_CAN_BUD_10K)
				{
					bOk = FALSE;
				}
				else
				{
					if((pParams->bud) ^ (*(u8 *)pValue))
					{
						pParams->bud = (*(u8 *)pValue);
						pParams->canPara.CAN_Prescaler = Device_BudGet(pParams->bud);
						chagFlag = TRUE;
						g_sFramBootParamenter.br = g_sDeviceParams.bud;
        				Fram_WriteBootParamenter();
					}
					bOk = TRUE;
				}
			}
			else if(subIndex == 0x03)
			{
				if(((*(u8 *)pValue) != CAN_FilterMode_IdList) && ((*(u8 *)pValue) != CAN_FilterMode_IdMask))
				{
					bOk = FALSE;
				}
				else
				{
					if((pParams->canFilterPara.CAN_FilterMode) ^ (*(u8 *)pValue))
					{
						pParams->canFilterPara.CAN_FilterMode = (*(u8 *)pValue);
						chagFlag = TRUE;
					}
					bOk = TRUE;
				}
			}
			else if(subIndex == 0x04)
			{
				if((pParams->canFilterPara.CAN_FilterIdLow) ^ (*(u16 *)pValue))
				{
					pParams->canFilterPara.CAN_FilterIdLow = (*(u16 *)pValue);
					chagFlag = TRUE;
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x05)
			{
				if((pParams->canFilterPara.CAN_FilterIdHigh) ^ (*(u16 *)pValue))
				{
					pParams->canFilterPara.CAN_FilterIdHigh = (*(u16 *)pValue);
					chagFlag = TRUE;
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x06)
			{
				if((pParams->canFilterPara.CAN_FilterMaskIdLow) ^ (*(u16 *)pValue))
				{
					pParams->canFilterPara.CAN_FilterMaskIdLow = (*(u16 *)pValue);
					chagFlag = TRUE;
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x07)
			{
				if((pParams->canFilterPara.CAN_FilterMaskIdHigh) ^ (*(u16 *)pValue))
				{
					pParams->canFilterPara.CAN_FilterMaskIdHigh = (*(u16 *)pValue);
					chagFlag = TRUE;
				}
				bOk = TRUE;
			}
			break;
		case 0x2001:
			if(subIndex == 0x01)
			{
				if(((*(u8 *)pValue) != DEVICE_OP_TAG_15693) && ((*(u8 *)pValue) != DEVICE_OP_TAG_14443A))
				{
					bOk = FALSE;
				}
				else
				{
					if((pParams->rfParams.protocol) ^ (*(u8 *)pValue))
					{
						pParams->rfParams.protocol = (*(u8 *)pValue);
						chagFlag = TRUE;
					}
					bOk = TRUE;
				}
			}
			else if(subIndex == 0x02)
			{
				if((pParams->rfParams.rBlkNum) ^ (*(u8 *)pValue))
				{
					pParams->rfParams.rBlkNum = (*(u8 *)pValue);
					chagFlag = TRUE;
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x03)
			{
				if((pParams->autoBlockParams.addr) ^ (*(u8 *)pValue))
				{
					pParams->autoBlockParams.addr = (*(u8 *)pValue);
					chagFlag = TRUE;
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x04)
			{
				if((pParams->autoBlockParams.len) ^ (*(u8 *)pValue))
				{
					pParams->autoBlockParams.len = (*(u8 *)pValue);
					chagFlag = TRUE;
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x05)
			{
				if((pParams->workTim.invtTim) ^ (*(u8 *)pValue))
				{
					pParams->workTim.invtTim = (*(u8 *)pValue);
					chagFlag = TRUE;
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x06)
			{
				if((pParams->workTim.idleTim) ^ (*(u8 *)pValue))
				{
					pParams->workTim.idleTim = (*(u8 *)pValue);
					chagFlag = TRUE;
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x07)
			{
				if((pParams->workTim.keepTim) ^ (*(u8 *)pValue))
				{
					pParams->workTim.keepTim = (*(u8 *)pValue);
					chagFlag = TRUE;
				}
				bOk = TRUE;
			}
			break;
		case 0x2002:
			if(subIndex == 0x02)
			{
				if((DeviceParams_OpDevice) ^ (*(u8 *)pValue))
				{
					DeviceParams_OpDevice = (*(u8 *)pValue);
					if(Device_ProcessCanFrame())
					{
						bOk = TRUE;
					}
				}
			}
			else if(subIndex == 0x03)
			{
				if((DeviceParams_OpAddr) ^ (*(u16 *)pValue))
				{
					DeviceParams_OpAddr = (*(u16 *)pValue);
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x04)
			{
				if((DeviceParams_OpLen) ^ (*(u16 *)pValue))
				{
					DeviceParams_OpLen = (*(u16 *)pValue);
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x05)
			{
				if((DeviceParams_OpFrame0) ^ (*(u32 *)pValue))
				{
					DeviceParams_OpFrame0 = (*(u32 *)pValue);
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x06)
			{
				if((DeviceParams_OpFrame1) ^ (*(u32 *)pValue))
				{
					DeviceParams_OpFrame1 = (*(u32 *)pValue);
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x07)
			{
				if((DeviceParams_OpFrame2) ^ (*(u32 *)pValue))
				{
					DeviceParams_OpFrame2 = (*(u32 *)pValue);
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x08)
			{
				if((DeviceParams_OpFrame3) ^ (*(u32 *)pValue))
				{
					DeviceParams_OpFrame3 = (*(u32 *)pValue);
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x09)
			{
				if((DeviceParams_OpFrame4) ^ (*(u32 *)pValue))
				{
					DeviceParams_OpFrame4 = (*(u32 *)pValue);
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x0A)
			{
				if((DeviceParams_OpFrame5) ^ (*(u32 *)pValue))
				{
					DeviceParams_OpFrame5 = (*(u32 *)pValue);
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x0B)
			{
				if((DeviceParams_OpFrame6) ^ (*(u32 *)pValue))
				{
					DeviceParams_OpFrame6 = (*(u32 *)pValue);
				}
				bOk = TRUE;
			}
			else if(subIndex == 0x0C)
			{
				if((DeviceParams_OpFrame7) ^ (*(u32 *)pValue))
				{
					DeviceParams_OpFrame7 = (*(u32 *)pValue);
				}
				bOk = TRUE;
			}
			break;
		case 0x2003:
		case 0x2005:
		case 0x2100:
			break;
		default:
			break;
	}
	
				
	if(chagFlag)				//参数更改，需保存
	{
		Device_WriteDeviceParamenter();
	}

	return bOk;
}

void Device_FormatFrame(u8 *pBuffer, TAG_OPINFO *pOpInfo)
{
	u8 pos = 0;
	
	pOpInfo->rspBuffer[pos ++] = pOpInfo->errCode;
	pOpInfo->rspBuffer[pos ++] = pOpInfo->rspLen;
	
	if(pOpInfo->rspLen)
	{
		memcpy(pOpInfo->rspBuffer + pos, pBuffer, pOpInfo->rspLen);
	}
}
