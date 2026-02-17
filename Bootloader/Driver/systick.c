/*
 * systick.c
 *
 *  Created on: Feb 13, 2026
 *      Author: iman
 */


#include "systick.h"

volatile uint32_t uwTick;

void SysTick_Handler(void){

	uwTick++;

}

uint32_t Get_Tick(void){
	return uwTick;
}

void systick_init(void){
	uint32_t tick = SYSCLK/1000; //1ms tick

	SysTick->VAL = 0; //This is to clear any kind of value inside this Value register
	SysTick->LOAD |= tick - 1;
	SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
	SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);


}

void systick_delaymS(uint32_t delay){

	for (int i = 0; i < delay ;i++){

		while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	}
}

