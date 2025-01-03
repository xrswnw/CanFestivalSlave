
/* File generated by gen_cfile.py. Should not be modified. */

#ifndef ANYID_FR321CO_SLAVE_H
#define ANYID_FR321CO_SLAVE_H

#include "data.h"

/* Prototypes of function provided by object dictionnary */
UNS32 AnyId_FR321CO_Slave_valueRangeTest (UNS8 typeValue, void * value);
const indextable * AnyId_FR321CO_Slave_scanIndexOD (UNS16 wIndex, UNS32 * errorCode, ODCallback_t **callbacks);

extern UNS8 AnyId_FR321CO_Slave_obj1008[10];
extern UNS8 AnyId_FR321CO_Slave_obj1009[10];
extern UNS8 AnyId_FR321CO_Slave_obj100A[10];
/* Master node data struct */
extern CO_Data AnyId_FR321CO_Slave_Data;
extern UNS8 HardParams_DeviceAddr;		/* Mapped at index 0x2000, subindex 0x01 */
extern UNS8 HardParams_Bud;		/* Mapped at index 0x2000, subindex 0x02 */
extern UNS8 HardParams_CanFilterMode;		/* Mapped at index 0x2000, subindex 0x03 */
extern UNS16 HardParams_CanFilterIdLow;		/* Mapped at index 0x2000, subindex 0x04 */
extern UNS16 HardParams_CanFilterIdHigh;		/* Mapped at index 0x2000, subindex 0x05 */
extern UNS16 HardParams_CanFilterMaskIdLow;		/* Mapped at index 0x2000, subindex 0x06 */
extern UNS16 HardParams_CanFilterMaskIdHigh;		/* Mapped at index 0x2000, subindex 0x07 */
extern UNS16 HardParams_RfidRfu;		/* Mapped at index 0x2000, subindex 0x08 */
extern UNS8 RfidParams_Protocol;		/* Mapped at index 0x2001, subindex 0x01 */
extern UNS8 RfidParams_RBlkNum;		/* Mapped at index 0x2001, subindex 0x02 */
extern UNS16 RfidParams_AutoAddr;		/* Mapped at index 0x2001, subindex 0x03 */
extern UNS16 RfidParams_AutoLen;		/* Mapped at index 0x2001, subindex 0x04 */
extern UNS8 RfidParams_InvtTim;		/* Mapped at index 0x2001, subindex 0x05 */
extern UNS8 RfidParams_IdleTim;		/* Mapped at index 0x2001, subindex 0x06 */
extern UNS8 RfidParams_KeepTim;		/* Mapped at index 0x2001, subindex 0x07 */
extern UNS16 RfidParams_RfidRfu;		/* Mapped at index 0x2001, subindex 0x08 */
extern UNS8 DeviceParams_TagOpState;		/* Mapped at index 0x2002, subindex 0x01 */
extern UNS8 DeviceParams_OpDevice;		/* Mapped at index 0x2002, subindex 0x02 */
extern UNS16 DeviceParams_OpAddr;		/* Mapped at index 0x2002, subindex 0x03 */
extern UNS16 DeviceParams_OpLen;		/* Mapped at index 0x2002, subindex 0x04 */
extern UNS32 DeviceParams_OpFrame0;		/* Mapped at index 0x2002, subindex 0x05 */
extern UNS32 DeviceParams_OpFrame1;		/* Mapped at index 0x2002, subindex 0x06 */
extern UNS32 DeviceParams_OpFrame2;		/* Mapped at index 0x2002, subindex 0x07 */
extern UNS32 DeviceParams_OpFrame3;		/* Mapped at index 0x2002, subindex 0x08 */
extern UNS32 DeviceParams_OpFrame4;		/* Mapped at index 0x2002, subindex 0x09 */
extern UNS32 DeviceParams_OpFrame5;		/* Mapped at index 0x2002, subindex 0x0A */
extern UNS32 DeviceParams_OpFrame6;		/* Mapped at index 0x2002, subindex 0x0B */
extern UNS32 DeviceParams_OpFrame7;		/* Mapped at index 0x2002, subindex 0x0C */
extern UNS32 DeviceParams_Rfu;		/* Mapped at index 0x2002, subindex 0x0D */
extern UNS32 Tag15693_UidLow;		/* Mapped at index 0x2003, subindex 0x01 */
extern UNS32 Tag15693_UidHigh;		/* Mapped at index 0x2003, subindex 0x02 */
extern UNS32 Tag15693_Block0;		/* Mapped at index 0x2003, subindex 0x03 */
extern UNS32 Tag15693_Block1;		/* Mapped at index 0x2003, subindex 0x04 */
extern UNS32 Tag15693_Block2;		/* Mapped at index 0x2003, subindex 0x05 */
extern UNS32 Tag15693_Block3;		/* Mapped at index 0x2003, subindex 0x06 */
extern UNS32 Tag15693_Block4;		/* Mapped at index 0x2003, subindex 0x07 */
extern UNS32 Tag15693_Block5;		/* Mapped at index 0x2003, subindex 0x08 */
extern UNS32 Tag15693_Block6;		/* Mapped at index 0x2003, subindex 0x09 */
extern UNS32 Tag15693_Block7;		/* Mapped at index 0x2003, subindex 0x0A */
extern UNS8 Tag15693_15693Block[10];		/* Mapped at index 0x2003, subindex 0x0B */
extern UNS8 Tag15693Block[64];		/* Mapped at index 0x2004, subindex 0x01 - 0x40 */
extern UNS8 ComData_OpState;		/* Mapped at index 0x2100, subindex 0x01 */
extern UNS8 ComData_RcvBuffer[10];		/* Mapped at index 0x2100, subindex 0x02 */
extern UNS8 ComData_RspBuffer[10];		/* Mapped at index 0x2100, subindex 0x03 */
extern UNS8 ComData_TestState;		/* Mapped at index 0x2100, subindex 0x04 */
extern UNS64 ComData_OkTick;		/* Mapped at index 0x2100, subindex 0x05 */
extern UNS64 ComData_AlllTick;		/* Mapped at index 0x2100, subindex 0x06 */




