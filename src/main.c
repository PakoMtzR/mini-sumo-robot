#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "adc.h"
#include "motors.h"

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VAL (F_CPU / 16 / BAUD - 1)

// =============== Definimos los pines ===============
// Sensores Sharp GP2Y0A21YK
#define SENSOR_FRONT PC0
#define SENSOR_RIGHT PC1
#define SENSOR_LEFT PC2

#define LED PB5
// ...

// =============== Declaracion de Variables ===============
uint16_t adcValue;
uint8_t strategy;
char texto[10];
//bool objectDetected = false;

int main() {
  // =============== Declaramos entradas y salidas ===============
  DDRB |= (1<<LED);                           // Indicador led del arduino OUTPUT
  DDRD &= ~(1<<PD2) | ~(1<<PD3);              // Sensores de linea INPUT 
  //DDRD |= (1<<PD7);                           // Activa el dip-switch OUTPUT
  //DDRD &= ~(1<<PD4) | ~(1<<PD5) | ~(1<<PD6);  // Dip-switch INPUT

  // =============== Config Interrupciones Sensores de Linea ===============
  EICRA |= (1<<ISC01) | (1<<ISC00) | (1<<ISC11) | (1<<ISC10); // Configura ambas interrupciones por flanco de subida
  EIMSK |= (1<<INT0) | (1<<INT1);                             // Habilita las interrupciones INT0 e INT1
  sei();                                                      // Activamos todas las interrupciones
  // =============== Inicializamos UART, ADC y TB6612fng ===============
  uart_init(UBRR_VAL);
  initADC();
  motors_init();
  // =============== Leemos la estrategia de combate ===============
  // Leer dip switch
  //PORTD |= (1<<PD7);  // Activamos el dip-switch para leer la estrategia
  //_delay_ms(20);
  //strategy = PIND;    // Leemos todo el puerto D
  //PORTD &= ~(1<<PD7); // Desactivamos el dip-switch para que no consuma energia
  // ...

  // =============== Esperamos a que inicie el combate ===============
  // Esperamos la señar IR
  // uart_print("Listo!\n");
  PORTB |= (1<<LED);  // Indicador de que todo esta listo
  _delay_ms(1000);
  PORTB &= ~(1<<LED);
  // ...
  
  while(1) {
    // rutina de busqueda  
    // ...
    adcValue = readADC(SENSOR_FRONT);
    utoa(adcValue, texto, 10);
    uart_println(texto);
    _delay_ms(200);
  }
  return 0;
}

// =============== Interrupciones Sensores de Linea ===============
ISR(INT0_vect) {
  PORTB |= (1<<LED);
  _delay_ms(200);
  PORTB &= ~(1<<LED);
}

ISR(INT1_vect) {
  PORTB |= (1<<LED);
  _delay_ms(200);
  PORTB &= ~(1<<LED);
}