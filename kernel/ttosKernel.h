#ifndef __TTOSKERNEL_H__
#define __TTOSKERNEL_H__

#include <stdint.h>

typedef void (*KernelTaskHandle_t) (void);

void TaskDefault(void);

void KernelAddTask(KernelTaskHandle_t task, uint32_t priority);
void KernelLaunch(uint32_t quanta);
void KernelInit(void);


#endif 