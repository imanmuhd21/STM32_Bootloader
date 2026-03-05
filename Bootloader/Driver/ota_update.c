/*
 * ota_update.c
 *
 *  Created on: Feb 16, 2026
 *      Author: iman
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ota_update.h"
#include "myprintf.h"




/*To hold packet*/
static uint8_t rx_buffer[OTA_MAX_PACKET_SIZE];

OTA_STATE ota_state = OTA_STATE_IDLE;

static uint32_t ota_fw_total_size;
static uint32_t ota_fw_total_receive_size;
static uint32_t ota_fw_crc;

static uint16_t ota_packet_check(uint8_t *buf, uint16_t max_len);
static OTA_EX_ ota_packet_operation(uint8_t *buf, uint16_t len);
static void ota_packet_response(uint8_t type);
static bool  write_to_flash(uint8_t *data, uint16_t datalen, bool is_first_block);


volatile uint32_t k = 0;


OTA_EX_ ota_download_and_flash(void){

	OTA_EX_ exception = OTA_EX_OK;
	uint16_t length;

	myprintf("Waiting for OTA Data...\n");

	ota_fw_total_size = 0;
	ota_fw_total_receive_size = 0;
	ota_fw_crc = 0;
	ota_state = OTA_STATE_START;

	do{
		//Clear memory buffer
		memset(rx_buffer, 0, OTA_MAX_PACKET_SIZE);

		length = ota_packet_check(rx_buffer, OTA_MAX_PACKET_SIZE);

		if (length != 0){
			exception = ota_packet_operation(rx_buffer, length);
		}
		else{
			exception = OTA_EX_ERR;
		}

		//Send ACK or NACK
		if(exception == OTA_EX_OK){
			myprintf("Sending ACK\n");
			ota_packet_response(OTA_ACK); //ACK RESPONSE
		}
		else{
			myprintf("Sending NACK\n");
			ota_packet_response(OTA_NACK);//NACK RESPONSE
			break;
		}
	}while(ota_state != OTA_STATE_IDLE);

	return exception;
}

static OTA_EX_ ota_packet_operation(uint8_t *buf, uint16_t len){
	OTA_EX_ ret = OTA_EX_ERR;

	do{

		if (len == 0 || (buf == NULL)){
			break;
		}

		//Check we received OTA Abort command
		OTA_COMMAND *cmd = (OTA_COMMAND *)buf;
		if(cmd->packet_type == OTA_PACKET_TYPE_CMD){
			if(cmd->cmd == OTA_CMD_ABORT){
				//received command to ABORT. Stop process
				break;
			}
		}
		switch (ota_state)
		{
		case OTA_STATE_IDLE:
	        myprintf("ETX_OTA_STATE_IDLE...\n");
	        ret = OTA_EX_OK;
			break;


		case OTA_STATE_START:
			OTA_COMMAND *start = (OTA_COMMAND *)buf;
			if(start->packet_type == OTA_PACKET_TYPE_CMD){
				if(start->cmd == OTA_CMD_START){
					myprintf("Received OTA START Command\n");
					ret = OTA_EX_OK;
					ota_state = OTA_STATE_HEADER;
				}
			}
			break;


		case OTA_STATE_HEADER:
			OTA_HEADER *header = (OTA_HEADER *)buf;
			if (header->packet_type == OTA_PACKET_TYPE_HEADER){
				ota_fw_total_size = header->metadata.package_size;
				ota_fw_crc = header->metadata.package_crc;
				myprintf("Received OTA Header. FW Size = %d\r\n", ota_fw_total_size);
				ota_state = OTA_STATE_DATA;
				ret = OTA_EX_OK;
			}
			break;

		case OTA_STATE_DATA:
			OTA_DATA *data = (OTA_DATA *)buf;
			bool rtx;
			if(data->packet_type == OTA_PACKET_TYPE_DATA){
				rtx = write_to_flash(buf, data->length, ota_fw_total_receive_size == 0);

				if (rtx == true){

					if(ota_fw_total_receive_size >= ota_fw_total_size){
						ota_state = OTA_STATE_END;
					}
					ret = OTA_EX_OK;

				}

			}
			break;

		case OTA_STATE_END:
			OTA_COMMAND *end  = (OTA_COMMAND *)buf;
			if (end->packet_type == OTA_PACKET_TYPE_CMD){
				if (end->cmd == OTA_CMD_END){
					myprintf("Received OTA END Command \n");
					ota_state = OTA_STATE_IDLE;
					ret = OTA_EX_OK;
				}
			}
			break;




		default:
			ret = OTA_EX_ERR;
			break;
		}

	}while(false);

	return ret;
}



static uint16_t ota_packet_check(uint8_t *buf, uint16_t max_len){
	uint16_t index = 0;
	uint16_t data_length;
	uint8_t byte;
	bool ret;
	do{
		k++;
		if (k == 1){
			uart2_read(&byte);
		}

		/*Checking SOF*/
		ret = uart2_receive(&buf[index], 1);
		if (ret != true){
			break;
		}

		if (buf[index++] != OTA_SOF){
			break;
		}

		/*Packet type*/
		ret = uart2_receive(&buf[index++], 1);
		if (ret != true){
			break;
		}

		/*Checking Data Length*/
		ret = uart2_receive(&buf[index], 2);
		if (ret != true){
			break;
		}

		data_length = *(uint16_t *)&buf[index];
		index += 2;

		/*Checking data*/
		ret = uart2_receive(&buf[index], data_length);
		if (ret != true){
			break;
		}
		index += data_length;

		/*Checking CRC*/
		ret  = uart2_receive(&buf[index], 4);
		if (ret != true){
			break;
		}
		index += 4;

		/*Checking End of Frame*/
		ret =uart2_receive(&buf[index], 1);
		if (ret != true){
			break;
		}
		if (buf[index] != OTA_EOF){
			ret = false;
			break;
		}

	}while(false);

	if (ret != true){
		index = 0;
	}

	if(max_len < index){
		myprintf("Received more data than expected. Expected = %d, Received = %d\r\n", max_len, index );
		index = 0;
	}

	return index;
}


static void ota_packet_response(uint8_t type){
	OTA_RESPONSE resp =
	{
			OTA_SOF,
			OTA_PACKET_TYPE_RESPONSE,
			1U,
			type,
			0,
			OTA_EOF
	};

	uint8_t *rsp = (uint8_t *)&resp;
	uart2_transmit(rsp, sizeof(OTA_RESPONSE));
}

static bool write_to_flash(uint8_t *data, uint16_t datalen, bool is_first_block ){
	bool ret;

	do{
		ret = flash_unlock();
		if (ret != true){
				break;
			}
		if (is_first_block){

			myprintf("Erasing the Flash Memory...\n");

			ERASE_TypeDefInit eraseStruct = {ERASE_SECTOR, 5, 1, VOLTAGE_RANGE_3};

			ret = flash_erase(&eraseStruct);
			if (ret != true){
				break;
			}

		}

		myprintf("Writing to Flash...\n");
		for(int i = 0; i < datalen; i++){
			ret = flash_program(PROGRAM_BYTE, (APP_FLASH_ADDR + ota_fw_total_receive_size), data[4+i] );

			if (ret != true){
				myprintf("Flash Write Error\n");
				break;
			}
			else{
				ota_fw_total_receive_size += 1;
			}
		}

		if(ret != true){
			break;
		}

		ret = flash_lock();
		if(ret != true){
			break;
		}

	}while(false);

	return ret;
}
