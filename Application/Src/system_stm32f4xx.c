/*
 * system_stm32f4xx.c
 *
 *  Created on: Feb 13, 2026
 *      Author: iman
 */


#include <stm32f401xe.h>
#include "core_cm4.h"

#define USER_VECT_TAB_ADDRESS


#define VECT_TAB_BASE_ADDRESS FLASH_BASE

#define VECT_TAB_OFFSET 0X00020000U


void SystemInit(void){
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
#endif

/* Configure the Vector Table location -------------------------------------*/
#if defined(USER_VECT_TAB_ADDRESS)
	SCB->VTOR = VECT_TAB_BASE_ADDRESS | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#endif /* USER_VECT_TAB_ADDRESS */
}
