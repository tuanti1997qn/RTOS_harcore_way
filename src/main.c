#include "stm32f10x.h"                  // Device header
#include <stdint.h>
#include "ttosKernel.h"

#define LED 13 // PC13 sink

//volatile uint32_t tick;
//volatile uint32_t _tick;

//void Sys_init();
//uint32_t getTick(void);
//void Delay_s( uint32_t second );


uint32_t stack_1[40];
void ahihi(void)
{
	while(1)
	{
		int t,u,a,n;
		t = 1;
		u = t + 1;
		a = u + 1;
		n = a + 1;
	}
}

int main (void)
{
	//Sys_init();
	KernelInit();
	KernelAddTask(ahihi,1);

	KernelLaunch(10);
	while(1);
}

void Sys_init()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH |= GPIO_CRH_MODE13_1;
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/100U);
	__enable_irq();
}

//void SysTick_Handler(void)
//{
//	++tick;
//}

//uint32_t getTick(void)
//{
//	__disable_irq();
//	_tick = tick;
//	__enable_irq();
//	
//	return _tick;
//}

//void Delay_s( uint32_t second )
//{
//	second *= 100;
//	uint32_t temp = getTick();
//	while((getTick() - temp)<second);
//}
