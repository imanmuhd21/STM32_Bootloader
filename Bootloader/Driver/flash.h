/*
 * flash.h
 *
 *  Created on: Feb 21, 2026
 *      Author: iman
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "stm32f401xe.h"
#include "stm32f4xx.h" //READ_BIT() & WRITE_BIT() usage
#include <stdbool.h>
#include <stdio.h>

#define FLASH_KEY1 0x45670123
#define FLASH_KEY2 0xCDEF89AB

#define FLASH_PSIZE_BYTE 		(0U << 8)
#define FLASH_PSIZE_HALF_WORD 	(1U << 8)
#define FLASH_PSIZE_WORD 		(2U << 8)
#define FLASH_PSIZE_DOUBLE_WORD (3U << 8)



typedef uint8_t VoltageRange_t;
typedef uint8_t ERASETYPE_t;

typedef enum{
	ERASE_SECTOR,
	ERASE_MASS,
}ERASETYPE;


typedef enum{
	VOLTAGE_RANGE_1,	//1.7v - 2.1v
	VOLTAGE_RANGE_2,	//2.1v - 2.4v @  2.4v - 2.7v
	VOLTAGE_RANGE_3,	//2.7v - 3.6v
	VOLTAGE_RANGE_4, 	//2.7v - 3.6v with External Vpp
}VoltageRange_;

typedef enum{
	PROGRAM_BYTE,
	PROGRAM_HALFWORD,
	PROGRAM_WORD,
}_PORGRAMTYPE;

typedef struct{
	ERASETYPE_t EraseType;
	uint8_t Sector;
	uint8_t NBSec;
	VoltageRange_t VoltageRange;
}ERASE_TypeDefInit;


bool flash_unlock(void);

bool flash_lock(void);

bool flash_erase(ERASE_TypeDefInit *eraseInit);

bool flash_program(uint8_t programtype, uint32_t address, uint32_t data);

#endif /* FLASH_H_ */
