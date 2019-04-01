/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdint.h>
#include "frame_ring_driver.h"

/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variable -----------------------------------------------------------*/
/* Private function prototypes------------------------------------------------*/
/* Function implementation ---------------------------------------------------*/
void RDRV_Uart_drv_initRing(SCIF_RING *rb)
{
	void *res;
	
	res = memset(rb, 0x00, sizeof(*rb));
	
	return;
}

unsigned int RDRV_Uart_drv_getLen(const SCIF_RING *rb)
{
	unsigned int len = 0;
	
	if(rb != NULL)
	{
		if(rb->wp < rb->rp)
		{
			len = SCIF_RING_LEN;
		}
		
		len += rb->wp;
		len -= rb->rp;
	}
	
	return len;
}


unsigned int RDRV_Uart_drv_getLast(const SCIF_RING *rb)
{
	return (unsigned int)SCIF_RING_LEN -1 -RDRV_Uart_drv_getLen(rb);
}


void RDRV_Uart_drv_incWpOverRun(SCIF_RING *rb)
{
	static unsigned int cntOverRun = 0;
	
	if(rb != NULL){
		rb->wp++;
		if(rb->wp == SCIF_RING_LEN){
			rb->wp = 0;
		}
		
		/* Overrun */
		if(rb->rp == rb->wp){
			rb->rp++;
			if(rb->rp == SCIF_RING_LEN){
				rb->rp = 0;
			}
			cntOverRun++;
		}
	}
	
	return;
}


void RDRV_Uart_drv_incWp(SCIF_RING *rb)
{
	static unsigned int cntPoi = 0;
	
	if(rb != NULL)
	{
		if(RDRV_Uart_drv_getLast(rb) != 0U)
		{
			rb->wp++;
			if(rb->wp == SCIF_RING_LEN)
			{
				rb->wp = 0;
			}
		}
		else
		{
			cntPoi++;
		}
	}
	
	return;
}


void RDRV_Uart_drv_incRp(SCIF_RING *rb)
{
	if(rb != NULL)
	{
		/* check enpty */
		if(rb->rp != rb->wp)
		{
			rb->rp++;
			if(rb->rp == SCIF_RING_LEN)
			{
				rb->rp = 0;
			}
		}
	}
	
	return;
}

void RDRV_Uart_drv_putChar(SCIF_RING *rb, unsigned char c)
{
	if(rb != NULL)
	{
		rb->buf[rb->wp] = c;
		RDRV_Uart_drv_incWp(rb);
	}
	
	return;
}

unsigned char RDRV_Uart_drv_getChar(SCIF_RING *rb, unsigned char *c)
{
	unsigned char res = 1;
	
	if((rb != NULL) && (c != NULL))
	{
		if(RDRV_Uart_drv_getLen(rb) > 0){
			*c = rb->buf[rb->rp];
			RDRV_Uart_drv_incRp(rb);
			res = 0;
		}
	}
	return res;
}

unsigned char RDRV_Uart_drv_getCharOfst(const SCIF_RING *rb, unsigned char *c, unsigned int ofst)
{
	unsigned int tmp;
	unsigned char res = 1;
	
	if((rb != NULL) && (c != NULL))
	{
		tmp = RDRV_Uart_drv_getLen(rb);
		if((tmp != 0) && (tmp > ofst))
		{
			tmp = rb->rp + ofst;
			while(tmp >= SCIF_RING_LEN){
				tmp -= SCIF_RING_LEN;
			}
			*c = rb->buf[tmp];
			res = 0;	/* success */
		}
	}
	
	return res;
}

unsigned char RDRV_Uart_drv_getStr(SCIF_RING *rb, unsigned char *out, unsigned int cnt)
{
	unsigned int i;
	unsigned char res = 0;
	
	if((rb != NULL) && (out != NULL))
	{
		for(i=0; i<cnt; i++)
		{
			res |= RDRV_Uart_drv_getChar(rb, &out[i]);
		}
	}
	
	return res;
}

unsigned char RDRV_Uart_drv_findNext(SCIF_RING *rb, unsigned char head)
{
	unsigned int len = RDRV_Uart_drv_getLen(rb);
	unsigned int i;
	unsigned char c = 0;
	unsigned char flag = 1; 
	for(i=0; i<len; i++){
		if(!RDRV_Uart_drv_getCharOfst(rb, &c, 0)){
			if(c == head){
				flag = 0;	
				break;
			}
			RDRV_Uart_drv_incRp(rb); 
		}
	}
	
	return flag;
}

unsigned char RDRV_Uart_drv_chkFrame(const SCIF_RING *rb, unsigned int *len)
{
	unsigned int p = 0;
	unsigned int rcvLen = 0;
	unsigned int frmLen = 0;
	unsigned char sum = 0xff;
	unsigned char c = 0;
	unsigned char res = 0;
	
	if((rb != NULL) && (len != NULL))
	{
		rcvLen = RDRV_Uart_drv_getLen(rb);
		if(rcvLen < 8)
		{
			res = 7; 
		}
		else
		{
			if(RDRV_Uart_drv_getCharOfst(rb, &c, p) != 0U)
			{
				res = 9; 
			}
			p++;
		}
		
		if(res != 0)
		{
			; 
		}
		else if(c != FRM_START)
		{
			res = 6; 
		}
		else
		{
			sum = FRM_START;
			
			if(RDRV_Uart_drv_getCharOfst(rb, &c, p) != 0U)
			{
				res = 9;
			}
			p++;
			frmLen = c;
			sum ^= c;
			
			if(RDRV_Uart_drv_getCharOfst(rb, &c, p) != 0U)
			{
				res = 9; 
			}
			p++;
			frmLen |= ((unsigned short)c) << 8;
			sum ^= c;
			
			if(RDRV_Uart_drv_getCharOfst(rb, &c, p) != 0U)
			{
				res = 9;
			}
			p++;
			sum ^= c;
		}
		
		if(res != 0)
		{
			; 
		}
		else if(sum != 0U)
		{
			res = 5; 
		}
		else
		{
			; 
		}
		
		if(res != 0)
		{
			; 
		}
		else if(FRM_LEN_MAX < frmLen)
		{
			res = 4; 
		}
		else
		{
			; 
		}
		
		if(res != 0)
		{
			; 
		}
		else if((frmLen+8) > rcvLen)
		{
			
			res = 3;
		}
		else
		{
			if(RDRV_Uart_drv_getCharOfst(rb, &c, 7+frmLen) != 0U)
			{
				res = 9; 
			}
		}
		
		if(res != 0)
		{
			; 
		}
		else if(c != FRM_END)
		{
			res = 2; 
		}
		else
		{
			for(; p<(frmLen+5); p++)
			{
				if(RDRV_Uart_drv_getCharOfst(rb, &c, p) != 0U)
				{
					res = 9; 
					break;
				}
				sum ^= c;
			}
		}
		
		if(res != 0)
		{
			;
		}
		else if(sum != 0U)
		{
			res = 1;
		}
		else
		{
			*len = frmLen + 8;
		}
	}
	else
	{
		res = 8; 
	}
	
	return res;
}

/*****************************END OF FILE**************************************/

