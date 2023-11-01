#include "motors.h"
#include <avr/io.h>

// Driver TB6612FNG
#define STBY PC3
#define AIN1 PD4
#define AIN2 PD5
#define BIN1 PD6
#define BIN2 PD7
#define PWMA PB1
#define PWMB PB2

void motors_init(void) {
    // Configuramos pines de salidas para dar direccion a los motores
    DDRD |= (1 << AIN1) | (1 << AIN2) | (1 << BIN1) | (1 << BIN2);
    // Configuramos pines de salida para las señales PWM
    DDRB |= (1 << PWMA) | (1 << PWMB);

    // Configurar el modo de Fast PWM de 16 bits con salida no invertida y sin preescaler (modo 14)
    TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11);   
    TCCR1B |= (1<<WGM12) | (1<<WGM13) | (1<<CS10);
    // Configurar el valor del límite superior (para obtener una frecuencia de 16 kHz)
    // ICR1 = TOP = (Frecuencia del reloj / (Frecuencia deseada * Preescaler)) - 1
    // ( 16MHz / (16KHz * 1)) - 1 = 999 -> 0x03E7
    ICR1 = 999;
    // Configurar el valor de ciclo de trabajo (0-999 para 0-100%)
    OCR1A = 800;  // Ejemplo de un ciclo de trabajo del 50%
    OCR1B = 800;  // Ejemplo de un ciclo de trabajo del 50%

    // Inicializa el puente H en una dirección
    PORTD |= (1 << AIN1) | (1 << BIN1);
    PORTD &= ~(1 << AIN2) | ~(1 << BIN2);
}

// Función para controlar la velocidad y dirección del motor A
void controlMotorA(uint16_t speed, uint8_t direction) {
    OCR1A = speed; // Establece la velocidad del Motor A
    if (direction == 1) {
        PORTD |= (1 << AIN1);
        PORTD &= ~(1 << AIN2);
    } else if (direction == 2) {
        PORTD |= (1 << AIN2);
        PORTD &= ~(1 << AIN1);
    } else {
        PORTD &= ~((1 << AIN1) | (1 << AIN2));
    }
}

// Función para controlar la velocidad y dirección del motor B
void controlMotorB(uint16_t speed, uint8_t direction) {
    OCR1B = speed; // Establece la velocidad del Motor B
    if (direction == 1) {
        PORTD |= (1 << BIN1);
        PORTD &= ~(1 << BIN2);
    } else if (direction == 2) {
        PORTD |= (1 << BIN2);
        PORTD &= ~(1 << BIN1);
    } else {
        PORTD &= ~((1 << BIN1) | (1 << BIN2));
    }
}

