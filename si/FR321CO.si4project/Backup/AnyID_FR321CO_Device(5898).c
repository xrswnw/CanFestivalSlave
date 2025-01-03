#include "AnyID_FR321CO_Device.h"

const u8 DEVICE_VERSION[DEVICE_VERSION_SIZE]@0x08005000 = "FR321CO 24010900 G231212";


u32 g_nDeviceState = 0;
DEVICE_PARAMS g_sDeviceParams = {0};
DEVICE_INFO g_sDeviceInfo = {0};
DEVICE_OP g_sDeviceOpInfo = {0};
DEVICE_TAGMEM g_sDeviceTagMemory = {0};
DEVICE_BACKUPUID g_sDeviceBackupUid = {0};
DEVICE_OPTAGINFO g_sDeviceOpTagInfo = {0};

void Device_Init()
{






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
    if(1)//(b == FALSE && bBackup == FALSE)
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
	if((g_sFramBootParamenter.appState != FRAM_BOOT_APP_OK) )
    {
        g_sFramBootParamenter.appState = FRAM_BOOT_APP_OK;
        Fram_WriteBootParamenter();
    }

}


void Device_ResetDeviceParamenter()
{
    memset(&g_sDeviceParams, 0, sizeof(DEVICE_PARAMS));
		
	
	//系统参数
	g_sDeviceParams.addr = 2;
	g_sDeviceParams.bud = DEVICE_CAN_BUD_500K;
	
	g_sDeviceParams.rfParams.protocol = DEVICE_OP_TAG_15693;
    g_sDeviceParams.rfParams.rBlkNum = 1;                               //一次读取n+1个数据块
	
    g_sDeviceParams.autoBlockParams.addr = 0;                         
    g_sDeviceParams.autoBlockParams.len = 0;
    g_sDeviceParams.workTim.invtTim = DEVICE_INVENTORY_TO_DFT;
    g_sDeviceParams.workTim.idleTim = DEVICE_IDLE_TIM_DFT;
    g_sDeviceParams.workTim.keepTim = DEVICE_KEEPLIVE_TO_DFT;
	
	//can硬件参数
	//v 典型值：1000K、800k、500K、250k、125k、50k、20k、10k
	//v = PLCK1 / (psc * (tbs1 + 1 + tbs2 + 1 + 1)) = 36/(8 *(１ + 1 + ２ + 1+ 1)) 
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
	//can过滤器
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
}

