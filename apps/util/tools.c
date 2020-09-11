/*
 * tools.c
 *
 *  Created on: Aug 17, 2018
 *      Author: Admin
 */

#include "tools.h"


int mySoftwareReset(void) {

   SYSTEM.PRCR.WORD = 0xA502;  /* Enable writing to the Software Reset */

   SYSTEM.SWRR = 0xA501;            /* Software Reset */

   SYSTEM.PRCR.WORD = 0xA500;  /* Disable writing to the Software Reset */

   return 0;

}

void tools_softDelay(uint32_t ms)
{
	uint32_t lastTime = g_sysTime;
	while(elapsedTime(g_sysTime, lastTime)<ms);
}
