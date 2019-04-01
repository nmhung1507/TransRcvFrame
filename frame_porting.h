/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FRAME_PORTING_H
#define _FRAME_PORTING_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
/**
  * @brief  Send data out, eg. UART, BLE. Call low-level function.
  * @param  data : buffer data.
  * @param  length : length of the data
  * @retval None
  */
void SendData_LL(unsigned char *data, unsigned short length);

#ifdef __cplusplus
}
#endif

#endif /* _FRAME_PORTING_H */

/******************************END OF FILE*************************************/

