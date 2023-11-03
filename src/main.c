#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "i2c.h"
#include "adc.h"
#include "motors.h"

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VAL (F_CPU / 16 / BAUD - 1)

#define MPU6050_ADDRESS 0x68 
// =============== Definimos los pines ===============
// Sensores Sharp GP2Y0A21YK
#define SENSOR_FRONT PC0
#define SENSOR_RIGHT PC1
#define SENSOR_LEFT PC2

#define LED PB5
// ...

// =============== Declaracion de Variables ===============
uint16_t adcValue_front, adcValue_right, adcValue_left;
uint8_t strategy;
int16_t accelX, accelY, accelZ;
char text[10];
//bool objectDetected = false;

int main() {
  // ====================== Declaramos entradas y salidas ======================
  DDRB |= (1<<LED);                       // Indicador led del arduino OUTPUT
  DDRB &= ~((1<<PB0)|(1<<PB3)|(1<<PB4));  // Pines para leer el dip-switch INPUT
  DDRD &= ~((1<<PD2)|(1<<PD3));           // Sensores de linea INPUT 

  // ================= Config Interrupciones Sensores de Linea =================
  EICRA |= (1<<ISC01) | (1<<ISC00) | (1<<ISC11) | (1<<ISC10); // Configura ambas interrupciones por flanco de subida
  EIMSK |= (1<<INT0) | (1<<INT1);                             // Habilita las interrupciones INT0 e INT1
  sei();                                                      // Activamos todas las interrupciones
  // =================== Inicializamos UART, ADC y TB6612fng ===================
  uart_init(UBRR_VAL);
  initADC();
  motors_init();
  I2C_Init();
  MPU6050_Init();
  // ===================== Leemos la estrategia de combate =====================
  PORTB |= (1<<LED);      // Activamos el dip-switch para leer la estrategia
  _delay_ms(20);          // Esperamos para asegurar que el dip-switch este alimentado
  strategy = PINB & 0x19; // Leemos todo el puerto D y leemos los pines 11001

  // =============== Esperamos a que inicie el combate ===============
  uart_print("Listo!\n");
  _delay_ms(1000);
  PORTB &= ~(1<<LED);     // Desactivamos el dip-switch para que no consuma energia
  // Esperamos la señal IR ...

  while(1) {
    // rutina de busqueda  
    adcValue_front = readADC(SENSOR_FRONT);
    adcValue_right = readADC(SENSOR_RIGHT);
    adcValue_left = readADC(SENSOR_LEFT);
    utoa(adcValue_front, text, 10);
    uart_println(text);
    I2C_Start();
    I2C_Write(MPU6050_ADDRESS << 1);
    I2C_Write(0x3B); // Comenzar con el registro 0x3B (ACCEL_XOUT_H)
    I2C_Stop();

    I2C_Start();
    I2C_Write((MPU6050_ADDRESS<< 1) | 1);
    uint16_t AcX = (I2C_ReadAck() << 8) | I2C_ReadAck();
    uint16_t AcY = (I2C_ReadAck() << 8) | I2C_ReadAck();
    uint16_t AcZ = (I2C_ReadAck() << 8) | I2C_ReadNack();
    I2C_Stop();
    utoa(AcX, text, 10);
    uart_println(text);
    _delay_ms(200);
  }
  return 0;
}

// =============== Interrupciones Sensores de Linea ===============
// Sensor de linea Izquierdo
ISR(INT0_vect) {
  PORTB |= (1<<LED);
  controlMotorA(-600);
  controlMotorB(-600);
  _delay_ms(200);
  controlMotorA(-600);
  controlMotorB(600);
  _delay_ms(200);
  controlMotorA(800);
  controlMotorB(800);
  PORTB &= ~(1<<LED);
}
// Sensor de linea Derecho
ISR(INT1_vect) {
  PORTB |= (1<<LED);
  controlMotorA(-600);
  controlMotorB(-600);
  _delay_ms(200);
  controlMotorA(600);
  controlMotorB(-600);
  _delay_ms(200);
  controlMotorA(800);
  controlMotorB(800);
  PORTB &= ~(1<<LED);
}