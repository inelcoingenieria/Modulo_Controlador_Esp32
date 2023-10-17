#include "LedOperaciones.h"
#include "DelayMT.h"
#include <Arduino.h>



void ConfiguroLed() {
pinMode(Led_1, OUTPUT);
pinMode(Led_2, OUTPUT);
pinMode(Led_3, OUTPUT);
pinMode(Led_4, OUTPUT);
}
void NumeroDestelloLed(int8_t NombreLed, int8_t Cantidad, int16_t TiempoEncendido, int16_t tiempoApagado)
{
    
    
    for (int i = 0; i < Cantidad; i++) {
        digitalWrite(NombreLed, HIGH);
        delay(TiempoEncendido);
        digitalWrite(NombreLed, LOW);
        delay(tiempoApagado);
    }
}

// void NumeroDestelloLed(int8_t NombreLed, int8_t Cantidad, int16_t TiempoEncendido, int16_t tiempoApagado)
// {
//     int i = 0;
//     bool LedEncendido = false;
//     bool LedApagado = false;
//     pinMode(NombreLed, OUTPUT);
//     DelayMT dtimeOn(TiempoEncendido); // Crear una instancia del temporizador con el tiempo de encendido como parámetro
//     DelayMT dtimeOff(tiempoApagado); // Crear una instancia del temporizador con el tiempo de apagado como parámetro
    
//     if (i < Cantidad) {
//         if (!LedEncendido && dtimeOn.StDelayMT()) {
//             digitalWrite(NombreLed, HIGH);
//             LedEncendido = true;
//         }
        
//         if (LedEncendido && !LedApagado && dtimeOff.StDelayMT()) {
//             digitalWrite(NombreLed, LOW);
//             LedEncendido = false;
//             LedApagado = true;
//             i++;
//         }
        
//         if (LedApagado) {
//             LedApagado = false;
//             dtimeOn.resetDelayMT();
//             dtimeOff.resetDelayMT();
//         }
//     }
// }


// void NumeroDestelloLed(int8_t NombreLed, int8_t Cantidad, int16_t TiempoEncendido, int16_t tiempoApagado)
// {
//     int i = 0;
//     bool LedEncendido;
//     bool LedApagado;
//     pinMode(NombreLed, OUTPUT);
//     DelayMT dtimeOn(TiempoEncendido); // Crear una instancia del temporizador con el tiempo de encendido como parámetro
//     DelayMT dtimeOff(tiempoApagado);// Crear una instancia del temporizador con el tiempo de encendido como parámetro

//      if (i == Cantidad && LedEncendido && LedApagado ) {
//         i=0;
//         LedEncendido=false;
//         LedApagado=false;
//         // return;
//     }
//     if (i < Cantidad){
//             if (!LedEncendido){        
//                 digitalWrite(NombreLed, HIGH);
//                 if (dtimeOn.StDelayMT()) {
//                     dtimeOn.resetDelayMT(); // Reiniciar el temporizador antes de encender el LED
//                     LedEncendido=true;
//                 }
//                 // return;
//             }

//             if(!LedApagado){
//                 digitalWrite(NombreLed, LOW);
//                 if (dtimeOff.StDelayMT()) {
//                     dtimeOff.resetDelayMT(); // Reiniciar el temporizador antes de encender el LED
//                     LedApagado=true;   
//                     i++;     
//                     if (i < Cantidad && LedEncendido && LedApagado ) {
//                         i=0;
//                         LedEncendido=false;
//                         LedApagado=false;
//                         // return;
//                     }

//                 }

//                 LedEncendido=false;
//                 LedApagado=false;
//                 // return;
//             }   
    
   
//     }
// }

void Prendo_Led(int8_t NombreLed)
{

 digitalWrite(NombreLed, HIGH);

}

void Apago_Led(int8_t NombreLed)
{

 digitalWrite(NombreLed, LOW);

}
