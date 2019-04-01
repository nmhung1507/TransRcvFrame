/* Includes ------------------------------------------------------------------*/
#include "frame_driver.h"
#include "frame_receive.h"
#include <stdio.h> 
#include <string.h>

/* Private defines -----------------------------------------------------------*/
#define MCOM_CMD_TOTAL_NUM 2

/* Private typedef -----------------------------------------------------------*/
typedef void (*CALLBACK_FCN)( unsigned char*);

typedef struct {
    unsigned char cmd_opc1; /* OPC1 */
    unsigned char cmd_opc2; /* OPC1 */
    CALLBACK_FCN callbackFnc;
} S_CMD_TBL;

/* Private function prototypes------------------------------------------------*/
static void CallbackFcn1(unsigned char* data);
static void CallbackFcn2(unsigned char* data);
static unsigned char ConvUartEvent();

/* Private variables ---------------------------------------------------------*/
static unsigned char data[1024] = {0};

static S_CMD_TBL cmd_table[MCOM_CMD_TOTAL_NUM] =
{
	{ 0x01, 0x02, &CallbackFcn1},
	{ 0x03, 0x04, &CallbackFcn2},											
};
/* Global variable -----------------------------------------------------------*/
/* Function implementation ---------------------------------------------------*/
static void CallbackFcn1(unsigned char* data)
{
	printf("Go to CallbackFcn1\r\n");
}

static void CallbackFcn2(unsigned char* data)
{
	printf("Go to CallbackFcn2\r\n");
}

static unsigned char ConvUartEvent()
{
   unsigned char from; /* from: sender */
   unsigned char to;   /* to: receiver */
   unsigned char opc1; /* command */
   unsigned char opc2; /* command */
   unsigned char evnt; /* event */
   int i = 0;
   from = data[8];
   to   = data[9]; 
   opc1 = data[10];
   opc2 = data[11];
   printf("==PT:ConvUartEvent():from = %x,to = %x,opc1 = %x,opc2 = %x\n",(unsigned char)from,(unsigned char)to,opc1,opc2);
    for (i = 0U; i < MCOM_CMD_TOTAL_NUM; i++) {
        if ((opc1 == cmd_table[i].cmd_opc1) && (opc2 == cmd_table[i].cmd_opc2)) {
			printf("evnt=%d\n", i);
			cmd_table[i].callbackFnc(&data[12]);
            break;
        }
    }
	return evnt;
}

void Frame_RecvData()
{
	unsigned int retVal;
	unsigned short errBit;
	unsigned char evnt;
	memset(data, 0x00, sizeof(data));
	retVal = FDRV_scif1_gets(data, 1024); 
	if(retVal != 0)
	{
		ConvUartEvent();
	}
}
/*****************************END OF FILE**************************************/

