#include "AnyID_CanFestival_Device.h"

const u8 DEVICE_VERSION[DEVICE_VERSION_SIZE]@0x08005000 = "CanFestival 24081302 G231200";

u32 g_nDeviceState = 0;
DEVICE_PARAMS g_sDeviceParams = {0};

READER_RSPFRAME g_sReaderRspFrame = {0};

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
    memcpy(slaveNormal_obj1008, DEVICE_VERSION, 15);
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

	//canopen参数
	g_sDeviceParams.canOpneParams.periphParams.deviceType = 0x74747410;				//设备型号
	g_sDeviceParams.canOpneParams.periphParams.errReg = 0x00;
	g_sDeviceParams.canOpneParams.periphParams.errFiled = 0x00;
	g_sDeviceParams.canOpneParams.periphParams.syncCobId = 0x00;
	g_sDeviceParams.canOpneParams.periphParams.cycleCom = 0;
	g_sDeviceParams.canOpneParams.periphParams.syncWindowsLenth = 0x00;

	g_sDeviceParams.canOpneParams.periphParams.guardTime = 0x00;
	g_sDeviceParams.canOpneParams.periphParams.timeCobId = 0x00;
	g_sDeviceParams.canOpneParams.periphParams.timesTMap = 0x00;
	g_sDeviceParams.canOpneParams.periphParams.emncCobId = 0x80 + 0x00;
	g_sDeviceParams.canOpneParams.periphParams.producerTime = 30000;		//生产者心跳时间，默认30s上报一次当前状态
