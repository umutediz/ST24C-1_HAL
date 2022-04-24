/*
 * eeprom.c
 *
 *  Created on: Mar 23, 2022
 *      Author: umutediz
 */

#include "m24c16.h"

extern I2C_HandleTypeDef hi2c1;

union eepromaddr {
	uint16_t byte_addr;
	uint8_t byte_addr_arr[2];
} uAddr;

tEEPROMReturn EEPROMRead(uint16_t byte_addr, uint16_t size, uint8_t *data, uint8_t try_count) {

	if (((uint32_t) byte_addr + (uint32_t) size) > EEPROM_SIZE) {
		return EEPROMFalse;
	}

	if (data == NULL) {
		return EEPROMFalse;
	}

	uint16_t timeout = (((1.0 / EEPROM_BUS_SPEED_KHZ * 10) * (2 + size)) * 1e3) + 1;
	while (try_count > 0) {
		uAddr.byte_addr = (byte_addr & EEPROM_BYTE_ADDRESS_MASK) | (EEPROM_BASE_ADDR << 8);
		if (HAL_I2C_Mem_Read(&hi2c1, uAddr.byte_addr_arr[1] << 1, uAddr.byte_addr_arr[0], 1, data, size, timeout) == HAL_OK) {
			return EEPROMTrue;
		}
		else {
			try_count--;
		}

	}
	return EEPROMFalse;
}

tEEPROMReturn EEPROMWrite(uint16_t byte_addr, uint16_t size, uint8_t *data, uint8_t try_count) {

	if (((uint32_t) byte_addr + (uint32_t) size) > EEPROM_SIZE) {
		return EEPROMFalse;
	}

	if (data == NULL) {
		return EEPROMFalse;
	}

	int32_t remaining_bytes = size;
	while ((remaining_bytes > 0) && (try_count > 0)) {
		uAddr.byte_addr = (byte_addr & EEPROM_BYTE_ADDRESS_MASK) | (EEPROM_BASE_ADDR << 8);
		uint8_t bus_op_size = remaining_bytes >= EEPROM_MAX_LEN ? EEPROM_MAX_LEN : remaining_bytes;
		uint16_t timeout = (((1.0 / EEPROM_BUS_SPEED_KHZ * 10) * (2 + bus_op_size)) * 1e3) + 1;
		if (HAL_I2C_Mem_Write(&hi2c1, uAddr.byte_addr_arr[1] << 1, uAddr.byte_addr_arr[0], 1, (data + (size - remaining_bytes)), bus_op_size, timeout) == HAL_OK) {
			remaining_bytes -= bus_op_size;
			byte_addr += bus_op_size;
			if (remaining_bytes <= 0) {
				return EEPROMTrue;
			}

			uint32_t time = HAL_GetTick();
			while (HAL_GetTick() - time < EEPROM_WR_SEQ_TIMEOUT) {
				uint8_t dummy;
				if (HAL_I2C_Master_Transmit(&hi2c1, EEPROM_BASE_ADDR << 1, &dummy, 1, 1) == HAL_OK) {
					break;
				}
			}
		}
		else {
			try_count--;
		}

	}
	return EEPROMFalse;
}
