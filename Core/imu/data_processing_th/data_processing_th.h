/*
 * data_processing_th.h
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */
#include "main.h"
#include "cmsis_os.h"
#include "imu/data_acquisition/data_acquisition_th.h"

#ifndef IMU_DATA_PROCESSING_TH_
#define IMU_DATA_PROCESSING_TH_

#ifdef __cplusplus
extern "C" {
#endif

// Define task/thread.
extern osThreadId_t dataProcessingTaskHandle;
extern const osThreadAttr_t dataProcessingTaskAttributes;

void data_processing_init(void);
void data_processing(void *arg);
HAL_StatusTypeDef data_processing_get_data(acc_gyr_data_type *data);


#ifdef __cplusplus
}
#endif
#endif /* IMU_DATA_PROCESSING_TH_ */
