#include "adc.h"
#include <avr/io.h>

void initADC(void) {
    ADMUX = (1<<REFS0); // Configuramos el voltaje de referencia con 5v interno
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);   // Habilitamos el ADC y configuramos un factor de division 64 (preescaler) 
}

uint16_t readADC(uint8_t channel) {
    // Limpiamos lo bits MUX0 a MUX4 y establecemos el canal deseado
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1<<ADSC);        // Iniciamos la conversion ADC
    while (ADCSRA & (1<<ADSC)); // Esperamos hasta que la conversion esté completa
    return ADC;                 // Retornamos el valor leido del ADC
}
