#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "uart.h"
#include "adc.h"

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VAL (F_CPU / 16 / BAUD - 1)

// =============== Definimos los pines ===============
// Sensores Sharp GP2Y0A21YK
#define SENSOR_FRONT 0
#define SENSOR_RIGHT 1
#define SENSOR_LEFT 2
// Sensores de linea 
#define SENSOR_LINE_RIGHT 1
#define SENSOR_LINE_LEFT 2
// Driver TB6612FNG
#define STBY 0
#define AIN1 0
#define AIN2 0
#define BIN1 0
#define BIN2 0
#define PWMA 0
#define PWMB 0
// ...

int main() {
  // =============== Declaramos entradas y salidas ===============
  DDRB |= (1<<DDB5);


  // =============== Inicializamos UART y ADC ===============
  uart_init(UBRR_VAL);
  initADC();
  uart_print("Listo!\n");

  // =============== Variables ===============
  uint16_t adcValue;
  //bool objectDetected = false;
  char texto[10];

  // =============== Leemos la estrategia de combate ===============
  // Leer dip switch
  // ...

  // =============== Esperamos a que inicie el combate ===============
  // Esperamos la señar IR
  // ...

  while(1) {
    // rutina de busqueda

    adcValue = readADC(0);
    utoa(adcValue, texto, 10);
    uart_println(texto);
    if ( adcValue < 950 ) PORTB |= (1<<PINB5);
    else PORTB &= ~(1<<PINB5);
    _delay_ms(200);
  }
  return 0;
}