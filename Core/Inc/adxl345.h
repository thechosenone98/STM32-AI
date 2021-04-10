#if !defined(ADXL345)
#define ADXL345

#include "stm32f4xx_hal.h"
#include "i2c.h"

typedef struct Accelerations{
    int16_t x;
    int16_t y;
    int16_t z;
}Accelerations;

//Buffer to perform operation
extern uint8_t _buff[6];
//Status variable
extern HAL_StatusTypeDef _status;

HAL_StatusTypeDef AccelInit(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef getAccelerations(I2C_HandleTypeDef *hi2c, Accelerations *accelerations);
HAL_StatusTypeDef AccelWrite(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t value);
HAL_StatusTypeDef AccelReadData(I2C_HandleTypeDef *hi2c, uint8_t *buffer, uint8_t reg, int nb_byte);
HAL_StatusTypeDef AccelGetId(I2C_HandleTypeDef *hi2c, int *id);
void setTargetDeviceAddress(I2C_HandleTypeDef *hi2c, uint16_t address);


#endif // ADXL345