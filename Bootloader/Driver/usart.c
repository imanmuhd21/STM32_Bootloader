/*
 * usart.c
 *
 *  Created on: Feb 13, 2026
 *      Author: iman
 */


#include "usart.h"
#include "ring_buffer.h"

static uint8_t tx_array[16];

static ringbuffer *tx_buff;

volatile uint32_t count = 0;

void disable_int(void){
	USART2->CR1 &= ~(1UL << 13);
	USART2->CR1 &= ~(1UL << 7);
	USART2->CR1 |= (1UL << 13);
}


void enable_int(void){
	USART2->CR1 &= ~(1UL << 13);
	USART2->CR1 |= (1UL << 7);
	USART2->CR1 |= (1UL << 13);
}

void USART2_IRQHandler(void){
	if (USART2->SR & (1UL << 7)){
		if (empty_ring_buff(tx_buff)){\
			count++;
 			while(1);
		}

		get_ring_buff(tx_buff);

		if(!(empty_ring_buff((tx_buff)))){

			USART2->DR = peek_ring_buff(tx_buff);

		}
		//only disable ISR when tail+1 = head
		if (guard_ring_buff(tx_buff)){
			disable_int();
		}
	}
}

void usart_rcc(USART_TypeDef *USARTx){

	RCC->APB1ENR |= USART2EN;

}

void altfunc(GPIO_TypeDef *GPIOx, uint8_t PIN, uint8_t AF){

	if (PIN >= 8){
		switch (AF){

		case ALTFUNC0:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC0) << ((PIN-8)*4));
			break;

		case ALTFUNC1:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC1) << ((PIN-8)*4));
			break;

		case ALTFUNC2:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC2) << ((PIN-8)*4));
			break;
		case ALTFUNC3:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC3) << ((PIN-8)*4));
			break;

		case ALTFUNC4:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC4) << ((PIN-8)*4));
			break;

		case ALTFUNC5:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC5) << ((PIN-8)*4));
			break;

		case ALTFUNC6:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC6) << ((PIN-8)*4));
			break;

		case ALTFUNC7:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC7) << ((PIN-8)*4));
			break;

		case ALTFUNC8:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC8) << ((PIN-8)*4));
			break;

		case ALTFUNC9:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC9) << ((PIN-8)*4));
			break;

		case ALTFUNC10:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC10) << ((PIN-8)*4));
			break;

		case ALTFUNC11:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC11) << ((PIN-8)*4));
			break;

		case ALTFUNC12:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC12) << ((PIN-8)*4));
			break;

		case ALTFUNC13:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC13) << ((PIN-8)*4));
			break;

		case ALTFUNC14:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC14) << ((PIN-8)*4));
			break;

		case ALTFUNC15:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC15) << ((PIN-8)*4));
			break;

		}

	}
	else{
		switch (AF){

		case ALTFUNC0:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC0) << (PIN*4));
			break;

		case ALTFUNC1:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC1) << (PIN*4));
			break;

		case ALTFUNC2:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC2) << (PIN*4));
			break;

		case ALTFUNC3:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC3) << (PIN*4));
			break;

		case ALTFUNC4:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC4) << (PIN*4));
			break;

		case ALTFUNC5:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC5) << (PIN*4));
			break;

		case ALTFUNC6:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC6) << (PIN*4));
			break;

		case ALTFUNC7:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC7) << (PIN*4));
			break;

		case ALTFUNC8:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC8) << (PIN*4));
			break;

		case ALTFUNC9:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC9) << (PIN*4));
			break;

		case ALTFUNC10:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC10) << (PIN*4));
			break;

		case ALTFUNC11:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC11) << (PIN*4));
			break;

		case ALTFUNC12:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC12) << (PIN*4));
			break;

		case ALTFUNC13:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC13) << (PIN*4));
			break;

		case ALTFUNC14:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC14) << (PIN*4));
			break;

		case ALTFUNC15:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC15) << (PIN*4));
			break;

				}
	}

}

uint32_t compute_uart_baudrate(uint32_t PeriphClk, uint32_t Baudrate){
	return ((PeriphClk +Baudrate/2)/Baudrate);
}

void uart_set_baudrate115200(USART_TypeDef *USARTx, uint32_t PeriphClk){

	USARTx->BRR = compute_uart_baudrate(PeriphClk, 115200UL);

}

void usart_init(USART_TypeDef *USARTx, GPIO_TypeDef *GPIOx, uint8_t PIN, uint8_t AF){

	usart_rcc(USART2);

	// 3. AF7 for USART2_TX
	altfunc(GPIOx, PIN, AF);

	uart_set_baudrate115200(USARTx,APB1CLK);
}

void uart2_tx(void){
	USART2->CR1 &= ~(1UL << 13);
	USART2->CR1 |= (1UL << 3);
	USART2->CR1 |= (1UL << 13);
}

void uart2_putcharpoll(char charw){
	while(!(USART2->SR & (1UL << 7)));
	USART2->DR = charw;

	if (charw == '\n'){
		uart2_putcharpoll('\r');
	}

}

/*ring_buffer.h usage section*/

void uart_txint(void){
	USART2->CR1 &= ~(1UL << 13);
	USART2->CR1 |= (1UL << 3);
	USART2->CR1 |= (1UL << 13);

	tx_buff = create_ring_buff(tx_array, sizeof(tx_array));

	NVIC_EnableIRQ(USART2_IRQn);
}

void uart2_putchar_int(char charw){

	while (full_ring_buff(tx_buff));

	disable_int();

	const bool tx_ongoing = !(empty_ring_buff(tx_buff));

	put_ring_buff(tx_buff, charw);

	if (!tx_ongoing){
		USART2->DR = peek_ring_buff(tx_buff);

	}

	enable_int();

	if (charw == '\n'){
		uart2_putchar_int('\r');
	}
}

void wait_empty_buff(void){
	while(!(guard_ring_buff(tx_buff)));
}

void uart_putchar(char *sr){

	for (int i = 0; sr[i] != '\0'; i++){

		uart2_putchar_int(sr[i]);
	}

}

void uart_putchar_integer(int n){

	uint8_t buff[4];

	int i = 0;

	if (n == '0'){
		uart2_putchar_int(n);
	}

	while (n > 0){
		buff[i++] = n%10 + '0';
		n = n/10;
	}

	while (i > 0){
		i--;
		uart2_putchar_int((char)buff[i]);
	}
}