/*
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

		*/
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
{
  /*
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
*/
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


u16 Device_ResponseFrame(u8 *pParam, u16 len, READER_RSPFRAME *pRspFrame)
{
    u16 pos = 0;
    u16 crc = 0;

    pRspFrame->buffer[pos++] = UART_FRAME_FLAG_HEAD1; // frame head first byte
    pRspFrame->buffer[pos++] = UART_FRAME_FLAG_HEAD2; // frame haed second byte
    pRspFrame->buffer[pos++] = 0x00;   // length
    pRspFrame->buffer[pos++] = (g_sDeviceParams.addr >> 0) & 0xFF;
    pRspFrame->buffer[pos++] = (g_sDeviceParams.addr >> 8) & 0xFF;
    pRspFrame->buffer[pos++] = (pRspFrame->destAddr >> 0) & 0xFF;
    pRspFrame->buffer[pos++] = (pRspFrame->destAddr >> 8) & 0xFF;
    pRspFrame->buffer[pos++] = UART_FRAME_RESPONSE_FLAG;
    pRspFrame->buffer[pos++] = pRspFrame->cmd;               // cmd
    pRspFrame->buffer[pos++] = UART_FRAME_PARAM_RFU;     // RFU

    if(len > UART_FRAME_DATA_MAX_LEN)
    {
        pRspFrame->buffer[pos++] = (len >> 0) & 0xFF;
        pRspFrame->buffer[pos++] = (len >> 8) & 0xFF;

        memcpy(pRspFrame->buffer + pos, pParam, len);
        pos += len;
    }
    else
    {
        memcpy(pRspFrame->buffer + pos, pParam, len);
        pos += len;
        
        pRspFrame->buffer[pos++] = pRspFrame->flag;
        pRspFrame->buffer[pos++] = pRspFrame->err;
        
        pRspFrame->buffer[UART_FRAME_POS_LEN] = pos - 3 + 2; //减去帧头7E 55 LEN 的三个字节，加上CRC的两个字节
    }

    crc = a_GetCrc(pRspFrame->buffer + UART_FRAME_POS_LEN, pos - UART_FRAME_POS_LEN); //从LEN开始计算crc
    pRspFrame->buffer[pos++] = crc & 0xFF;
    pRspFrame->buffer[pos++] = (crc >> 8) & 0xFF;

    pRspFrame->len = pos;

    return pos;
}



u16 Reader_ProcessUartFrame(u8 *pFrame, u16 len)
{
    u8 cmd = 0;
    u16 destAddr = 0;
    u16 paramsLen = 0;
    u8 *pParams = NULL;

    destAddr = *(pFrame + UART_FRAME_POS_DESTADDR);
    if((destAddr != DEVICE_FRAME_BROADCAST_ADDR) && (destAddr != 0x00FF) &&(destAddr != g_sDeviceParams.addr) )
    {
        return 0;
    }

    g_sReaderRspFrame.destAddr = *((u16 *)(pFrame + UART_FRAME_POS_SRCADDR));
    g_sReaderRspFrame.len = 0;
    cmd = *(pFrame + UART_FRAME_POS_CMD);

    g_sReaderRspFrame.cmd = cmd;
    g_sReaderRspFrame.flag = UART_FRAME_FLAG_OK;
    g_sReaderRspFrame.err = UART_FRAME_RSP_NOERR;

    if(pFrame[UART_FRAME_POS_LEN] == 0)
    {
        paramsLen = *((u16 *)(pFrame + UART_FRAME_POS_PAR));
        pParams = pFrame + UART_FRAME_POS_PAR + 2;
    }
    else
    {
        paramsLen = len - UART_FRAME_MIN_LEN;
        pParams = pFrame + UART_FRAME_POS_PAR;
    }
    
    switch(cmd)
    {
        default:
            break;
    }
    
    if(g_sReaderRspFrame.len == 0)
    {
        g_sReaderRspFrame.flag = UART_FRAME_FLAG_FAIL;
        g_sReaderRspFrame.err = UART_FRAME_RSP_PARAMSERR;
        g_sReaderRspFrame.len = Device_ResponseFrame(NULL, 0, &g_sReaderRspFrame);
    }
    
    return g_sReaderRspFrame.len;
}


/*
	1.直接映射参数，在修改参数前加入校验，如校验不过，不允修改, 返回错误。节省资源，快速。

	2.间接映射参数，在回调函数中进行数据操作。数据修改完再校验，不过则恢复，结构相对整洁，但资源消耗过大，且需高频更新间接参数值。
*/
UNS32 index0x1017_0x00_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{

	return OD_SUCCESSFUL;
}


UNS32 index0x2003_0x00_callback(CO_Data* d, const indextable *table, UNS8 bSubindex)											//回调函数
{
    static u32 oParams = 0;
    OUTOUT_PERIPH *pOutInfo = NULL;
    
    
    if(oParams ^ g_nRufu32)
    {
        oParams = g_nRufu32;
        pOutInfo = &g_sPeriphInfo.oPeriph;
        
        
        pOutInfo->outFlag = oParams >> 24;
        pOutInfo->openTime = oParams >> 16;
        pOutInfo->closeTime = oParams >> 8;
        pOutInfo->opNum = oParams >> 0;
        
        pOutInfo->state = PERIPH_OUTPUT_PERIPH_STATE_START;
    }
  
	return OD_SUCCESSFUL;
}

u32 Device_CanFestivalCallBackInit(void)				//回调函数注册
{
	UNS32 errorCode = OD_VALUE_RANGE_EXCEEDED;

    Device_ChkCallBack(RegisterSetODentryCallBack(&slaveNormal_Data, 0x2003, 0x00, index0x2003_0x00_callback));
	return errorCode;
}


BOOL Device_ProcessCanFrame()
{
	BOOL bOk = TRUE;
	
	return bOk;
}



//----------------

void Device_LedTask()
{
    OUTOUT_PERIPH *pOutInfo = NULL;
    
    pOutInfo = &g_sPeriphInfo.oPeriph;
    
    if(pOutInfo->state == PERIPH_OUTPUT_PERIPH_STATE_START)
    {
        if(pOutInfo->opNum)
        {
            Periph_Ctr(pOutInfo->outFlag, PERIPH_OUPUT_OPEN);
            pOutInfo->time = g_nSysTick;
            pOutInfo->state = PERIPH_OUTPUT_PERIPH_STATE_OPEN;
        }
        else
        {
            pOutInfo->status = PERIPH_OUTPUT_PERIPH_STATUS_IDLE;
            pOutInfo->state = PERIPH_OUTPUT_PERIPH_STATE_IDLE;
        }
    }
    else if(pOutInfo->state == PERIPH_OUTPUT_PERIPH_STATE_OPEN)
    {
        if(pOutInfo->time + pOutInfo->openTime * 2 <= g_nSysTick)
        {
            pOutInfo->state = PERIPH_OUTPUT_PERIPH_STATE_STOP;
        }
    }
    else if(pOutInfo->state == PERIPH_OUTPUT_PERIPH_STATE_STOP)
    {
        pOutInfo->time = g_nSysTick;
        Periph_Ctr(pOutInfo->outFlag, PERIPH_OUPUT_CLOSE);
        pOutInfo->state = PERIPH_OUTPUT_PERIPH_STATE_CLOSE;
    }
    else if(pOutInfo->state == PERIPH_OUTPUT_PERIPH_STATE_CLOSE)
    {
        if(pOutInfo->time + pOutInfo->closeTime * 2 <= g_nSysTick)
        {
            pOutInfo->state = PERIPH_OUTPUT_PERIPH_STATE_START;
            
            if(pOutInfo->opNum)
            {
                pOutInfo->opNum--;
            }
        }
    }
}