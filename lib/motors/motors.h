#ifdef MOTORS_H
#define MOTORS_H

void motors_init(void);
void controlMotorA(uint16_t speed, uint8_t direction);
void controlMotorB(uint16_t speed, uint8_t direction);

#endif
