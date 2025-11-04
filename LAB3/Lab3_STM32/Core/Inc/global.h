/*
 * global.h
 *
 *  Created on: Oct 16, 2025
 *      Author: ADMIN
 */
#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#define INIT				0
#define MODE1				1
#define MODE2				2
#define MODE3				3
#define MODE4				4

#define RED_GREEN			11
#define RED_AMBER			12
#define GREEN_RED			13
#define	AMBER_RED			14

#define AUTO_RED			21
#define INC_RED				22

#define AUTO_AMBER			31
#define INC_AMBER			32

#define AUTO_GREEN			41
#define INC_GREEN			42

extern int status;

extern int RED;
extern int AMBER;
extern int GREEN;


#endif /* INC_GLOBAL_H_ */
