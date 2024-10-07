# IMU Data Processing
Project handels reading of row data from BMI270 followed by processing stage to produce accurate position measurment data. Data acces via abstraction is handeld vie modules and drives located in `Core/core_library/`. Threads and project specific code is located in `Core/imu/`. `imu` contains 2 threads, data aquasition thread which is responsible for reading sensor data. And, data processing thread responsible for further data processing. 

### Dependency
MCU: F072RB [Nucleo Dev Board](https://www.st.com/en/evaluation-tools/nucleo-f072rb.html)     
RTOS: Free RTOS    
Sensor: [BMI270](https://download.mikroe.com/documents/datasheets/bst-bmi270-ds000-2_datasheet.pdf)    
Sensor API: [BMI270](https://github.com/boschsensortec/BMI270_SensorAPI/tree/master) 
IDE: [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubeide.html)

## Setup
```
// Clone this repository.
git clone {URL}

// Initiate sub-module
git submodule init
git submodule update
```




