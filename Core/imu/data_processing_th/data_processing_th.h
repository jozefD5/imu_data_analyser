/*
 * data_processing_th.h
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */
#include "main.h"
#include "cmsis_os.h"

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

#ifdef __cplusplus
}
#endif
#endif /* IMU_DATA_PROCESSING_TH_ */