u16 Device_BudGet(u8 bud)
{
	u16 value = DEVICE_CAN_BUD_1000K_PSCVALUE;
	
	if(bud == DEVICE_CAN_BUD_1000K)
	{
		value = DEVICE_CAN_BUD_1000K_PSCVALUE;
	}
	else if(bud == DEVICE_CAN_BUD_800K)
	{
		value = DEVICE_CAN_BUD_800K_PSCVALUE;
	}
	else if(bud == DEVICE_CAN_BUD_500K)
	{
		value = DEVICE_CAN_BUD_500K_PSCVALUE;
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
	
	return value;
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


void Device_RfCtrl(u8 mode)
{


}
/*
SDO 问题，拉取200404的数据，已经调用发送，但数据超时，发送不出去，暂不清楚原因，其它索引正常(200401,200403,200405,200406)

*/
/*
void Device_RfidTask()
{
	DEVICE_RFIDPARS15693 *pRfidParaments;
	DEVICE_INFO *pDeviceInfo;

	pDeviceInfo = &g_sDeviceInfo;
	if(a_CheckStateBit(g_sDeviceParams.mode, DEVICE_RFID_WORK_READ_UID))				//增加限制，最低位不允许更改
	{
		if(g_sDeviceParams.protocol == DEVICE_RFID_WORK_PROTOCOL_15693)
		{
			pRfidParaments = &g_sDeviceParams.rfidParaments15693;
			if(a_CheckStateBit(g_nDeviceState, DEVICE_STATE_AUTO_RFID))
			{
				a_ClearStateBit(g_nDeviceState, DEVICE_STATE_AUTO_RFID);
				St25r3916_OpenAntenna();Device_Delayms(1);
				if(!ISO15693_Inventory(pRfidParaments->afiCtrl, pRfidParaments->afiValue, pDeviceInfo->readInfo15696.sampleUid))
				{
					if(pDeviceInfo->hTick >= DEVICE_RFID_MASK_TICK)
					{
						a_SetStateBit(pDeviceInfo->state, DEVICE_RFID_STATE_HAVE_TAG);
						memcpy(pDeviceInfo->readInfo15696.uid, pDeviceInfo->readInfo15696.sampleUid, ISO15693_SIZE_UID);
					}
					else
					{
						pDeviceInfo->hTick ++;
					}
					pDeviceInfo->nTick = 0;
					
					if(a_CheckStateBit(g_sDeviceParams.mode, DEVICE_RFID_WORK_READ_BLOCK) && a_CheckStateBit(pDeviceInfo->state, DEVICE_RFID_STATE_HAVE_TAG))	//数据块读使能					
					{
						if(ISO15693_ReadBlock(pDeviceInfo->readInfo15696.uid, pRfidParaments->readNum, 
											  pRfidParaments->readAddr, pDeviceInfo->readInfo15696.block, ISO15693_SIZE_BLOCK) == ISO15693_ERR_SUC)
						{
							a_SetStateBit(pDeviceInfo->state, DEVICE_RFID_STATE_BLOCK_READ_OK);
						}
					}
					
					if(a_CheckStateBit(g_sDeviceParams.mode, DEVICE_RFID_WORK_WRITE_BLOCK) && a_CheckStateBit(pDeviceInfo->state, DEVICE_RFID_STATE_HAVE_TAG))
					{
						if(a_CheckStateBit(g_sDeviceParams.mode, DEVICE_RFID_WORK_READ_BLOCK) && a_CheckStateBit(pDeviceInfo->state, DEVICE_RFID_STATE_BLOCK_READ_OK))
						{	//若读数据块同时使能，可判断块内数据
							if(memcmp(pRfidParaments->block, pDeviceInfo->readInfo15696.block, (pRfidParaments->writeNum) * (pRfidParaments->writeAddr)))
							{
								if(ISO15693_WriteBlock(pDeviceInfo->readInfo15696.uid, pRfidParaments->writeNum,
													   pRfidParaments->writeAddr, pRfidParaments->block, ISO15693_SIZE_BLOCK) == ISO15693_ERR_SUC)
								{
									a_SetStateBit(pDeviceInfo->state, DEVICE_RFID_STATE_BLOCK_WRITE_OK);
								}
							}
							else
							{
								a_SetStateBit(pDeviceInfo->state, DEVICE_RFID_STATE_BLOCK_WRITE_OK);
							}
						}
						else
						{	//是否增加校验
							if(ISO15693_WriteBlock(pDeviceInfo->readInfo15696.uid, pRfidParaments->writeNum,
												   pRfidParaments->writeAddr, pRfidParaments->block, ISO15693_SIZE_BLOCK) == ISO15693_ERR_SUC)
							{
								a_SetStateBit(pDeviceInfo->state, DEVICE_RFID_STATE_BLOCK_WRITE_OK);
							}
						}
					}
				}
				else
				{
					if(pDeviceInfo->nTick >= DEVICE_RFID_MASK_TICK)
					{
						if(a_CheckStateBit(pDeviceInfo->state, DEVICE_RFID_STATE_HAVE_TAG))
						{
							pDeviceInfo->state = 0;
							memset(&pDeviceInfo->readInfo15696, 0, sizeof(DEVICE_TAGINFO15693));
						}
					}
					else
					{
						pDeviceInfo->nTick ++;
					}
					pDeviceInfo->hTick = 0;
				}
				St25r3916_CloseAntenna();
			}
		}
		else
		{
		
		
		}
	}
}
*/
void Device_ChkState()
{




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

    pOpInfo->opTagMode = DEVICE_OP_TAG_INVT;
    pOpInfo->operation[pOpInfo->num++] = DEVICE_OPERATION_INVT;

	temp = (short)g_sDeviceParams.autoBlockParams.addr;
	if(temp)
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
}


void Device_Iso15693SaveUid(u8 *pUid, DEVICE_TAGMEM *pTagMem)
{
    u16 uidReg[ISO15693_SIZE_UID >> 1] = {0};
	
    memcpy(uidReg, pTagMem->tag15693Info.uid, ISO15693_SIZE_UID);

    /*pTagMem->uid[0] = pUid[7]; pTagMem->uid[1] = pUid[6];pTagMem->uid[2] = pUid[5]; pTagMem->uid[3] = pUid[4];
    pTagMem->uid[4] = pUid[3]; pTagMem->uid[5] = pUid[2];pTagMem->uid[6] = pUid[1]; pTagMem->uid[7] = pUid[0];
	*/
	//---
	pTagMem->tag15693Info.uid[0] = pUid[7]; pTagMem->tag15693Info.uid[1] = pUid[6];pTagMem->tag15693Info.uid[2] = pUid[5]; pTagMem->tag15693Info.uid[3] = pUid[4];
    pTagMem->tag15693Info.uid[4] = pUid[3]; pTagMem->tag15693Info.uid[5] = pUid[2];pTagMem->tag15693Info.uid[6] = pUid[1]; pTagMem->tag15693Info.uid[7] = pUid[0];
	//--
    //不一样的UID，清空block存储区
    if(memcmp(uidReg, pTagMem->tag15693Info.uid, ISO15693_SIZE_UID) )
    {
        //memset(pTagMem->block, 0, DEVICE_BLK_MEM_SIZE);     //不样的标签
		/*if(uidReg[0] == 0 && uidReg[1] == 0)
		{
			pTagMem->succTick ++;
			a_SetStateBit(g_nDeviceState, DEVICE_STAT_ALARMLED);
		}*/
		memset(pTagMem->tag15693Info.ReadBlockData, 0, DEVICE_BLOCK_LEN_15693);     //不样的标签
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
    u8 state = ISO15693_ERR_SUC;
    u8 blockNum = 0;
    u8 i = 0;
    u8 opNum = 0, rAddr = 0;

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
                    //memcpy(pTagMem->block + pInfo->b.rAddr[pInfo->b.rIndex] * pInfo->b.blockSize, pInfo->b.block, pInfo->b.opNum * pInfo->b.blockSize);
					memcpy(pTagMem->tag15693Info.ReadBlockData, pInfo->b.block, pOpInfo->memLen);
                }
                rlt = ISO15693_ERR_SUC;
            }
            break;
        case DEVICE_OPERATION_WRITE:
            //state = Reader_Iso15693FmtWriteBlock(pInfo);
            //if(state == RC663_STAT_OK)
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
	static u32 keepTime = 0, oldState = 0;;
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
				keepTime = g_nSysTick;
				
				if(oldState != DEVICE_STAT_TAG_EXIXT)
				{
					g_sDeviceTagMemory.succTick ++;
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

				/*if(keepTime + g_sDeviceParams.workTim.keepTim <= g_nSysTick)
				{
					keepTime = g_nSysTick;
					if(Device_ChkSate(DEVICE_STAT_TAG_EXIXT))
					{
						Device_ClaSate(DEVICE_STAT_TAG_EXIXT);
						memset(&g_sDeviceTagMemory.tag15693Info, 0, sizeof(PROTOCOL_15693INFO));
					}
				}*/	
				//----------
                //盘点超时，即结束，否则继续盘点
                if(pOpInfo->delay + (g_sDeviceParams.workTim.invtTim << 2) <= g_nSysTick)
                {
                    b = FALSE;
					if(Device_ChkSate(DEVICE_STAT_TAG_EXIXT))
					{
						oldState = DEVICE_STAT_TAG_NO_EXIXT;
						Device_ClaSate(DEVICE_STAT_TAG_EXIXT);
						memset(&g_sDeviceTagMemory.tag15693Info, 0, sizeof(PROTOCOL_15693INFO));
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

void Device_Iso15693GetReadBlockAddr(ISO15693_BLOCK *pBlockInfo, u16 memAddr, u16 memLen)
{
    u16 endAddr = 0;
    u16 blockAddr = 0, blockNum = 0;
    
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
    u16 endAddr = 0;
    u16 blockAddr = 0, blockNum = 0;

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
                memcpy(pOpTagInfo->tagMem, g_sDeviceTagMemory.uid + pOpTagInfo->addr + ISO15693_SIZE_UID, pOpTagInfo->len);
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