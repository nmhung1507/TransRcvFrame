/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FRAME_DRIVER_H_
#define _FRAME_DRIVER_H_


#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
/**
  * @brief  Get data from global ring, copy to pBuff
  * @param  pBuff : destination buffer
  * @param  buffLen : max length of the data.
  * @retval Length of the available data.
  */
unsigned int FDRV_scif1_gets(unsigned char *pBuff, unsigned int buffLen);

/**
  * @brief  pull data to global memory. This function should be called from receive ISR.
  * @param  buf - buffer
  * @param  length - length of the data
  * @retval None
  */
void FDRV_PullData(unsigned char *buf, int length);

#ifdef __cplusplus
}
#endif

#endif /* _FRAME_DRIVER_H_ */

/******************************END OF FILE*************************************/

