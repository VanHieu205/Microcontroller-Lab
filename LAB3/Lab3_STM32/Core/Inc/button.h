/*
 * button.h
 *
 *  Created on: Oct 16, 2025
 *      Author: ADMIN
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"
#include "software_timer.h"

#define NORMAL_STATE 			SET
#define PRESSED_STATE 			RESET
#define NUM_OF_BUTTONS 			3

int isButtonPressed(int index);
void getKeyInput();

#endif /* INC_BUTTON_H_ */
