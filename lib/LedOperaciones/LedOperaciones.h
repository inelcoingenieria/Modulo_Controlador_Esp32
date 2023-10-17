#ifndef __LEDOPERACIONES_H__
#define __LEDOPERACIONES_H__

#include <Arduino.h>

const byte Led_1    = 35;
const byte Led_2    = 45;
const byte Led_3    = 48;
const byte Led_4    = 47;

 

void NumeroDestelloLed(int8_t NombreLed, int8_t Cantidad, int16_t TiempoEncendido, int16_t tiempoApagado);
void ConfiguroLed();
void Prendo_Led(int8_t NombreLed);
void Apago_Led(int8_t NombreLed);
#endif // __LEDOPERACIONES_H__

