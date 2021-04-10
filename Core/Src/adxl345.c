#include "adxl345.h"

//Buffer to perform operation
uint8_t _buff[6];
//Status variable
HAL_StatusTypeDef _status;


void setTargetDeviceAddress(I2C_HandleTypeDef *hi2c, uint16_t address){
    hi2c->Devaddress = address;
}

HAL_StatusTypeDef AccelInit(I2C_HandleTypeDef *hi2c){
    HAL_StatusTypeDef status;
	status = AccelWrite (hi2c, 0x2d, 0x00);  // reset all bits
    if(status != HAL_OK){
        return status;
    }
	status = AccelWrite (hi2c, 0x2d, 0x08);  // power_cntl measure and wake up 8hz*/
    if(status != HAL_OK){
        return status;
    }
	AccelWrite (hi2c, 0x31, 0x00);  // data_format range= +- 2g
	//AccelWrite (0x2c, 0x0d);  // 800 hz output data rate
    return status;
}

HAL_StatusTypeDef getAccelerations(I2C_HandleTypeDef *hi2c, Accelerations *accelerations) {
    _status = AccelReadData(hi2c, _buff, 0x32, 6);
    if(_status == HAL_OK){
        accelerations->x = ((int16_t)_buff[1] << 8) + (int16_t)_buff[0];
        accelerations->y = ((int16_t)_buff[3] << 8) + (int16_t)_buff[2];
        accelerations->z = ((int16_t)_buff[5] << 8) + (int16_t)_buff[4];
    }
    return _status;
}

HAL_StatusTypeDef AccelWrite(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    return HAL_I2C_Master_Transmit(hi2c, hi2c->Devaddress, data, 2, 100);
}

HAL_StatusTypeDef AccelReadData(I2C_HandleTypeDef *hi2c, uint8_t *buffer, uint8_t reg, int nb_byte) {
    return HAL_I2C_Mem_Read(hi2c, hi2c->Devaddress, reg, 1, buffer, nb_byte, 100);
}

HAL_StatusTypeDef AccelGetId(I2C_HandleTypeDef *hi2c, int *id){
    return AccelReadData(hi2c, (uint8_t *)id, 0x00, 1);
}