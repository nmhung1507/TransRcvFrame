/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FRAME_SEND_H_
#define _FRAME_SEND_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
/**
  * @brief  Send frame pack User's API
  * @param  data : data buffer
  * @param  cmdFrameLen : length of the cmd (equal to cmd header + data)
  * @param  src : source address
  * @param  dest : destination address
  * @param  dupNo : duplicate number, increase it if you want to resend a frame.
  * @param  opcode1 : 1st cmd opcode
  * @param  opcode2 : 2nd cmd opcode
  * @retval None
  */
void Frame_SendData(unsigned char *data, unsigned int cmdFrameLen, unsigned char src, unsigned char dest, unsigned char dupNo, unsigned char opcode1, unsigned char opcode2);

#ifdef __cplusplus
}
#endif

#endif /* _FRAME_SEND_H_ */

/******************************END OF FILE*************************************/

