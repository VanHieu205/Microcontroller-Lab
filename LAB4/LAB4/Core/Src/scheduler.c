/*
 * scheduler.c
 * Created on: March 27, 2025
 * Author: Admin
 */

#include "scheduler.h"

int TIME_CYCLE;
sTasks SCH_tasks_G[SCH_MAX_TASKS];
uint8_t current_index_task = 0;

void SCH_Init(void) {
	while (current_index_task != 0) {
		SCH_Delete_Task(0);
	}
}

void SCH_Update(void) {
	for (int i = 0; i < current_index_task; i++) {
		if (SCH_tasks_G[i].Delay > 0) {
			SCH_tasks_G[i].Delay--;
		} else {
			SCH_tasks_G[i].RunMe++;
			SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period;
		}
	}
}

void SCH_Dispatch_Tasks(void) {
	for (int i = 0; i < current_index_task; i++) {
		if (SCH_tasks_G[i].RunMe > 0) {
			SCH_tasks_G[i].RunMe--;
			(*SCH_tasks_G[i].pTask)();
			if (SCH_tasks_G[i].Period == 0) {
				SCH_Delete_Task(i);
				i--;
			}
		}
	}
}

uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
	if (current_index_task < SCH_MAX_TASKS) {
		SCH_tasks_G[current_index_task].pTask = pFunction;
		SCH_tasks_G[current_index_task].Delay = DELAY / TIME_CYCLE;
		SCH_tasks_G[current_index_task].Period = PERIOD / TIME_CYCLE;
		SCH_tasks_G[current_index_task].RunMe = 0;
		SCH_tasks_G[current_index_task].TaskID = current_index_task;
		current_index_task++;
		return SCH_tasks_G[current_index_task].TaskID;
	}
	return -1;
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
	if (current_index_task == 0 || taskID >= SCH_MAX_TASKS) {
		return -1;
	}
	for (int i = taskID; i < SCH_MAX_TASKS - 1; i++) {
		SCH_tasks_G[i] = SCH_tasks_G[i + 1];
	}
	SCH_tasks_G[SCH_MAX_TASKS - 1].pTask = 0x0000;
	SCH_tasks_G[SCH_MAX_TASKS - 1].Delay = 0;
	SCH_tasks_G[SCH_MAX_TASKS - 1].Period = 0;
	SCH_tasks_G[SCH_MAX_TASKS - 1].RunMe = 0;
	current_index_task--;
	return taskID;
}
