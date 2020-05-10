#include "ttosKernel.h"
#include "stdint.h"
#include "ttosConfig.h"
#include "stdlib.h"
#include "stm32f10x.h"                  

#define NUM_OF_THREAD 3

/* stack control block struct */
struct tcb{
	uint32_t *StackPtr;   // point to top of the stack pointer
	struct tcb *NextPtr;	// point to the next tcb
	int32_t priority;			// the higher number the higher order
};

typedef struct tcb tcpType;

/* define some nessessary var */
tcpType *tcbHead;
tcpType *CurrentPtr;
volatile uint32_t tick;


/* define the static function*/

static void KernerStackInit (uint32_t *StackPtr, KernelTaskHandle_t task)
{
/*
	remember the stack frame, you can see at here http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Babefdjc.html
	Higher address
	|	xPXR	// set bit 24 to enable thump mode
	|	PC		// assign the address of the task
	|	LR		// link register
	| R12
	|	R3
	| R2
	| R1
	V R0
	Lower address
*/
	*(--StackPtr) = 1 << 24;  //	xPXR
	*(--StackPtr) = (uint32_t)(TaskDefault); // PC
	*(--StackPtr) = 0xD;	//	LR
	*(--StackPtr) = 0xEA;	// 	R12
	*(--StackPtr) = 0xD;	//	R3
	*(--StackPtr) = 0xB;	//	R2
	*(--StackPtr) = 0xEE;	//	R1
	*(--StackPtr) = 0xF;	//	R0
}

void TaskDefault (void)
{
	int a;
	int b;
	while(1)
	{
			a = 100;
			b = a + 100;

	}
}


//void SysTick_Handler(void)
//{
//	++tick;
//	TestStack();
//}



/* define the extern fuction */


void KernelInit(void)
{
	tcbHead = (tcpType*)calloc (STACK_SIZE,sizeof(uint32_t));
	CurrentPtr = (tcpType*)calloc (STACK_SIZE,sizeof(uint32_t));
	uint32_t *temp= (uint32_t*)CurrentPtr;
	CurrentPtr->StackPtr = temp+STACK_SIZE;
	//uint32_t DefaultTaskStack[STACK_SIZE];
	temp= (uint32_t*)tcbHead;
	tcbHead->StackPtr = temp+STACK_SIZE;
	tcbHead->priority = 0;
	tcbHead->NextPtr = tcbHead;
	
	
	*(--tcbHead->StackPtr) = 1 << 24;  //	xPXR
	*(--tcbHead->StackPtr) = (uint32_t)(TaskDefault); // PC
	*(--tcbHead->StackPtr) = (uint32_t)(TaskDefault);		//	LR, hold the value for the first init to go to default task
	*(--tcbHead->StackPtr) = 0xEA;	// 	R12
	*(--tcbHead->StackPtr) = 0xD;		//	R3
	*(--tcbHead->StackPtr) = 0xB;		//	R2
	*(--tcbHead->StackPtr) = 0xEE;	//	R1
	*(--tcbHead->StackPtr) = 0xF;		//	R0
	
	*(--tcbHead->StackPtr) = 0xD;		//	R4
	*(--tcbHead->StackPtr) = 0xE;	// 	R5
	*(--tcbHead->StackPtr) = 0xA;		//	R6
	*(--tcbHead->StackPtr) = 0xD;		//	R7
	*(--tcbHead->StackPtr) = 0xB;	//	R8
	*(--tcbHead->StackPtr) = 0xF;		//	R9
	*(--tcbHead->StackPtr) = 0xE;	//	R10
	*(--tcbHead->StackPtr) = 0xF;		//	R11
	//CurrentPtr->NextPtr = tcbHead;
	CurrentPtr=tcbHead;


//	CurrentPtr = tcbHead;
	//TestStack();
}

void KernelAddTask(KernelTaskHandle_t task, uint32_t priority)
{
	__disable_irq();
	tcpType *new_tcb = (tcpType*)calloc (STACK_SIZE,sizeof(uint32_t));
	uint32_t *temp= (uint32_t*)new_tcb;
	new_tcb->StackPtr = temp+STACK_SIZE;
	*(--new_tcb->StackPtr) = 1 << 24;  //	xPXR
	*(--new_tcb->StackPtr) = (uint32_t)(task); // PC
	*(--new_tcb->StackPtr) = 0xD;		//	LR, hold the value for the first init to go to default task
	*(--new_tcb->StackPtr) = 0xEA;	// 	R12
	*(--new_tcb->StackPtr) = 0xD;		//	R3
	*(--new_tcb->StackPtr) = 0xB;		//	R2
	*(--new_tcb->StackPtr) = 0xEE;	//	R1
	*(--new_tcb->StackPtr) = 0xF;		//	R0
	
	*(--new_tcb->StackPtr) = 0xD;		//	R4
	*(--new_tcb->StackPtr) = 0xE;	// 	R5
	*(--new_tcb->StackPtr) = 0xA;		//	R6
	*(--new_tcb->StackPtr) = 0xD;		//	R7
	*(--new_tcb->StackPtr) = 0xB;	//	R8
	*(--new_tcb->StackPtr) = 0xF;		//	R9
	*(--new_tcb->StackPtr) = 0xE;	//	R10
	*(--new_tcb->StackPtr) = 0xF;		//	R11
	
	new_tcb->NextPtr = CurrentPtr;
	new_tcb->priority = priority;
	CurrentPtr->NextPtr = new_tcb;
	__enable_irq();
}

void KernelLaunch(uint32_t quanta_ms)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/100U);
	osSchedulerLaunch();
	//__enable_irq();
	
}

