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

// Define task/thread.
extern osThreadId_t dataProcessingTaskHandle;
extern const osThreadAttr_t dataProcessingTaskAttributes;



/**
 * @brief Data processing task.
 */
void dataProcessing(void *arg);



#endif /* IMU_DATA_PROCESSING_TH_DATA_PROCESSING_TH_H_ */
