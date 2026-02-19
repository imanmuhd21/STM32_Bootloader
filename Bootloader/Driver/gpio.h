/*
 * gpio.h
 *
 *  Created on: Feb 13, 2026
 *      Author: iman
 */

#include "stm32f401xe.h"

#ifndef GPIO_H_
#define GPIO_H_

#define GPIOAEN (1UL << 0)
#define GPIOBEN (1UL << 1)
#define GPIOCEN (1UL << 2)
#define GPIODEN  (1UL << 3)
#define GPIOEEN (1UL << 4)
#define GPIOHEN  (1UL << 7)

#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3
#define PIN4 4
#define PIN5 5
#define PIN6 6
#define PIN7 7
#define PIN8 8
#define PIN9 9
#define PIN10 10
#define PIN11 11
#define PIN12 12
#define PIN13 13
#define PIN14 14
#define PIN15 15

#define BS0 (1UL << 0)
#define BS1 (1UL << 1)
#define BS2 (1UL << 2)
#define BS3 (1UL << 3)
#define BS4 (1UL << 4)
#define BS5 (1UL << 5)
#define BS6 (1UL << 6)
#define BS7 (1UL << 7)
#define BS8 (1UL << 8)


#define BR0 (1UL << 16)
#define BR1 (1UL << 17)
#define BR2 (1UL << 18)
#define BR3 (1UL << 19)
#define BR4 (1UL << 20)
#define BR5 (1UL << 21)
#define BR6 (1UL << 22)


#define INPUT 0
#define OUTPUT 1
#define ALTFUNC 2
#define ANALOG 3

typedef enum{
	GPIO_PIN_RESET = 0,
	GPIO_PIN_SET = 1,
}GPIO_PIN_STATE;



void gpio_rcc(GPIO_TypeDef *GPIOx);

void gpio_init(GPIO_TypeDef *GPIOx, uint8_t PIN, uint8_t MODER);

void gpioc_init(GPIO_TypeDef *GPIOx, uint8_t PIN);

void gpioa_led5(void);

void gpio_set(GPIO_TypeDef *GPIOx, uint8_t PIN);

void gpio_reset(GPIO_TypeDef *GPIOx, uint8_t PIN);


GPIO_PIN_STATE gpio_read_status(GPIO_TypeDef *GPIOx, uint8_t PIN);

#endif /* GPIO_H_ */
