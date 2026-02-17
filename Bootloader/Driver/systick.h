/*
 * systick.h
 *
 *  Created on: Feb 13, 2026
 *      Author: iman
 */

#include "stm32f401xe.h"

#ifndef SYSTICK_H_
#define SYSTICK_H_

#define SYSCLK 16000000UL

uint32_t Get_Tick(void);

void systick_init(void);

void systick_delaymS(uint32_t delay);

#endif /* SYSTICK_H_ */
