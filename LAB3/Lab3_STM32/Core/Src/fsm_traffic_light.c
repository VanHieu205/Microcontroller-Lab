/*
 * fsm_traffic_light.c
 *
 *  Created on: Oct 16, 2025
 *      Author: ADMIN
 */
#include "fsm_traffic_light.h"

int status = INIT;

void fsm_traffic_light(){
	switch(status){

	//INIT
	case INIT:
		status = MODE1;
		break;

	//MODE1
	case MODE1:
		status = RED_GREEN;
		//Set up LED
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET);
		HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, SET);
		HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET);
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, SET);
		HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, SET);
		HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, RESET);

		//Set up default value
		counter1 = counter2 = 1;
		index_led = 0;
		updateLedBufferMode1();
		update7SEG(index_led);

		//Set timer
		setTimer(0, GREEN * 1000);	// Set timer for RED_GREEN
		setTimer(1, 1000);			// Set timer for updateLedBufferMode1
		setTimer(2, 250);			// Set timer for 7SEG LED
		clearTimer(3);				// Clear timer for blinking LED

		break;

	case RED_GREEN:
		//Set up LED
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET);
		HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, SET);
		HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, SET);
		HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, RESET);

		if (timer_flag[0] == 1){
			status = RED_AMBER;
			setTimer(0, AMBER * 1000);
		}

		if (timer_flag[1] == 1){
			updateLedBufferMode1();
			setTimer(1, 1000);
		}

		if (timer_flag[2] == 1){
			update7SEG(index_led);
			setTimer(2, 250);
		}
		// If button 1 is pressed, come to MODE2
		if (isButtonPressed(0) == 1){
			status = MODE2;
		}
		break;

	case RED_AMBER:
		//Set up LED
		HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, SET);
		HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, RESET);

		if (timer_flag[0] == 1){
			status = GREEN_RED;
			setTimer(0, GREEN * 1000);
		}

		if (timer_flag[1] == 1){
			updateLedBufferMode1();
			setTimer(1, 1000);
		}

		if (timer_flag[2] == 1){
			update7SEG(index_led);
			setTimer(2, 250);
		}
		// If button 1 is pressed, come to MODE2
		if (isButtonPressed(0) == 1){
			status = MODE2;
		}
		break;

	case GREEN_RED:
		//Set up LED
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, SET);
		HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, RESET);
		HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, SET);
		HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, RESET);

		if (timer_flag[0] == 1){
			status = AMBER_RED;
			setTimer(0, AMBER * 1000);
		}

		if (timer_flag[1] == 1){
			updateLedBufferMode1();
			setTimer(1, 1000);
		}

		if (timer_flag[2] == 1){
			update7SEG(index_led);
			setTimer(2, 250);
		}
		// If button 1 is pressed, come to MODE2
		if (isButtonPressed(0) == 1){
			status = MODE2;
		}
		break;

	case AMBER_RED:
		//Set up LED
		HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, RESET);
		HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET);

		if (timer_flag[0] == 1){
			status = RED_GREEN;
			setTimer(0, GREEN * 1000);
		}

		if (timer_flag[1] == 1){
			updateLedBufferMode1();
			setTimer(1, 1000);
		}

		if (timer_flag[2] == 1){
			update7SEG(index_led);
			setTimer(2, 250);
		}
		// If button 1 is pressed, come to MODE2
		if (isButtonPressed(0) == 1){
			status = MODE2;
		}
		break;

	//MODE2
	case MODE2:
		status = AUTO_RED;
		//Set up RED LED
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET);
		HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, SET);
		HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET);
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET);
		HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, SET);
		HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, SET);

		index_led = 0;
		updateLedBufferMode2();
		update7SEG(index_led);

		setTimer(2, 250);			// Set timer for 7SEG LED
		setTimer(3, 500);			// Set timer for blinking LED
		clearTimer(0);				// Clear timer display LED in MODE1
		clearTimer(1);				// Clear timer update buffer
		break;


	case AUTO_RED:
		if (timer_flag[2] == 1){
			updateLedBufferMode2();
			update7SEG(index_led);
			setTimer(2, 250);
		}
		if (timer_flag[3] == 1){
			HAL_GPIO_TogglePin(R1_GPIO_Port, R1_Pin);
			HAL_GPIO_TogglePin(R2_GPIO_Port, R2_Pin);
			setTimer(3, 250);
		}
		if (isButtonPressed(0) == 1){
			status = MODE3;
		}
		if (isButtonPressed(1) == 1){
			status = INC_RED;
			if (RED >= 99) RED = GREEN + 1;
			else RED++;
		}
		break;

	case INC_RED:
		if (timer_flag[2] == 1){
			updateLedBufferMode2();
			update7SEG(index_led);
			setTimer(2, 250);
		}
		if (timer_flag[3] == 1){
			HAL_GPIO_TogglePin(R1_GPIO_Port, R1_Pin);
			HAL_GPIO_TogglePin(R2_GPIO_Port, R2_Pin);
			setTimer(3, 250);
		}
		if (isButtonPressed(1) == 1){
			status = INC_RED;
			if (RED >= 99) RED = GREEN + 1;
			else RED++;
		}
		if (isButtonPressed(2) == 1){
			status = AUTO_RED;
//			GREEN = RED - AMBER;
		}
		break;


	//MODE3
	case MODE3:
		status = AUTO_AMBER;
		//Set up AMBER LED
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, SET);
		HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, RESET);
		HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET);
		HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, SET);
		HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, RESET);
		HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, SET);

		index_led = 0;
		updateLedBufferMode3();
		update7SEG(index_led);

		setTimer(2, 250);			// Set timer for 7SEG LED
		setTimer(3, 500);			// Set timer for blinking LED
		clearTimer(0);				// Clear timer display LED in MODE1
		clearTimer(1);				// Clear timer update buffer
		break;

	case AUTO_AMBER:
		if (timer_flag[2] == 1){
			updateLedBufferMode3();
			update7SEG(index_led);
			setTimer(2, 250);
		}
		if (timer_flag[3] == 1){
			HAL_GPIO_TogglePin(A1_GPIO_Port, A1_Pin);
			HAL_GPIO_TogglePin(A2_GPIO_Port, A2_Pin);
			setTimer(3, 250);
		}
		if (isButtonPressed(0) == 1){
			status = MODE4;
		}
		if (isButtonPressed(1) == 1){
			status = INC_AMBER;
			if (AMBER >= 4) AMBER = 1;
			else AMBER++;
		}
		break;

	case INC_AMBER:
		if (timer_flag[2] == 1){
			updateLedBufferMode3();
			update7SEG(index_led);
			setTimer(2, 250);
		}
		if (timer_flag[3] == 1){
			HAL_GPIO_TogglePin(A1_GPIO_Port, A1_Pin);
			HAL_GPIO_TogglePin(A2_GPIO_Port, A2_Pin);
			setTimer(3, 250);
		}
		if (isButtonPressed(1) == 1){
			status = INC_AMBER;
			if (AMBER >= 4) AMBER = 1;
			else AMBER++;
		}
		if (isButtonPressed(2) == 1){
			status = AUTO_AMBER;
//			if (RED <= AMBER) RED = AMBER + 1;
//			GREEN = RED - AMBER;
		}
		break;

	//MODE4
	case MODE4:
		status = AUTO_GREEN;
		//Set up GREEN LED
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, SET);
		HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, SET);
		HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, RESET);
		HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, SET);
		HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, SET);
		HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, RESET);

		index_led = 0;
		updateLedBufferMode4();
		update7SEG(index_led);

		setTimer(2, 250);			// Set timer for 7SEG LED
		setTimer(3, 500);			// Set timer for blinking LED
		clearTimer(0);				// Clear timer display LED in MODE1
		clearTimer(1);				// Clear timer update buffer
		break;

	case AUTO_GREEN:
		if (timer_flag[2] == 1){
			updateLedBufferMode4();
			update7SEG(index_led);
			setTimer(2, 250);
		}
		if (timer_flag[3] == 1){
			HAL_GPIO_TogglePin(G1_GPIO_Port, G1_Pin);
			HAL_GPIO_TogglePin(G2_GPIO_Port, G2_Pin);
			setTimer(3, 250);
		}
		if (isButtonPressed(0) == 1){
			status = MODE1;
			if (GREEN <= AMBER) GREEN = AMBER + 1;
			RED = GREEN + AMBER;
		}
		if (isButtonPressed(1) == 1){
			status = INC_GREEN;
			if (GREEN >= 99) GREEN = RED - AMBER;
			else GREEN++;
		}
		break;

	case INC_GREEN:
		if (timer_flag[2] == 1){
			updateLedBufferMode4();
			update7SEG(index_led);
			setTimer(2, 250);
		}
		if (timer_flag[3] == 1){
			HAL_GPIO_TogglePin(G1_GPIO_Port, G1_Pin);
			HAL_GPIO_TogglePin(G2_GPIO_Port, G2_Pin);
			setTimer(3, 250);
		}
		if (isButtonPressed(1) == 1){
			status = INC_GREEN;
			if (GREEN >= 99) GREEN = RED - AMBER;
			else GREEN++;
		}
		if (isButtonPressed(2) == 1){
			status = AUTO_GREEN;
//			RED = GREEN + AMBER;
		}
		break;

	default:
		break;

	}
}

