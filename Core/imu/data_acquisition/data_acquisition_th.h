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

void data_bmi_isr(void);
HAL_StatusTypeDef data_aquisition_int(I2C_HandleTypeDef *i2c);
void data_aquisition(void *arg);
HAL_StatusTypeDef data_aquisition_get_data(void);

#ifdef __cplusplus
}
#endif
#endif /* IMU_DATA_ACQU_TH_ */