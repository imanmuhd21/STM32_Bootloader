/*
 * usart.h
 *
 *  Created on: Feb 13, 2026
 *      Author: iman
 */

#include "stm32f401xe.h"
#include "systick.h"
#include <stdbool.h>

#ifndef USART_H_
#define USART_H_
#define APB1CLK SYSCLK

#define USART2EN (1UL << 17)

#define ALTFUNC0 0
#define ALTFUNC1 1
#define ALTFUNC2 2
#define ALTFUNC3 3
#define ALTFUNC4 4
#define ALTFUNC5 5
#define ALTFUNC6 6
#define ALTFUNC7 7
#define ALTFUNC8 8
#define ALTFUNC9 9
#define ALTFUNC10 10
#define ALTFUNC11 11
#define ALTFUNC12 12
#define ALTFUNC13 13
#define ALTFUNC14 14
#define ALTFUNC15 15

void enable_int(void);

void disable_int(void);

void uart2_putchar_int(char charw);

void uart_txint(void);

void uart_putchar(char *sr);

void uart_putchar_integer(int n);

void uart2int_putchar(char charw);



void usart_rcc (USART_TypeDef *USARTx);

void altfunc (GPIO_TypeDef *GPIOx, uint8_t PIN, uint8_t AF);

void uart_set_baudrate115200(USART_TypeDef *USARTx, uint32_t PeriphCLk);

uint32_t compute_uart_baudrate(uint32_t PeriphCLK, uint32_t Baudrate);

void uart2_tx (void);

void usart_init (USART_TypeDef *USARTx, GPIO_TypeDef *GPIOx, uint8_t PIN, uint8_t AF);

void uart2_putcharpoll(char charw);



/*Receive function*/

void uart2rx_init(void);

void uart2_read_int(void);

bool uart2_read(char *charw);

#endif /* USART_H_ */