//pdoparams
extern UNS8 AnyId_FR321CO_Slave_highestSubIndex_obj1400; /* number of subindex - 1*/
extern UNS32 AnyId_FR321CO_Slave_obj1400_COB_ID_used_by_PDO;	/* 512 */
extern UNS8 AnyId_FR321CO_Slave_obj1400_Transmission_Type	/* 0 */
extern UNS16 AnyId_FR321CO_Slave_obj1400_Inhibit_Time;	/* 0 */
extern UNS8 AnyId_FR321CO_Slave_obj1400_Compatibility_Entry; /* 0 */
extern UNS16 AnyId_FR321CO_Slave_obj1400_Event_Timer;	/* 0 */
extern UNS8 AnyId_FR321CO_Slave_obj1400_SYNC_start_value;	/* 0 */
extern subindex AnyId_FR321CO_Slave_Index1400[];

extern UNS8 AnyId_FR321CO_Slave_highestSubIndex_obj1600; /* number of subindex - 1*/
extern UNS32 AnyId_FR321CO_Slave_obj1600[];
extern subindex AnyId_FR321CO_Slave_Index1600[];
//

extern UNS8 AnyId_FR321CO_Slave_highestSubIndex_obj1401; /* number of subindex - 1*/
extern UNS32 AnyId_FR321CO_Slave_obj1401_COB_ID_used_by_PDO;	/* 768 */
extern UNS8 AnyId_FR321CO_Slave_obj1401_Transmission_Type;	/* 0 */
extern UNS16 AnyId_FR321CO_Slave_obj1401_Inhibit_Time;	/* 0 */
extern UNS8 AnyId_FR321CO_Slave_obj1401_Compatibility_Entry; /* 0 */
extern UNS16 AnyId_FR321CO_Slave_obj1401_Event_Timer;	/* 0 */
extern UNS8 AnyId_FR321CO_Slave_obj1401_SYNC_start_value;	/* 0 */
extern subindex AnyId_FR321CO_Slave_Index1401[];

extern UNS8 AnyId_FR321CO_Slave_highestSubIndex_obj1601; /* number of subindex - 1*/
extern UNS32 AnyId_FR321CO_Slave_obj1601[];
extern subindex AnyId_FR321CO_Slave_Index1601[];
//

extern UNS8 AnyId_FR321CO_Slave_highestSubIndex_obj1402; /* number of subindex - 1*/
extern UNS32 AnyId_FR321CO_Slave_obj1402_COB_ID_used_by_PDO;	/* 1024 */
extern UNS8 AnyId_FR321CO_Slave_obj1402_Transmission_Type;	/* 0 */
extern UNS16 AnyId_FR321CO_Slave_obj1402_Inhibit_Time;	/* 0 */
extern UNS8 AnyId_FR321CO_Slave_obj1402_Compatibility_Entry; /* 0 */
extern UNS16 AnyId_FR321CO_Slave_obj1402_Event_Timer;	/* 0 */
extern UNS8 AnyId_FR321CO_Slave_obj1402_SYNC_start_value;	/* 0 */
extern subindex AnyId_FR321CO_Slave_Index1402[];

extern UNS8 AnyId_FR321CO_Slave_highestSubIndex_obj1602; /* number of subindex - 1*/
extern UNS32 AnyId_FR321CO_Slave_obj1602[];
extern subindex AnyId_FR321CO_Slave_Index1602[];
//

extern UNS8 AnyId_FR321CO_Slave_highestSubIndex_obj1403; /* number of subindex - 1*/
extern UNS32 AnyId_FR321CO_Slave_obj1403_COB_ID_used_by_PDO;	/* 1280 */
extern UNS8 AnyId_FR321CO_Slave_obj1403_Transmission_Type;	/* 0 */
extern UNS16 AnyId_FR321CO_Slave_obj1403_Inhibit_Time;	/* 0 */
extern UNS8 AnyId_FR321CO_Slave_obj1403_Compatibility_Entry; /* 0 */
extern UNS16 AnyId_FR321CO_Slave_obj1403_Event_Timer;	/* 0 */
extern UNS8 AnyId_FR321CO_Slave_obj1403_SYNC_start_value;	/* 0 */
extern subindex AnyId_FR321CO_Slave_Index1403[];


extern UNS8 AnyId_FR321CO_Slave_highestSubIndex_obj1603; /* number of subindex - 1*/
extern UNS32 AnyId_FR321CO_Slave_obj1603[];
extern subindex AnyId_FR321CO_Slave_Index1603[];


#endif // ANYID_FR321CO_SLAVE_H
