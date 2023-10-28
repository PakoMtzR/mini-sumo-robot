#include "uart.h"
#include <avr/io.h>

void uart_init(unsigned int ubrr) {
  // Configurar la velocidad de transmisión
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr; 

  UCSR0B = (1 << TXEN0) | (1 << RXEN0);   // Habilitar transmisor y receptor
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Configurar el formato de datos: 8 bits de datos, 1 bit de parada
}

void uart_transmit(unsigned char data) {
  while (!(UCSR0A & (1 << UDRE0)));   // Esperar a que el buffer de transmisión esté vacío
  UDR0 = data;                        // Enviar datos
}

unsigned char uart_recive(void) {
  while (!(UCSR0A & (1 << RXC0)));    // Esperar hasta que lleguen datos
  return UDR0;                        // Retornar los datos recibidos
}

void uart_print(char* msg) {
  for (int i = 0; msg[i] != '\0'; i++) uart_transmit(msg[i]);
}

void uart_println(char* msg) {
  for (int i = 0; msg[i] != '\0'; i++) uart_transmit(msg[i]);
  uart_transmit('\n');
}