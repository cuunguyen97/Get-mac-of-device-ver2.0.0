
/**************************************************************************************************/
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	

#include "misc.h"
#include "sys.h"
#include "timer.h"
#include "delay.h"

uint32_t dwCalculatorTime(uint32_t dwTimeInit,uint32_t dwTimeCurrent)
{
	if(dwTimeCurrent >= dwTimeInit)
	{
		return (dwTimeCurrent - dwTimeInit);
	}else
	{
		return (0xFFFFFFFF + dwTimeCurrent - dwTimeInit);
	}
}
void delay_ms(u32 nms)
{	 	 
	uint32_t dwTimeInit = GetMilSecTick();
	uint32_t dwTimeCurrent = 0;

	do{
		dwTimeCurrent = GetMilSecTick();
	}
	while(dwCalculatorTime(dwTimeInit, dwTimeCurrent)<nms);
}


			 



































