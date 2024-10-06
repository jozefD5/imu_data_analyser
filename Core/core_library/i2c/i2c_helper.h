/*
 * i2c_helper.h
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */

#ifndef CORE_LIBRARY_I2C_I2C_HELPER_H_
#define CORE_LIBRARY_I2C_I2C_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

HAL_StatusTypeDef i2c_helper_init(I2C_HandleTypeDef *i2c);
HAL_StatusTypeDef i2c_helper_write(uint16_t device_address, uint8_t *data, uint16_t size);
HAL_StatusTypeDef i2c_helper_read(uint16_t device_address, uint8_t *data, uint16_t size);
HAL_StatusTypeDef i2c_helper_write_read(uint16_t device_address,
		                          uint8_t *tx_data,
								  uint16_t tx_size,
								  uint8_t *rx_data,
								  uint16_t rx_size);

#ifdef __cplusplus
}
#endif
#endif /* CORE_LIBRARY_I2C_I2C_HELPER_H_ */
