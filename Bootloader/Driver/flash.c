/*
 * flash.c
 *
 *  Created on: Feb 21, 2026
 *      Author: iman
 */


#include "flash.h"

static void flash_erase_sector(uint32_t sector, uint8_t voltagerange);
static void flash_erase_mass(uint8_t voltagerange);
static bool check_last_operation(void);
static void flash_program_byte(uint32_t address, uint8_t data);
static void flash_program_halfword(uint32_t address, uint16_t data);
static void flash_program_word(uint32_t address, uint32_t data);

bool flash_unlock(void){
	bool status = true;

	if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != RESET){
		WRITE_REG(FLASH->KEYR, FLASH_KEY1);
		WRITE_REG(FLASH->KEYR, FLASH_KEY2);
	}

	if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0){
		status = false;
	}

	return status;
}

bool flash_lock(void){
	FLASH->CR |= FLASH_CR_LOCK;

	return true;
}

bool flash_erase(ERASE_TypeDefInit *eraseInit){
	bool ret;
	if (eraseInit->EraseType == ERASE_SECTOR){

		ret = check_last_operation();
		if(ret == true){
			for (uint8_t idx = eraseInit->Sector; idx < eraseInit->NBSec; idx++){
				flash_erase_sector((uint32_t)idx, eraseInit->VoltageRange);
			}

			ret = check_last_operation();
			if (ret ==  true){

				CLEAR_BIT(FLASH->CR, (FLASH_CR_SER | FLASH_CR_SNB));
			}
		}
	}
	else{
		ret = check_last_operation();
		if(ret == true){
			flash_erase_mass(eraseInit->VoltageRange);
		}

		ret = check_last_operation();
		if (ret == true){

			CLEAR_BIT(FLASH->CR, FLASH_CR_MER);
		}

	}
	return ret;
}

static void flash_erase_sector(uint32_t sector, uint8_t voltagerange){
	uint32_t psize_temp;

	switch (voltagerange){
	case VOLTAGE_RANGE_1:
		psize_temp = FLASH_PSIZE_BYTE;
		break;

	case VOLTAGE_RANGE_2:
		psize_temp = FLASH_PSIZE_HALF_WORD;
		break;

	case VOLTAGE_RANGE_3:
		psize_temp = FLASH_PSIZE_WORD;
		break;

	case VOLTAGE_RANGE_4:
		psize_temp = FLASH_PSIZE_WORD;
		break;
	}

	CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);
	FLASH->CR |= psize_temp;
	CLEAR_BIT(FLASH->CR, FLASH_CR_SNB);
	FLASH->CR |= FLASH_CR_SER | (sector << FLASH_CR_SNB_Pos);
	FLASH->CR |= FLASH_CR_STRT;
}

static void flash_erase_mass(uint8_t voltagerange){
	uint32_t psize_temp;

	switch (voltagerange){
	case VOLTAGE_RANGE_1:
		psize_temp = FLASH_PSIZE_BYTE;
		break;

	case VOLTAGE_RANGE_2:
		psize_temp = FLASH_PSIZE_HALF_WORD;
		break;

	case VOLTAGE_RANGE_3:
		psize_temp = FLASH_PSIZE_WORD;
		break;

	case VOLTAGE_RANGE_4:
		psize_temp = FLASH_PSIZE_WORD;
		break;
	}

	CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);
	FLASH->CR |= psize_temp;
	CLEAR_BIT(FLASH->CR, FLASH_CR_MER);
	FLASH->CR |= FLASH_CR_MER;
	FLASH->CR |= FLASH_CR_STRT;
}

static bool check_last_operation(void){

	bool ret = true;
	while(FLASH->SR == FLASH_SR_BSY);



	if ((FLASH->SR) & (FLASH_SR_WRPERR | FLASH_SR_PGPERR | FLASH_SR_PGPERR | FLASH_SR_PGSERR | FLASH_SR_RDERR)){
		ret = false;
	}

	return ret;
}


bool flash_program(uint8_t programtype, uint32_t address, uint32_t data){
	bool ret = true;

	ret = check_last_operation();
	if(ret == true){
		if (programtype == PROGRAM_BYTE){
			flash_program_byte(address, (uint8_t)data);
		}
		else if (programtype == PROGRAM_HALFWORD){
			flash_program_halfword(address, (uint16_t)data);
		}
		else{
			flash_program_word(address, data);
		}
	}

	ret = check_last_operation();
	if (ret == true){
		CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
	}

	return ret;
}

static void flash_program_byte(uint32_t address, uint8_t data){
	CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);
	FLASH->CR |= FLASH_PSIZE_BYTE;
	FLASH->CR |= FLASH_CR_PG;

	*(volatile uint8_t*)address = data;
}

static void flash_program_halfword(uint32_t address, uint16_t data){
	CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);
	FLASH->CR |= FLASH_PSIZE_BYTE;
	FLASH->CR |= FLASH_CR_PG;

	*(volatile uint16_t*)address = data;
}

static void flash_program_word(uint32_t address, uint32_t data){
	CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);
	FLASH->CR |= FLASH_PSIZE_BYTE;
	FLASH->CR |= FLASH_CR_PG;

	*(volatile uint32_t*)address = data;
}
