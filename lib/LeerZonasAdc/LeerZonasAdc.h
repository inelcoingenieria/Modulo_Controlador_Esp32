#ifndef __LEERZONASADC_H__
#define __LEERZONASADC_H__

#include <Arduino.h> 


const int NUM_ZONAS = 8;  // Número de zonas
const int pinesADC[NUM_ZONAS] = {4, 5, 6, 7, 15, 16, 8,10};  // Pines ADC a utilizar   {4, 5, 6, 7, 15, 16, 8,10}

const int Cantidad_Promedio = 3;
float promedioGeneral = 0.0;
bool algunaFalla      = false;
bool algunaAlarma     = false;
bool todasNormales    = true;



// /*
class LeerZonasAdc {
  
private:

  // Arreglo para almacenar las lecturas
  int pin;
  float voltajes[3];  

public:
  
  // Constructor con valor predeterminado para el pin ADC
  LeerZonasAdc(int adcPin = 0) 
  {  
    pin = adcPin;
    for (int i = 0; i < Cantidad_Promedio; i++) {
      voltajes[i] = 0.0;
    }
  }

  void leerADC() {
   
    for (int i = 0; i < Cantidad_Promedio; i++) {
      int rawValue = analogRead(pin);
      voltajes[i] = rawValue * (3.3 / 4095); // Convertir valor crudo a voltaje (resolución ADC de 12 bits)
     
    }

  }
  float obtenerPromedio() {

    float suma = 0.0;
    for (int i = 0; i < Cantidad_Promedio; i++) {
      suma += voltajes[i];
    }
    return suma / Cantidad_Promedio;  // Calcular promedio dividiendo entre Cantidad_Promedio
    
  }
  

  String obtenerEstado() 
  {
    promedioGeneral=obtenerPromedio();
    if (promedioGeneral >= 0.0 && promedioGeneral < 0.9) {
      todasNormales = false;
      algunaAlarma = true;
      return "Alarma";
    } else if (promedioGeneral >= 1 && promedioGeneral < 1.9) {
      algunaAlarma = false;
      algunaFalla = false;
      todasNormales = true;
      return "Normal";
    } else if (promedioGeneral >= 2 && promedioGeneral <= 2.7) {
      todasNormales = false;
      algunaFalla = true;
      return "Falla";
    } else {
      return "Valor fuera de rango";
    }
  }

 };
// */


 #endif // __LEERZONASADC_H__