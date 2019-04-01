/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "frame_driver.h"
#include "frame_ring_driver.h"
#include "NCOM_Command.h"

/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static SCIF_RING g_scir1 = {0};
/* Global variable -----------------------------------------------------------*/
/* Private function prototypes------------------------------------------------*/
/* Function implementation ---------------------------------------------------*/

void FDRV_PullData(unsigned char *buf, int length)
{
	int i = 0;
    for(i = 0; i < length; i++)
	{	
		RDRV_Uart_drv_putChar(&g_scir1, buf[i]); 
    }
}

unsigned int FDRV_scif1_gets(unsigned char *pBuff, unsigned int buffLen)
{
    unsigned int retVal = 0U;
	unsigned char res;
	unsigned char rcvFlg = 0U;
	unsigned char loopFlg = 1U;
	unsigned char poi;

	while(loopFlg != 0U)
	{
		if (rcvFlg == 1)
		{
			;
		}
		if(RDRV_Uart_drv_findNext(&g_scir1, FRM_START) != 0U)
		{
			rcvFlg = 1U;
		}
		else
		{
			/* Frame error (1: Checksum character is invalid, 2: End character is invalid, 8: Argument error, 9: Ring buffer error) */
			res = RDRV_Uart_drv_chkFrame(&g_scir1, &retVal);
			//printf("res = %d\n", res);
			if((res == 1U) || (res == 2U) || (res == 8U) || (res == 9U))
			{
				RDRV_Uart_drv_incRp(&g_scir1);
				poi = RDRV_Uart_drv_findNext(&g_scir1, FRM_START);
				
				loopFlg = 0U;
				retVal = 0U;
			}
			/* Header error (4: illegal frame length, 5: thumb error) */
			else if((res == 4U) || (res == 5U))
			{
				RDRV_Uart_drv_incRp(&g_scir1);
				rcvFlg = 0U;
			}
			else if((res == 3U) || (res == 6U) || (res == 7U))
			{
				rcvFlg = 1U;
			}
			else
			{
				loopFlg = 0U;
			}
		}
	}
	
	if(retVal > buffLen)
	{
		retVal = 0U;
	}
	if(retVal != 0U)
	{
		if(RDRV_Uart_drv_getStr(&g_scir1, pBuff, retVal) != 0U)
		{
			retVal = 0;
		}
	}
	
	return retVal;
}

/*****************************END OF FILE**************************************/

