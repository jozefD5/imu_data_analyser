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

/**
 * @brief I2C device, represents I2C peripheral.
 */
typedef struct {
	I2C_HandleTypeDef *i2c;
	uint8_t address;
	uint32_t timeout;
}i2c_device_type;

HAL_StatusTypeDef i2c_helper_init(i2c_device_type *device);
HAL_StatusTypeDef i2c_helper_write(i2c_device_type *device, uint8_t *data, uint16_t size);
HAL_StatusTypeDef i2c_helper_read(i2c_device_type *device, uint8_t *data, uint16_t size);
HAL_StatusTypeDef i2c_helper_write_read(i2c_device_type *device,
		                          uint8_t *tx_data,
								  uint16_t tx_size,
								  uint8_t *rx_data,
								  uint16_t rx_size);

#ifdef __cplusplus
}
#endif
#endif /* CORE_LIBRARY_I2C_I2C_HELPER_H_ */
