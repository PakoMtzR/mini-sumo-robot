#include "i2c.h"
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#define MPU6050_ADDRESS 0x68  // Dirección I2C del MPU6050
//#define F_CPU 16000000UL
    
void I2C_Init() {
    // Configurar velocidad de reloj I2C a 100 kHz
    TWSR = 0;
    TWBR = ((F_CPU / 100000) - 16) / 2;
}

void I2C_Start() {
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

void I2C_Stop() {
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
}

void I2C_Write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

uint8_t I2C_ReadAck() {
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t I2C_ReadNack() {
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

void MPU6050_Init() {
    I2C_Start();
    I2C_Write(MPU6050_ADDRESS << 1);
    I2C_Write(0x6B); // Registro PWR_MGMT_1
    I2C_Write(0); // Establecer en cero para activar el MPU6050
    I2C_Stop();
}

