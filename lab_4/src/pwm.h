#ifndef PWM_H
#define PWM_H

void initPWMTimer3();
void initPWMTimer4();

void changeDutyCycle(float dutycycle1, float dutycycle2);

void analyzeADC();

#endif