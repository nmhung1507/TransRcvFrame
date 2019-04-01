/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _NDRV_UART_DRV_RING_H_
#define _NDRV_UART_DRV_RING_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported constants --------------------------------------------------------*/
/* Data Link Layer Header data number[byte] */
#define NDRV_DLL_HEADER_DNUM  4u
/* Data Link Layer Trailer data number[byte] */
#define NDRV_DLL_TRAILER_DNUM 4u 
/* Data Link Layer Frame max length [byte] */
#define NDRV_DLL_FRAME_MAXLEN 1024u 

/*--------- Byte Data ---------*/
/* DLL Frame starter: SFD (Start of Frame Delimiter) */
#define NDRV_DLL_FRAME_SFD 0xABu
/* DLL Frame terminator: EOT (End of Transmission) */
#define NDRV_DLL_FRAME_EOT 0x04u 

#define SCIF_RING_LEN (NDRV_DLL_FRAME_MAXLEN * 4u)

#define FRM_START	(NDRV_DLL_FRAME_SFD)
#define FRM_END		(NDRV_DLL_FRAME_EOT)

#define FRM_LEN_MIN	(0)
#define FRM_LEN_MAX	(NDRV_DLL_FRAME_MAXLEN-NDRV_DLL_HEADER_DNUM-NDRV_DLL_TRAILER_DNUM)

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    unsigned char buf[SCIF_RING_LEN];
    unsigned int wp;
    unsigned int rp;
}SCIF_RING;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void NDRV_scif_initRing(SCIF_RING *rb);
unsigned int RDRV_Uart_drv_getLen(const SCIF_RING *rb);
unsigned int RDRV_Uart_drv_getLast(const SCIF_RING *rb);
void RDRV_Uart_drv_incWpOverRun(SCIF_RING *rb);
void RDRV_Uart_drv_incWp(SCIF_RING *rb);
void RDRV_Uart_drv_incRp(SCIF_RING *rb);
void RDRV_Uart_drv_putChar(SCIF_RING *rb, unsigned char c);
unsigned char RDRV_Uart_drv_getChar(SCIF_RING *rb, unsigned char *c);
unsigned char RDRV_Uart_drv_getCharOfst(const SCIF_RING *rb, unsigned char *c, unsigned int ofst);
unsigned char RDRV_Uart_drv_getStr(SCIF_RING *rb, unsigned char *out, unsigned int cnt);
unsigned char RDRV_Uart_drv_findNext(SCIF_RING *rb, unsigned char head);
unsigned char RDRV_Uart_drv_chkFrame(const SCIF_RING *rb, unsigned int *len);

#ifdef __cplusplus
}
#endif

#endif /* _NDRV_UART_DRV_RING_H_ */

/******************************END OF FILE*************************************/

