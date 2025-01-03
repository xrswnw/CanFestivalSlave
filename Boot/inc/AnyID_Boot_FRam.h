#ifndef _ANYID_BOOT_FRAM_
#define _ANYID_BOOT_FRAM_

#include "AnyID_FRam.h"

#define FRAM_BOOT_APP_OK                    0x5555
#define FRAM_BOOT_APP_FAIL                  0xAAAA
#define FRAM_BOOT_APP_NONE                  0xFFFF      //crc校验错误，或者IIC读取失败

#define FRAM_BR_9600                        5
#define FRAM_BR_38400                       7
#define FRAM_BR_115200                      11

#define FRAM_CAN_BUD_1000K			0x00
#define FRAM_CAN_BUD_800K			0x01
#define FRAM_CAN_BUD_500K			0x02
#define FRAM_CAN_BUD_250K			0x03
#define FRAM_CAN_BUD_125K			0x04
#define FRAM_CAN_BUD_50K			0x05
#define FRAM_CAN_BUD_20K			0x06
#define FRAM_CAN_BUD_10K			0x07
typedef struct framBootFRAMPar{
    u16 appState;
    u16 br;
    u16 addr;
    u16 crc;
}FRAM_BOOTPARAMS;
extern FRAM_BOOTPARAMS g_sFramBootParamenter;

#define FRAME_INFO_ADDR                     0x0100
#define FRAME_INFO_BACKUP_ADDR              0x0300

#define FRAM_MEM_SIZE                       (32 << 10)    //32k
#define FRAME_BOOT_INFO_BACKUP_ADDR         (FRAM_MEM_SIZE - 512) 
#define FRAME_BOOT_INFO_ADDR                (FRAM_MEM_SIZE - sizeof(FRAM_BOOTPARAMS)) 


#define FRAM_KEY_DFT_ADDR                   0x08008000
#define FRAM_KEY1_POS_START                 12
#define FRAM_KEY2_POS_START                 32
#define FRAM_KEY3_POS_START                 52
typedef struct frameKeyParams{
    u8 keyBuffer[60];
    u32 crc;
}FRAM_KEY;
extern FRAM_KEY g_sFramKeyInfo;
extern u32 g_nFRamKey1;
extern u32 g_nFRamKey2;
extern u32 g_nFRamKey3;

#define FRam_CheckKey1()                    (*((u32 *)(g_sFramKeyInfo.keyBuffer + FRAM_KEY1_POS_START)) == g_nFRamKey1)
#define FRam_CheckKey2()                    (*((u32 *)(g_sFramKeyInfo.keyBuffer + FRAM_KEY2_POS_START)) == g_nFRamKey2)
#define FRam_CheckKey3()                    (*((u32 *)(g_sFramKeyInfo.keyBuffer + FRAM_KEY3_POS_START)) == g_nFRamKey3)

#define FRAME_KEY_ADDR                      0x1000      //key
#define FRAME_KEY_BACKUP_ADDR               0x1080      //key

#define FRAME_SN_SIZE                       200


void Fram_ReadBootParamenter(void);
BOOL Fram_WriteBootParamenter(void);
BOOL Fram_WriteBackupBootParamenter(void);
u32 Fram_GetUartBaudrate(void);
void FRam_ReadKey(void);
BOOL FRam_WriteKey(void);
u32	FRam_KeeloqEncrypt(u32 data, u64 key);
u32	FRam_KeeloqDecrypt(u32 data, u64 key);

void FRam_EncryptKey(u8 *pCpuId);
#endif

