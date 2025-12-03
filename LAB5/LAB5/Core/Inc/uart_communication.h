
#ifndef INC_UART_COMMUNICATION_H_
#define INC_UART_COMMUNICATION_H_

#include "main.h"
#include "software_timer.h"
#include "global.h"
#include "command_parser.h"
#include "stdio.h"
#include "stdlib.h"

void uart_communiation_fsm(ADC_HandleTypeDef hadc1, UART_HandleTypeDef huart2);

#endif /* INC_UART_COMMUNICATION_H_ */
