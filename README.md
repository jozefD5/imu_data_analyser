# IMU Data Processing
Project handels reading of row data from BMI270 followed by processing stage to produce accurate position measurment data. Data acces via abstraction is handeld vie modules and drives located in `Core/core_lib/`. Threads and project specific code is located in `Core/imu/`. 

### Dependency
MCU: F072RB [Nucleo Dev Board](https://www.st.com/en/evaluation-tools/nucleo-f072rb.html)     
RTOS: Free RTOS    
Sensor: [BMI270](https://download.mikroe.com/documents/datasheets/bst-bmi270-ds000-2_datasheet.pdf)    
Sensor API: [BMI270](https://github.com/boschsensortec/BMI270_SensorAPI/tree/master) 

## Setup
```
// Clone this repository.
git clone {URL}

// Initiate sub-module
git submodule init
git submodule update
```




