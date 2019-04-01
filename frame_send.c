/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "NCOM_Command.h"
#include "frame_send.h"
#include "frame_driver.h"
#include "frame_porting.h"

/* Private defines -----------------------------------------------------------*/
#define NUMBER_FRAME_MAX  11
#define MCOM_UART_SEQ_NO_MAX			0xFF		
#define MCOM_UART_DUPLICATE_NO_MAX		0xFF

/* Private typedef -----------------------------------------------------------*/
typedef void(*callHandle)(unsigned char *data, unsigned int lengData, 
    unsigned char src, unsigned char dest, unsigned char dupNo, 
    unsigned char opcode1, unsigned char opcode2);

typedef struct
{
	unsigned int length;
	callHandle func;

}S_NCOM_Handle_Send_Data;

/* Global variable -----------------------------------------------------------*/
/* Private function prototypes------------------------------------------------*/
static unsigned char MCOM_getSeqNo(void);
static void SendPackFrame8ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2);
static void SendPackFrame12ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2);
static void SendPackFrame16ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2);
static void SendPackFrame24ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2);
static void SendPackFrame46ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2);
static void SendPackFrame48ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2);
static void SendPackFrame62ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2);
static void SendPackFrame64ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2);
static void SendPackFrame512ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2);
static void SendPackFrame640ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2);
static void SendPackFrame1012ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2);

/* Private variables ---------------------------------------------------------*/
static unsigned char g_MCOM_Uart_SeqNo = 1;
static S_NCOM_Handle_Send_Data table_Proc_Handle[NUMBER_FRAME_MAX] =
{
	{8,    SendPackFrame8ByteData   },
	{12,   SendPackFrame12ByteData  },
	{16,   SendPackFrame16ByteData  },
	{24,   SendPackFrame24ByteData  },
	{46,   SendPackFrame46ByteData  },
	{48,   SendPackFrame48ByteData  },
	{62,   SendPackFrame62ByteData  },
	{64,   SendPackFrame64ByteData  },
	{512,  SendPackFrame512ByteData },
	{640,  SendPackFrame640ByteData },
	{1012, SendPackFrame1012ByteData},
};

/* Function implementation ---------------------------------------------------*/

static void ParkHeaderInfo(S_NCOM_CMD_HEAD *header, unsigned int lengthData, unsigned char src, unsigned char dest, unsigned char dupNo)
{
	int appDataLen = (int)lengthData;
	memset(header, 0x00, sizeof(S_NCOM_CMD_HEAD));

	/* DL Header */
	header->DL.marker_sfd = NCOM_CMD_SFD;
	header->DL.dataLen1 = (unsigned char)((appDataLen + 4) & 0x00ff);
	header->DL.dataLen2 = (unsigned char)(((appDataLen + 4) & 0xff00) >> 8);
	header->DL.cSum = (unsigned char)(NCOM_CMD_SFD ^ header->DL.dataLen1 ^ header->DL.dataLen2);

	/* NW Header */
	header->NW.dstAdr = dest;
	header->NW.srcAdr = src;

	/* TP Header */
	header->TP.dCnt = dupNo;
	header->TP.sCnt = MCOM_getSeqNo();
}

void Frame_SendData(unsigned char *data, unsigned int cmdFrameLen, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2)
{
	int i = 0;
	for (i = 0; i < NUMBER_FRAME_MAX; i++)
	{
		if (table_Proc_Handle[i].length == cmdFrameLen)
		{
			table_Proc_Handle[i].func(data, cmdFrameLen, src, dest, dupNo, opcode1, opcode2);
			break;
		}
	}
}

