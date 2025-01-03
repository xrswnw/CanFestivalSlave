#ifndef _ANYID_CANFESTIVAL_FRAM_
#define _ANYID_CANFESTIVAL_FRAM_

#include "AnyID_FRam.h"

#define FRAM_BOOT_APP_OK                    0x5555
#define FRAM_BOOT_APP_FAIL                  0xAAAA
#define FRAM_BOOT_APP_DATA_DOWD             0x1111

typedef struct framBootDevicePar{
    u16 appState;
    u16 br;
    u16 addr;
    u16 crc;
}FRAM_BOOTPARAMS;

extern FRAM_BOOTPARAMS g_sFramBootParamenter;

#define FRAME_INFO_ADDR                     0x0100
#define FRAME_INFO_BACKUP_ADDR              0x0500

#define FRAM_MEM_SIZE                       (32 << 10)    //32k
#define FRAME_BOOT_INFO_BACKUP_ADDR         (FRAM_MEM_SIZE - 512) 
#define FRAME_BOOT_INFO_ADDR                (FRAM_MEM_SIZE - sizeof(FRAM_BOOTPARAMS)) 

void Fram_ReadBootParamenter(void);
BOOL Fram_WriteBootParamenter(void);

#endif

