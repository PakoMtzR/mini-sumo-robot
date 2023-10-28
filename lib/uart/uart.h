#ifdef UART_H
#define UART_H

void uart_init(unsigned int ubrr);
void uart_transmit(unsigned char data);
void uart_print(char* msg);
void uart_println(char* msg);
unsigned char uart_recive(void);

#endif