static void SendPackFrame8ByteData(unsigned char *data, unsigned int cmdFrameLen, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2)
{
	int appDataLen = (int)cmdFrameLen;
	unsigned char sumVal = 0;
	unsigned char *c = NULL;
	int i = 0;
	unsigned int sizeofFrame = 0;
	S_NCOM_CMD_FRAME_8B frame = { 0, };
	sizeofFrame = sizeof(S_NCOM_CMD_FRAME_8B);
	ParkHeaderInfo(&(frame.head), cmdFrameLen, src, dest, dupNo);
	frame.data[0] = src;
	frame.data[1] = dest;
	frame.data[2] =  opcode1;
	frame.data[3] = opcode2;
	memcpy(&frame.data[4], data, appDataLen - 4);

	/* check sum calc. */
	sumVal ^= frame.head.NW.dstAdr;
	sumVal ^= frame.head.NW.srcAdr;
	sumVal ^= frame.head.TP.dCnt;
	sumVal ^= frame.head.TP.sCnt;
	c = (unsigned char *)frame.data;
	for (i = 0; i < appDataLen; i++) {
		sumVal ^= c[i];
	}
	/* DL Trailer */
	frame.trail.DL.cSum = sumVal;
	frame.trail.DL.reserve1 = 0x00;
	frame.trail.DL.reserve2 = 0x00;
	frame.trail.DL.marker_eot = NCOM_CMD_EOT;

	SendData_LL((unsigned char*)&frame, sizeofFrame);
}

