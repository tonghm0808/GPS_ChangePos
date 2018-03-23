#include "main.h"

OS_STK startup_Stk[128];
OS_STK led_Stk[128];
OS_STK gps_Stk[255];

u32 get_IDCode(void);

void startup_Task(void *pdate);
void led_Task(void *pdata);
void gps_Task(void *pdate);


void led_Task(void *pdata)
{
	while (1)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		CoTimeDelay(0, 0, 0, 500);
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
		CoTimeDelay(0, 0, 0, 500);
	}
}

void gps_Task(void *pdate)
{
	StatusType err;
	void *msg;
	while (1)
	{
		msg = CoPendQueueMail(usart_Rx, 0, &err);
		if (err == E_OK)
		{
			gps_Get(*(u8 *)msg);
		}
	}
}

void startup_Task(void *pdate)
{
	bsp_Init();
	usart_Rx = CoCreateQueue(rx_Queue, rx_BuffSize, EVENT_SORT_TYPE_FIFO);
	CoCreateTask(gps_Task, 0, 1, &gps_Stk[255-1], 255);
	CoCreateTask(led_Task, 0, 2, &led_Stk[128-1], 128);
	CoExitTask();
}

u32 get_IDCode(void)
{
	mcuID[2] = *(vu32*)(0x1FFFF7E8);
	mcuID[1] = *(vu32*)(0x1FFFF7EC);
	mcuID[0] = *(vu32*)(0x1FFFF7F0);
	return (mcuID[1] - mcuID[2]);
}

int main(void)
{
	CoInitOS();
	CoCreateTask(startup_Task, 0, 0, &startup_Stk[128-1], 128);
	if (get_IDCode() != chipID) //验证芯片ID进行硬件加密 此处已经取消本功能
	{
		CoStartOS();
	}
	return 0;
}
