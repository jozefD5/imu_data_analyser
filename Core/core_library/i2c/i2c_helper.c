/*
 * i2c_helper.c
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */
#include <core_library/debug_log/debug_log.h>
#include <core_library/i2c/i2c_helper.h>

#define I2C_HELPER_TIMEOUT    100U

// I2C driver.
static I2C_HandleTypeDef *i2c_driver;

/**
 * @brief Initialise I2C communication driver.
 * @param i2c    I2C driver.
 */
HAL_StatusTypeDef i2c_helper_init(I2C_HandleTypeDef *i2c) {
	HAL_StatusTypeDef res = HAL_OK;

	if(i2c == NULL) {
		return HAL_ERROR;
	}

	return res;
}

/**
 * @brief Send data to target device.
 * @param device_address    target device I2C address.
 * @param data              transmit buffer, data to be transmitted.
 * @param size              size of data buffer.
 */
HAL_StatusTypeDef i2c_helper_write(uint16_t device_address, uint8_t *data, uint16_t size) {
	HAL_StatusTypeDef res = HAL_OK;

	res = HAL_I2C_Master_Transmit(i2c_driver, device_address, data, size, I2C_HELPER_TIMEOUT);

	return res;
}

/**
 * @brief Receive data from target device
 * @param device_address    target device I2C address.
 * @param data              receive buffer, data to be received.
 * @param size              size of data buffer.
 */
HAL_StatusTypeDef i2c_helper_read(uint16_t device_address, uint8_t *data, uint16_t size) {
	HAL_StatusTypeDef res = HAL_OK;

	res = HAL_I2C_Master_Receive(i2c_driver, device_address, data, size, I2C_HELPER_TIMEOUT);

	return res;
}

/**
 * @brief Transmit and then receive data from target device
 * @param device_address    target device I2C address.
 * @param tx_data           transmit buffer, data to be transmitted.
 * @param tx_size           size of data buffer.
 * @param rx_data           received buffer, data to be received.
 * @param rx_size           size of receive data buffer.
 */
HAL_StatusTypeDef i2c_helper_write_read(uint16_t device_address,
		                          uint8_t *tx_data,
								  uint16_t tx_size,
								  uint8_t *rx_data,
								  uint16_t rx_size) {
	HAL_StatusTypeDef res = HAL_OK;

	res = HAL_I2C_Master_Transmit(i2c_driver, device_address, tx_data, tx_size, I2C_HELPER_TIMEOUT);
	if(res != HAL_OK) {
		return res;
	}

	res = HAL_I2C_Master_Receive(i2c_driver, device_address, rx_data, rx_size, I2C_HELPER_TIMEOUT);

	return res;
}

