#include "scheduler.h"

int TIME_CYCLE = 1;


sTasks* timer_wheel[WHEEL_SIZE];
uint8_t current_slot = 0;

static sTasks task_pool[SCH_MAX_TASKS];
static uint8_t task_count = 0;

void SCH_Init(void) {
    for (int i = 0; i < WHEEL_SIZE; i++) {
        timer_wheel[i] = 0;
    }
    task_count = 0;
    current_slot = 0;
}

uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
    if (task_count >= SCH_MAX_TASKS)
        return -1;

    sTasks* t = &task_pool[task_count];

    t->pTask = pFunction;
    t->Delay = DELAY / TIME_CYCLE;
    t->Period = PERIOD / TIME_CYCLE;
    t->RunMe = 0;
    t->TaskID = task_count;
    t->next = 0;

    uint32_t targetSlot = (current_slot + t->Delay) % WHEEL_SIZE;

    t->next = timer_wheel[targetSlot];
    timer_wheel[targetSlot] = t;

    task_count++;
    return t->TaskID;
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
    if (taskID >= task_count) return -1;

    sTasks* t = &task_pool[taskID];


    for (int i = 0; i < WHEEL_SIZE; i++) {
        sTasks* prev = 0;
        sTasks* p = timer_wheel[i];

        while (p != 0) {
            if (p == t) {
                if (prev == 0)
                    timer_wheel[i] = p->next;
                else
                    prev->next = p->next;

                t->pTask = 0;
                t->next = 0;
                return 1;
            }
            prev = p;
            p = p->next;
        }
    }
    return 0;
}

void SCH_Update(void) {
    current_slot = (current_slot + 1) % WHEEL_SIZE;

}

void SCH_Dispatch_Tasks(void) {
    sTasks* p = timer_wheel[current_slot];
    timer_wheel[current_slot] = 0;

    while (p != 0) {
        sTasks* nextTask = p->next;

        p->RunMe++;

        // Chạy task
        if (p->RunMe > 0) {
            p->RunMe--;
            (*p->pTask)();
        }

        if (p->Period == 0) {
            SCH_Delete_Task(p->TaskID);
        } else {
            uint32_t nextSlot = (current_slot + p->Period) % WHEEL_SIZE;
            p->next = timer_wheel[nextSlot];
            timer_wheel[nextSlot] = p;
        }

        p = nextTask;
    }
}
