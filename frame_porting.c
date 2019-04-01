/* Includes ------------------------------------------------------------------*/
#include "frame_porting.h"
#include "string.h"
/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variable -----------------------------------------------------------*/
 extern unsigned char testArr[100]; /* temporary array, just for test*/
 
/* Private function prototypes------------------------------------------------*/
/* Function implementation ---------------------------------------------------*/
void SendData_LL(unsigned char *data, unsigned short length)
{
  memcpy(testArr, data, length); /* This is just for test in PC */
}

/*****************************END OF FILE**************************************/

