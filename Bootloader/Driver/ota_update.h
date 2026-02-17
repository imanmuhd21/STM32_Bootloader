/*
 * ota_update.h
 *
 *  Created on: Feb 16, 2026
 *      Author: iman
 */

#ifndef OTA_UPDATE_H_
#define OTA_UPDATE_H_

#include <stdint.h>
#include <stdbool.h>

#define OTA_SOF 0XAA	//Start of Frame
#define OTA_EOF 0XBB	//End of Frame
#define OTA_ACK 0		//ACK
#define OTA_NACK 1		//NACK

#define APP_FLASH_ADDR 0x08020000	//Application's Flash address

#define OTA_MAX_DATA_SIZE 1024		//Max data size
#define OTA_DATA_OVERHEAD_SIZE 1	//Data overhead
#define OTA_MAX_PACKET_SIZE (OTA_DATA_OVERHEAD_SIZE + OTA_MAX_DATA_SIZE)


/*Exception codes*/
typedef enum{
	OTA_EX_OK  = 0,	//Success
	OTA_EX_ERR = 1,	//Failure
}OTA_EX_;


/*Type of Packet*/
typedef enum{
	OTA_PACKET_TYPE_CMD 	 = 0, //command
	OTA_PACKET_TYPE_DATA 	 = 1, //data
	OTA_PACKET_TYPE_HEADER   = 2, //header
	OTA_PACKET_TYPE_RESPONSE = 3, //response
}OTA_PACKET_TYPE;


/*State machine for OTA*/
typedef enum{
	OTA_STATE_IDLE 		= 0,
	OTA__STATE_START 	= 1,
	OTA_STATE_HEADER 	= 2,
	OTA_STATE_DATA 		= 3,
	OTA_STATE_END		= 4,
}OTA_STATE;


/*command type*/
typedef enum{
	OTA_CMD_START,
	OTA_CMD_END,
	OTA_CMD_ABORT,
}OTA_CMD;


typedef struct
{
	uint32_t package_size;
	uint32_t package_crc;
	uint32_t reserved1;
	uint32_t reserved2;
}__attribute__((packed)) meta_info;

/*OTA packet format for COMMAND*/
typedef struct
{
	uint8_t  sof;
	uint8_t  packet_type;
	uint16_t length;
	uint8_t  cmd;
	uint32_t crc;
	uint8_t  eof;
}__attribute__((packed)) OTA_COMMAND;


/*OTA packet format for HEADER*/
typedef struct
{
	uint8_t   sof;
	uint8_t   packet_type;
	uint16_t  length;
	meta_info metada;
	uint32_t  crc;
	uint8_t   eof;
}__attribute__((packed)) OTA_HEADER;


/*OTA packet format for DATA*/
typedef struct
{
	uint8_t  sof;
	uint8_t  packet_type;
	uint16_t length;
	uint8_t* data;
	uint32_t crc;
	uint8_t  eof;
}__attribute__((packed)) OTA_DATA;


/*OTA packet format for RESPONSE*/
typedef struct
{
	uint8_t  sof;
	uint8_t  packet_type;
	uint16_t length;
	uint16_t status;
	uint32_t crc;
	uint8_t  eof;
}__attribute__((packed))OTA_RESPONSE;

OTA_EX_ ota_download_and_flash(void);
#endif /* OTA_UPDATE_H_ */
