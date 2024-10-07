/*
 * data acquisition.h
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */
#include "main.h"
#include "cmsis_os.h"

#ifndef IMU_DATA_ACQU_TH_
#define IMU_DATA_ACQU_TH_

#ifdef __cplusplus
extern "C" {
#endif

// Define task/thread.
extern osThreadId_t dataAquisitionTaskHandle;
extern const osThreadAttr_t dataAquisitionTaskAttributes;

/**
 * @brief Gyroscope and accelerometer readings.
 */
typedef struct {
	float acc_x;
	float acc_y;
	float acc_z;

	float gyr_x;
	float gyr_y;
	float gyr_z;

	float yaw;
	float pitch;
	float roll;


}acc_gyr_data_type;

void data_bmi_isr(void);
HAL_StatusTypeDef data_aquisition_int(I2C_HandleTypeDef *i2c);
void data_aquisition(void *arg);
HAL_StatusTypeDef data_aquisition_get_data(acc_gyr_data_type *data);

#ifdef __cplusplus
}
#endif
#endif /* IMU_DATA_ACQU_TH_ */