static void SendPackFrame12ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2)
{
	int appDataLen = (int)lengData;
	unsigned char sumVal = 0;
	unsigned char *c = NULL;
	int i = 0;
	unsigned int sizeofFrame = 0;
	S_NCOM_CMD_FRAME_12B frame = { 0, };
	sizeofFrame = sizeof(S_NCOM_CMD_FRAME_12B);
	ParkHeaderInfo(&(frame.head), lengData, src, dest, dupNo);
	frame.data[0] = src;
	frame.data[1] = dest;
	frame.data[2] =  opcode1;
	frame.data[3] = opcode2;
	memcpy(&frame.data[4], data, appDataLen - 4);
	/* check sum calc. */
	sumVal ^= frame.head.NW.dstAdr;
	sumVal ^= frame.head.NW.srcAdr;
	sumVal ^= frame.head.TP.dCnt;
	sumVal ^= frame.head.TP.sCnt;
	c = (unsigned char *)frame.data;
	for (i = 0; i < appDataLen; i++) {
		sumVal ^= c[i];
	}
	/* DL Trailer */
	frame.trail.DL.cSum = sumVal;
	frame.trail.DL.reserve1 = 0x00;
	frame.trail.DL.reserve2 = 0x00;
	frame.trail.DL.marker_eot = NCOM_CMD_EOT;

	SendData_LL((unsigned char*)&frame, sizeofFrame);
}
static void SendPackFrame16ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2)
{
	int appDataLen = (int)lengData;
	unsigned char sumVal = 0;
	unsigned char *c = NULL;
	int i = 0;
	unsigned int sizeofFrame = 0;
	S_NCOM_CMD_FRAME_16B frame = { 0, };
	sizeofFrame = sizeof(S_NCOM_CMD_FRAME_16B);
	ParkHeaderInfo(&(frame.head), lengData, src, dest, dupNo);
	frame.data[0] = src;
	frame.data[1] = dest;
	frame.data[2] =  opcode1;
	frame.data[3] = opcode2;
	memcpy(&frame.data[4], data, appDataLen - 4);

	/* check sum calc. */
	sumVal ^= frame.head.NW.dstAdr;
	sumVal ^= frame.head.NW.srcAdr;
	sumVal ^= frame.head.TP.dCnt;
	sumVal ^= frame.head.TP.sCnt;
	c = (unsigned char *)frame.data;
	for (i = 0; i < appDataLen; i++) {
		sumVal ^= c[i];
	}
	/* DL Trailer */
	frame.trail.DL.cSum = sumVal;
	frame.trail.DL.reserve1 = 0x00;
	frame.trail.DL.reserve2 = 0x00;
	frame.trail.DL.marker_eot = NCOM_CMD_EOT;

	SendData_LL((unsigned char*)&frame, sizeofFrame);
}
static void SendPackFrame24ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2)
{
	int appDataLen = (int)lengData;
	unsigned char sumVal = 0;
	unsigned char *c = NULL;
	int i = 0;
	unsigned int sizeofFrame = 0;
	S_NCOM_CMD_FRAME_24B frame = { 0, };
	sizeofFrame = sizeof(S_NCOM_CMD_FRAME_24B);
	ParkHeaderInfo(&(frame.head), lengData, src, dest, dupNo);
	frame.data[0] = src;
	frame.data[1] = dest;
	frame.data[2] =  opcode1;
	frame.data[3] = opcode2;
	memcpy(&frame.data[4], data, appDataLen - 4);

	/* check sum calc. */
	sumVal ^= frame.head.NW.dstAdr;
	sumVal ^= frame.head.NW.srcAdr;
	sumVal ^= frame.head.TP.dCnt;
	sumVal ^= frame.head.TP.sCnt;
	c = (unsigned char *)frame.data;
	for (i = 0; i < appDataLen; i++) {
		sumVal ^= c[i];
	}
	/* DL Trailer */
	frame.trail.DL.cSum = sumVal;
	frame.trail.DL.reserve1 = 0x00;
	frame.trail.DL.reserve2 = 0x00;
	frame.trail.DL.marker_eot = NCOM_CMD_EOT;

	SendData_LL((unsigned char*)&frame, sizeofFrame);
}
static void SendPackFrame46ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2)
{
	int appDataLen = (int)lengData;
	unsigned char sumVal = 0;
	unsigned char *c = NULL;
	int i = 0;
	unsigned int sizeofFrame = 0;
	S_NCOM_CMD_FRAME_46B frame = { 0, };
	sizeofFrame = sizeof(S_NCOM_CMD_FRAME_46B);
	ParkHeaderInfo(&(frame.head), lengData, src, dest, dupNo);
	frame.data[0] = src;
	frame.data[1] = dest;
	frame.data[2] =  opcode1;
	frame.data[3] = opcode2;
	memcpy(&frame.data[4], data, appDataLen - 4);

	/* check sum calc. */
	sumVal ^= frame.head.NW.dstAdr;
	sumVal ^= frame.head.NW.srcAdr;
	sumVal ^= frame.head.TP.dCnt;
	sumVal ^= frame.head.TP.sCnt;
	c = (unsigned char *)frame.data;
	for (i = 0; i < appDataLen; i++) {
		sumVal ^= c[i];
	}
	/* DL Trailer */
	frame.trail.DL.cSum = sumVal;
	frame.trail.DL.reserve1 = 0x00;
	frame.trail.DL.reserve2 = 0x00;
	frame.trail.DL.marker_eot = NCOM_CMD_EOT;

	SendData_LL((unsigned char*)&frame, sizeofFrame);
}
static void SendPackFrame48ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2)
{
	int appDataLen = (int)lengData;
	unsigned char sumVal = 0;
	unsigned char *c = NULL;
	int i = 0;
	unsigned int sizeofFrame = 0;
	S_NCOM_CMD_FRAME_48B frame = { 0, };
	sizeofFrame = sizeof(S_NCOM_CMD_FRAME_48B);
	ParkHeaderInfo(&(frame.head), lengData, src, dest, dupNo);
	frame.data[0] = src;
	frame.data[1] = dest;
	frame.data[2] =  opcode1;
	frame.data[3] = opcode2;
	memcpy(&frame.data[4], data, appDataLen - 4);

	/* check sum calc. */
	sumVal ^= frame.head.NW.dstAdr;
	sumVal ^= frame.head.NW.srcAdr;
	sumVal ^= frame.head.TP.dCnt;
	sumVal ^= frame.head.TP.sCnt;
	c = (unsigned char *)frame.data;
	for (i = 0; i < appDataLen; i++) {
		sumVal ^= c[i];
	}
	/* DL Trailer */
	frame.trail.DL.cSum = sumVal;
	frame.trail.DL.reserve1 = 0x00;
	frame.trail.DL.reserve2 = 0x00;
	frame.trail.DL.marker_eot = NCOM_CMD_EOT;

	SendData_LL((unsigned char*)&frame, sizeofFrame);
}
static void SendPackFrame62ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2)
{
	int appDataLen = (int)lengData;
	unsigned char sumVal = 0;
	unsigned char *c = NULL;
	int i = 0;
	unsigned int sizeofFrame = 0;
	S_NCOM_CMD_FRAME_62B frame = { 0, };
	sizeofFrame = sizeof(S_NCOM_CMD_FRAME_62B);
	ParkHeaderInfo(&(frame.head), lengData, src, dest, dupNo);
	frame.data[0] = src;
	frame.data[1] = dest;
	frame.data[2] =  opcode1;
	frame.data[3] = opcode2;
	memcpy(&frame.data[4], data, appDataLen - 4);

	/* check sum calc. */
	sumVal ^= frame.head.NW.dstAdr;
	sumVal ^= frame.head.NW.srcAdr;
	sumVal ^= frame.head.TP.dCnt;
	sumVal ^= frame.head.TP.sCnt;
	c = (unsigned char *)frame.data;
	for (i = 0; i < appDataLen; i++) {
		sumVal ^= c[i];
	}
	/* DL Trailer */
	frame.trail.DL.cSum = sumVal;
	frame.trail.DL.reserve1 = 0x00;
	frame.trail.DL.reserve2 = 0x00;
	frame.trail.DL.marker_eot = NCOM_CMD_EOT;

	SendData_LL((unsigned char*)&frame, sizeofFrame);
}
static void SendPackFrame64ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2)
{
	int appDataLen = (int)lengData;
	unsigned char sumVal = 0;
	unsigned char *c = NULL;
	int i = 0;
	unsigned int sizeofFrame = 0;
	S_NCOM_CMD_FRAME_64B frame = { 0, };
	sizeofFrame = sizeof(S_NCOM_CMD_FRAME_64B);
	ParkHeaderInfo(&(frame.head), lengData, src, dest, dupNo);
	frame.data[0] = src;
	frame.data[1] = dest;
	frame.data[2] =  opcode1;
	frame.data[3] = opcode2;
	memcpy(&frame.data[4], data, appDataLen - 4);

	/* check sum calc. */
	sumVal ^= frame.head.NW.dstAdr;
	sumVal ^= frame.head.NW.srcAdr;
	sumVal ^= frame.head.TP.dCnt;
	sumVal ^= frame.head.TP.sCnt;
	c = (unsigned char *)frame.data;
	for (i = 0; i < appDataLen; i++) {
		sumVal ^= c[i];
	}
	/* DL Trailer */
	frame.trail.DL.cSum = sumVal;
	frame.trail.DL.reserve1 = 0x00;
	frame.trail.DL.reserve2 = 0x00;
	frame.trail.DL.marker_eot = NCOM_CMD_EOT;

	SendData_LL((unsigned char*)&frame, sizeofFrame);
}
static void SendPackFrame512ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2)
{
	int appDataLen = (int)lengData;
	unsigned char sumVal = 0;
	unsigned char *c = NULL;
	int i = 0;
	unsigned int sizeofFrame = 0;
	S_NCOM_CMD_FRAME_512B frame = { 0, };
	sizeofFrame = sizeof(S_NCOM_CMD_FRAME_512B);
	ParkHeaderInfo(&(frame.head), lengData, src, dest, dupNo);
	frame.data[0] = src;
	frame.data[1] = dest;
	frame.data[2] =  opcode1;
	frame.data[3] = opcode2;
	memcpy(&frame.data[4], data, appDataLen - 4);

	/* check sum calc. */
	sumVal ^= frame.head.NW.dstAdr;
	sumVal ^= frame.head.NW.srcAdr;
	sumVal ^= frame.head.TP.dCnt;
	sumVal ^= frame.head.TP.sCnt;
	c = (unsigned char *)frame.data;
	for (i = 0; i < appDataLen; i++) {
		sumVal ^= c[i];
	}
	/* DL Trailer */
	frame.trail.DL.cSum = sumVal;
	frame.trail.DL.reserve1 = 0x00;
	frame.trail.DL.reserve2 = 0x00;
	frame.trail.DL.marker_eot = NCOM_CMD_EOT;

	SendData_LL((unsigned char*)&frame, sizeofFrame);
}
static void SendPackFrame640ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2)
{
	int appDataLen = (int)lengData;
	unsigned char sumVal = 0;
	unsigned char *c = NULL;
	int i = 0;
	unsigned int sizeofFrame = 0;
	S_NCOM_CMD_FRAME_640B frame = { 0, };
	sizeofFrame = sizeof(S_NCOM_CMD_FRAME_640B);
	ParkHeaderInfo(&(frame.head), lengData, src, dest, dupNo);
	frame.data[0] = src;
	frame.data[1] = dest;
	frame.data[2] =  opcode1;
	frame.data[3] = opcode2;
	memcpy(&frame.data[4], data, appDataLen - 4);

	/* check sum calc. */
	sumVal ^= frame.head.NW.dstAdr;
	sumVal ^= frame.head.NW.srcAdr;
	sumVal ^= frame.head.TP.dCnt;
	sumVal ^= frame.head.TP.sCnt;
	c = (unsigned char *)frame.data;
	for (i = 0; i < appDataLen; i++) {
		sumVal ^= c[i];
	}
	/* DL Trailer */
	frame.trail.DL.cSum = sumVal;
	frame.trail.DL.reserve1 = 0x00;
	frame.trail.DL.reserve2 = 0x00;
	frame.trail.DL.marker_eot = NCOM_CMD_EOT;

	SendData_LL((unsigned char*)&frame, sizeofFrame);
}
static void SendPackFrame1012ByteData(unsigned char *data, unsigned int lengData, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2)
{
	int appDataLen = (int)lengData;
	unsigned char sumVal = 0;
	unsigned char *c = NULL;
	int i = 0;
	unsigned int sizeofFrame = 0;
	S_NCOM_CMD_FRAME_1K frame = { 0, };
	sizeofFrame = sizeof(S_NCOM_CMD_FRAME_1K);
	ParkHeaderInfo(&(frame.head), lengData, src, dest, dupNo);
	frame.data[0] = src;
	frame.data[1] = dest;
	frame.data[2] =  opcode1;
	frame.data[3] = opcode2;
	memcpy(&frame.data[4], data, appDataLen - 4);

	/* check sum calc. */
	sumVal ^= frame.head.NW.dstAdr;
	sumVal ^= frame.head.NW.srcAdr;
	sumVal ^= frame.head.TP.dCnt;
	sumVal ^= frame.head.TP.sCnt;
	c = (unsigned char *)frame.data;
	for (i = 0; i < appDataLen; i++) {
		sumVal ^= c[i];
	}
	/* DL Trailer */
	frame.trail.DL.cSum = sumVal;
	frame.trail.DL.reserve1 = 0x00;
	frame.trail.DL.reserve2 = 0x00;
	frame.trail.DL.marker_eot = NCOM_CMD_EOT;

	SendData_LL((unsigned char*)&frame, sizeofFrame);
}

static unsigned char MCOM_getSeqNo(void)
{
	unsigned char seqNo = g_MCOM_Uart_SeqNo;
	if (g_MCOM_Uart_SeqNo < MCOM_UART_SEQ_NO_MAX) {
		g_MCOM_Uart_SeqNo++;
	}
	else {
		g_MCOM_Uart_SeqNo = 1;
	}
	return seqNo;
}

/*****************************END OF FILE**************************************/

