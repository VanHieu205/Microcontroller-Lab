
#include "command_parser.h"

void command_parser_fsm() {
	switch(status_parser) {
		case INIT_STR:
			// If received char '!', status = WAIT_END, begin reading the command
			if(temp == '!') {
				status_parser = WAIT_END;
				command_index = 0;
			}
			break;

		case WAIT_END:
			// If string ends with char '#', status = INIT_STR, save the command and
			// go to uart_communication_fsm function, set command_flag = 1.
			if(temp == '#') {
				status_parser = INIT_STR;
				command[command_index] = '\0';
				command_flag = 1;
			}
			else {
				// If received char '!', reset command_index, read the command again
				if (temp == '!') command_index = 0;
				// Else continue to read the command
				else {
					command[command_index++] = temp;
					if (command_index == MAX_BUFFER_SIZE) command_index = 0;
				}
			}
			break;

		default:
			break;
	}
}
