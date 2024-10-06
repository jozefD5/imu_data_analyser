/*
 * data_processing_th.h
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */
#include "main.h"
#include "cmsis_os.h"

#ifndef IMU_DATA_PROCESSING_TH_DATA_PROCESSING_TH_H_
#define IMU_DATA_PROCESSING_TH_DATA_PROCESSING_TH_H_

#ifdef __cplusplus
extern "C" {
#endif

// Define task/thread.
extern osThreadId_t dataProcessingTaskHandle;
extern const osThreadAttr_t dataProcessingTaskAttributes;


void data_processing_init(I2C_HandleTypeDef *i2c);
void data_bmi_isr(void);
void data_processing(void *arg);



#ifdef __cplusplus
}
#endif
#endif /* IMU_DATA_PROCESSING_TH_DATA_PROCESSING_TH_H_ */
