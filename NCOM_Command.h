/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _NCOM_COMMAND_H_
#define _NCOM_COMMAND_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
#pragma pack(1)
typedef struct __S_NCOM_CMD_HEAD_F__{
	unsigned char srcAdr;
	unsigned char dstAdr;
	unsigned char code1;
	unsigned char code2;
} S_NCOM_CMD_HEAD_F;

typedef struct __S_NCOM_CMD_MFD_BASE_DISP_INFO__{
	unsigned char BaseScreen;					
	unsigned char reserve[3];	
}S_NCOM_CMD_MFD_BASE_DISP_INFO;

typedef struct __S_NCOM_CMD_FRAME29__{
	S_NCOM_CMD_HEAD_F head1;
	S_NCOM_CMD_MFD_BASE_DISP_INFO head2;
} S_NCOM_CMD_FRAME29;


typedef struct __S_NCOM_CMD_FRAME2B__{
	S_NCOM_CMD_HEAD_F head1;
	unsigned char use_name[40];				
	unsigned char pass[10];		
	unsigned char reverse[8];
} S_NCOM_CMD_FRAME2B;

typedef struct __DL_HEAD__ {
	unsigned char marker_sfd;
	unsigned char dataLen1;
	unsigned char dataLen2;
	unsigned char cSum;
} S_NCOM_DL_HEAD;

typedef struct __DL_TRAIL__ {
	unsigned char cSum;
	unsigned char reserve1;
	unsigned char reserve2;
	unsigned char marker_eot;
} S_NCOM_DL_TRAIL;

typedef struct __NW_HEAD__ {
	unsigned char dstAdr;
	unsigned char srcAdr;
} S_NCOM_NW_HEAD;

typedef struct __TP_HEAD__ {
	unsigned char dCnt;
	unsigned char sCnt;
//	unsigned char reserve;
} S_NCOM_TP_HEAD;

typedef struct __COM_HEAD__ {
	S_NCOM_DL_HEAD DL;
	S_NCOM_NW_HEAD NW;
	S_NCOM_TP_HEAD TP;
} S_NCOM_CMD_HEAD;

typedef struct __COM_TRAIL__ {
	S_NCOM_DL_TRAIL DL;
} S_NCOM_CMD_TRAIL;

typedef struct __COM_FRAME_1K__ {
	S_NCOM_CMD_HEAD head;
	unsigned char data[1012];
	S_NCOM_CMD_TRAIL trail;
} S_NCOM_CMD_FRAME_1K;

typedef struct __COM_FRAME_640__ {
	S_NCOM_CMD_HEAD head;
	unsigned char data[640];
	S_NCOM_CMD_TRAIL trail;
} S_NCOM_CMD_FRAME_640B;

typedef struct __COM_FRAME_512__ {
	S_NCOM_CMD_HEAD head;
	unsigned char data[512];
	S_NCOM_CMD_TRAIL trail;
} S_NCOM_CMD_FRAME_512B;

typedef struct __COM_FRAME_64__ {
	S_NCOM_CMD_HEAD head;
	unsigned char data[64];
	S_NCOM_CMD_TRAIL trail;
} S_NCOM_CMD_FRAME_64B;

typedef struct __COM_FRAME_62__ {
	S_NCOM_CMD_HEAD head;
	unsigned char data[62];
	S_NCOM_CMD_TRAIL trail;
} S_NCOM_CMD_FRAME_62B;

typedef struct __COM_FRAME_48__ {
	S_NCOM_CMD_HEAD head;
	unsigned char data[48];
	S_NCOM_CMD_TRAIL trail;
} S_NCOM_CMD_FRAME_48B;

typedef struct __COM_FRAME_46__ {
	S_NCOM_CMD_HEAD head;
	unsigned char data[46];
	S_NCOM_CMD_TRAIL trail;
} S_NCOM_CMD_FRAME_46B;

typedef struct __COM_FRAME_24__ {
	S_NCOM_CMD_HEAD head;
	unsigned char data[24];
	S_NCOM_CMD_TRAIL trail;
} S_NCOM_CMD_FRAME_24B;

typedef struct __COM_FRAME_16__ {
	S_NCOM_CMD_HEAD head;
	unsigned char data[16];
	S_NCOM_CMD_TRAIL trail;
} S_NCOM_CMD_FRAME_16B;

typedef struct __COM_FRAME_12__ {
	S_NCOM_CMD_HEAD head;
	unsigned char data[12];
	S_NCOM_CMD_TRAIL trail;
} S_NCOM_CMD_FRAME_12B;

typedef struct __COM_FRAME_8__ {
	S_NCOM_CMD_HEAD head;
	unsigned char data[8];
	S_NCOM_CMD_TRAIL trail;
} S_NCOM_CMD_FRAME_8B;
/* Exported constants --------------------------------------------------------*/
#define NCOM_CMD_SFD				0xAB
#define NCOM_CMD_EOT				0x04

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* _NCOM_COMMAND_H_ */

/******************************END OF FILE*************************************/

