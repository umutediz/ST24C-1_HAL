/*
 * eeprom.h
 *
 *  Created on: Mar 23, 2022
 *      Author: umutediz
 */

#ifndef INC_M24C16_H_
#define INC_M24C16_H_

#include "main.h"

#define EEPROM_BASE_ADDR         (0x50)
#define EEPROM_PAGE_COUNT        (128)
#define EEPROM_SIZE              (2048)
#define EEPROM_PAGE_SIZE         (16)
#define EEPROM_BYTE_ADDRESS_MASK ((1 << 12) - 1)
#define EEPROM_MAX_LEN			 (1 << 4)
#define EEPROM_BUS_SPEED_KHZ     (100000.0f)
#define EEPROM_WR_SEQ_TIMEOUT    (5000)

typedef enum EEPROMReturn {
	EEPROMTrue,
	EEPROMFalse
} tEEPROMReturn;

tEEPROMReturn EEPROMRead(uint16_t byte_addr, uint16_t size, uint8_t *data, uint8_t try_count);
tEEPROMReturn EEPROMWrite(uint16_t byte_addr, uint16_t size, uint8_t *data, uint8_t try_count);

#endif /* INC_M24C16_H_ */
