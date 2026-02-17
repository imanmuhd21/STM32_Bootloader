/*
 * gpio.c
 *
 *  Created on: Feb 13, 2026
 *      Author: iman
 */

#include "gpio.h"


void gpio_rcc(GPIO_TypeDef *GPIOx){

	if (GPIOx == GPIOA){
		RCC->AHB1ENR |= GPIOAEN;
	}
	else if (GPIOx == GPIOB){

		RCC->AHB1ENR |= GPIOBEN;
	}
	else if (GPIOx == GPIOC){
		RCC->AHB1ENR |= GPIOCEN;

	}
	else if (GPIOx == GPIOD){
		RCC->AHB1ENR |= GPIODEN;

	}
	else if (GPIOx == GPIOE){
		RCC->AHB1ENR |= GPIOEEN;

	}
	else {
		RCC->AHB1ENR = GPIOHEN;

	}
}



void gpio_init(GPIO_TypeDef *GPIOx, uint8_t PIN, uint8_t MODER){

	switch(MODER){

	case INPUT:
		GPIOx->MODER &= ~(3UL << (PIN*2));
		break;

	case OUTPUT:
		GPIOx->MODER &= ~(3UL << (PIN*2));
		GPIOx->MODER |= (1UL << (PIN*2));
		break;

	case ALTFUNC:
		GPIOx->MODER &= ~(3UL << (PIN*2));
		GPIOx->MODER |= (2UL << (PIN*2));
		break;

	case ANALOG:
		GPIOx->MODER &= ~(3UL << (PIN*2));
		GPIOx->MODER |= (3UL << (PIN*2));
		break;
	}
}

void gpioa_led5(){
	GPIOA->ODR ^= (1UL << 5);
}


void gpio_set(GPIO_TypeDef *GPIOx, uint8_t PIN){

	GPIOx->BSRR |= BS5;
}

void gpio_reset(GPIO_TypeDef *GPIOx, uint8_t PIN){
	GPIOx->BSRR |= BR5;
}

void gpioc_init(GPIO_TypeDef *GPIOx, uint8_t PIN){
	gpio_rcc(GPIOx);
	gpio_init(GPIOx, PIN, INPUT);
	GPIOx->PUPDR &= ~(2UL << (PIN*2));
	GPIOx->PUPDR |= (1UL << (PIN*2));
}

GPIO_PIN_STATE gpio_read_status(GPIO_TypeDef* GPIOx, uint8_t PIN){


	GPIO_PIN_STATE bitstatus;

	if((GPIOx->IDR & (1UL << PIN)) != (uint32_t)(GPIO_PIN_SET))
	{
		bitstatus = GPIO_PIN_RESET;
	}
	else{
		bitstatus = GPIO_PIN_SET;
	}

	return bitstatus;
}
