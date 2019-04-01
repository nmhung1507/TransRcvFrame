/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FRAME_RECEIVE_H_
#define _FRAME_RECEIVE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "NCOM_Command.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
/**
  * @brief  Receive and handle data. Call this function in a while loop.
  * @param  None
  * @retval None
  */
void Frame_RecvData();

#ifdef __cplusplus
}
#endif

#endif /* _FRAME_RECEIVE_H_ */

/******************************END OF FILE*************************************/

