#include "AnyID_FR321CO_Device.h"

const u8 DEVICE_VERSION[DEVICE_VERSION_SIZE]@0x08005000 = "FR321CO 24010900 G231212";

DEVICE_PARAMS g_sDeviceParams = {0};
DEVICE_INFO g_sDeviceInfo = {0};
u32 g_nDeviceState = 0;
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

        //�������Ƿ���ȷ���������ȷ��ʹ��Ĭ�ϲ�������
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

        //�������Ƿ���ȷ���������ȷ��ʹ��Ĭ�ϲ�������                        .
        if(crc1 != crc2)
        {
            bBackup = FALSE;
        }
    }
    if(b == FALSE && bBackup == FALSE)
    {
        memset(&g_sDeviceParams, 0, sizeof(DEVICE_PARAMS));
		//������λָ�Ĭ�ϣ�����������������������������������������������������������������������
		
		//ϵͳ����

		g_sDeviceParams.addr = DEVICE_CANOPEN_DEFAULT_ADDR;
		g_sDeviceParams.protocol = DEVICE_RFID_WORK_PROTOCOL_15693;
		g_sDeviceParams.mode = DEVICE_RFID_WORK_READ_UID | DEVICE_RFID_WORK_READ_BLOCK;
		//rfid ����

		g_sDeviceParams.rfidParaments15693.readAddr = 0x00;
		g_sDeviceParams.rfidParaments15693.readNum = 4;
		g_sDeviceParams.rfidParaments15693.rfCtrl = DEVICE_RF_OPEN;

		
        //canӲ������
		//v ����ֵ��1000K��800k��500K��250k��125k��50k��20k��10k
		//v = PLCK1 / (psc * (tbs1 + 1 + tbs2 + 1 + 1)) = 36/(8 *(3 + 1 + 3 + 1+ 1)) 
		//v = 4000 / psc��Ĭ��1000K
		g_sDeviceParams.canPara.CAN_BS1 = CAN_BS1_4tq;
		g_sDeviceParams.canPara.CAN_BS2 = CAN_BS2_4tq;
		g_sDeviceParams.canPara.CAN_SJW = CAN_SJW_2tq;
		g_sDeviceParams.canPara.CAN_Prescaler = DEVICE_CAN_BUD_500K_PSCVALUE;
		g_sDeviceParams.canPara.CAN_Mode = CAN_Mode_Normal;
		g_sDeviceParams.canPara.CAN_ABOM = DISABLE;   								// ����Զ����߹���
		g_sDeviceParams.canPara.CAN_AWUM = DISABLE;   								// ˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
		g_sDeviceParams.canPara.CAN_NART = ENABLE;   								// ʹ�ñ����Զ����� 
		g_sDeviceParams.canPara.CAN_RFLM = DISABLE;   								// ���Ĳ�����,�µĸ��Ǿɵ� 
		g_sDeviceParams.canPara.CAN_TTCM = DISABLE;									// ��ʱ�䴥��ͨ��ģʽ  
		g_sDeviceParams.canPara.CAN_TXFP = DISABLE;   								// ���ȼ��ɱ��ı�ʶ������ 
		//can������
		g_sDeviceParams.canFilterPara.CAN_FilterNumber = 0;  					 	// ������0	
		g_sDeviceParams.canFilterPara.CAN_FilterIdHigh = 0x0000;   					// 32λID
		g_sDeviceParams.canFilterPara.CAN_FilterIdLow = 0x0000;
		g_sDeviceParams.canFilterPara.CAN_FilterMaskIdHigh = 0x0000;   				// 32λMASK
		g_sDeviceParams.canFilterPara.CAN_FilterMaskIdLow = 0x0000;
		g_sDeviceParams.canFilterPara.CAN_FilterMode = CAN_FilterMode_IdMask;   	// ����ģʽ
		g_sDeviceParams.canFilterPara.CAN_FilterScale = CAN_FilterScale_32bit;   	// 32λ 
		g_sDeviceParams.canFilterPara.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;   // ������0������FIFO0
		g_sDeviceParams.canFilterPara.CAN_FilterActivation = ENABLE;				// ���������0

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
    if(mode == DEVICE_RF_OPEN || mode == DEVICE_RF_CLOSE || mode == DEVICE_RF_RESET)
    {
        //�ر�RF��Ҳ��Ҫ�ر�����
        if(mode == DEVICE_RF_CLOSE || mode == DEVICE_RF_RESET)
        {
            St25r3916_CloseAntenna();
        }

        if(mode == DEVICE_RF_RESET)
        {
            Device_Delayms(10);//��ʱ10ms
        }

        if((mode == DEVICE_RF_OPEN && g_sDeviceParams.rfidParaments15693.rfCtrl == DEVICE_RF_CLOSE) || mode == DEVICE_RF_RESET)
        {
            St25r3916_OpenAntenna();
        }

        if(mode == DEVICE_RF_OPEN || mode == DEVICE_RF_RESET)
        {
            g_sDeviceParams.rfidParaments15693.rfCtrl = DEVICE_RF_OPEN;
        }
        else
        {
            g_sDeviceParams.rfidParaments15693.rfCtrl = DEVICE_RF_CLOSE;
        }
    }

}
/*
SDO ���⣬��ȡ200404�����ݣ��Ѿ����÷��ͣ������ݳ�ʱ�����Ͳ���ȥ���ݲ����ԭ��������������(200401,200403,200405,200406)

*/
void Device_RfidTask()
{
	DEVICE_RFIDPARS15693 *pRfidParaments;
	DEVICE_INFO *pDeviceInfo;

	pDeviceInfo = &g_sDeviceInfo;
	if(a_CheckStateBit(g_sDeviceParams.mode, DEVICE_RFID_WORK_READ_UID))				//�������ƣ����λ���������
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
					
					if(a_CheckStateBit(g_sDeviceParams.mode, DEVICE_RFID_WORK_READ_BLOCK) && a_CheckStateBit(pDeviceInfo->state, DEVICE_RFID_STATE_HAVE_TAG))	//���ݿ��ʹ��					/*���ݿ��ϱ����̣��˸����ݿ�ɷֱ�ӳ������ÿ������һ�����ݿ顢�ĸ��ֽڣ��ɼ���һ·PDO�ϱ��������ݿ�����*/
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
						{	//�������ݿ�ͬʱʹ�ܣ����жϿ�������
							if(memcmp(pRfidParaments->block, pDeviceInfo->readInfo15696.block, pRfidParaments->writeNum * pRfidParaments->writeAddr))
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
						{	//�Ƿ�����У��
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
