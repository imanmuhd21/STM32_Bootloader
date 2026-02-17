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

	switch(PIN){

	case PIN0:
		GPIOx->BSRR |= BS0;
		break;

	case PIN1:
		GPIOx->BSRR |= BS1;
		break;

	case PIN2:
		GPIOx->BSRR |= BS2;
		break;

	case PIN3:
		GPIOx->BSRR |= BS3;
		break;

	case PIN4:
		GPIOx->BSRR |= BS4;
		break;

	case PIN5:
		GPIOx->BSRR |= BS5;
		break;

	case PIN6:
		GPIOx->BSRR |= BS6;
		break;

	case PIN7:
		GPIOx->BSRR |= BS7;
		break;

	case PIN8:
		GPIOx->BSRR |= BS8;
		break;

	case PIN9:
		GPIOx->BSRR |= BS9;
		break;

	case PIN10:
		GPIOx->BSRR |= BS10;
		break;

	case PIN11:
		GPIOx->BSRR |= BS11;
		break;

	case PIN12:
		GPIOx->BSRR |= BS12;
		break;

	case PIN13:
		GPIOx->BSRR |= BS13;
		break;

	case PIN14:
		GPIOx->BSRR |= BS14;
		break;

	case PIN15:
		GPIOx->BSRR |= BS15;
		break;
	}

}

void gpio_reset(GPIO_TypeDef *GPIOx, uint8_t PIN){
	GPIOx->BSRR |= BR5;
}


bitstatus gpio_read_status(GPIO_TypeDef* GPIOx, uint8_t PIN){
	gpio_rcc(GPIOx);
	gpio_init(GPIOx, PIN, INPUT);

	GPIO_PIN_STATE bitstatus;

	GPIOx->PUPDR &= ~(2UL << (PIN*2));
	GPIOx->PUPDR |= (1UL << (PIN*2));

	if((GPIOx->IDR & (1UL << PIN)) != (uint32_t)(GPIO_PIN_SET))
	{
		bitstatus = GPIO_PIN_RESET;
	}
	else{
		bitstatus = GPIO_PIN_SET;
	}

	return bitstatus;
}
