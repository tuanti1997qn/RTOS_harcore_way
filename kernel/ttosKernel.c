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

tcpType *tcbHead;
tcpType *CurrentPtr;


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
	--StackPtr;
	// TestStack();
}

void TaskDefault (void)
{
	int a;
	int b;
	a = 100;
	b = a + 100;
}

/* define the extern fuction */


void KernelInit(void)
{
	tcbHead = (tcpType*)calloc (1, sizeof(tcpType));
	uint32_t DefaultTaskStack[STACK_SIZE];
	tcbHead->StackPtr = __get_MSP();
	tcbHead->priority = 0;
	tcbHead->NextPtr = NULL;
	
	*(--tcbHead->StackPtr) = 1 << 24;  //	xPXR
	*(--tcbHead->StackPtr) = (uint32_t)(TaskDefault); // PC
	*(--tcbHead->StackPtr) = 0xD;		//	LR
	*(--tcbHead->StackPtr) = 0xEA;	// 	R12
	*(--tcbHead->StackPtr) = 0xD;		//	R3
	*(--tcbHead->StackPtr) = 0xB;		//	R2
	*(--tcbHead->StackPtr) = 0xEE;	//	R1
	*(--tcbHead->StackPtr) = 0xF;		//	R0
	--tcbHead->StackPtr;
	TestStack();
}

void KernelAddTask(KernelTaskHandle_t task, uint32_t priority)
{
	__disable_irq();
	tcpType *new_tcb = (tcpType*)calloc (1, sizeof(tcpType));
	new_tcb->NextPtr = tcbHead;
	tcbHead = new_tcb;
	new_tcb->priority = priority;
	__enable_irq();
}


int32_t TCB_STACK[NUM_OF_THREAD][STACK_SIZE];