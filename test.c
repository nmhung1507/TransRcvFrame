#include <stdio.h>
#include <stdlib.h>
#include "frame_receive.h"
#include "frame_send.h"

unsigned char DataSent[8] = {1, 2, 3, 4, 5, 6, 7, 8};
unsigned char testArr[100] = {0};
int main(int argc, char *argv[]) {
	printf("Start");
	/* Send a packet */
	Frame_SendData(DataSent, 8, 0x11, 0x22, 1, 0x01, 0x02);
	
	/* Pull data to the ring, in real system, this must be call when receiving data */
	FDRV_PullData(testArr, 100);
	
	/* Receive and handle data, in real system, this should be call in a while loop */
	Frame_RecvData();
	return 0;
}

