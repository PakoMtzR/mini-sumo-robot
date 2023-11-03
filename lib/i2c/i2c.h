#ifdef I2C_H
#define I2C_H

void I2C_Init(void);
void I2C_Start(void);
void I2C_RepeatedStart(void);
void I2C_Stop(void);
void I2C_Write(uint8_t data);
uint8_t I2C_ReadAck(void);
uint8_t I2C_ReadNack(void);
void I2C_InitiateTransmission(uint8_t address, uint8_t writeFlag);
void MPU6050_Init(void);
int16_t ReadMPU6050Data(uint8_t registerAddress);

#endif
