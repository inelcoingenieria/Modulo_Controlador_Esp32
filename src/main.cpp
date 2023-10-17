#include <Arduino.h>
#include "LeerZonasAdc.h"
#include "LedOperaciones.h"
#include "DelayMT.h"
#include "HardwareSerial.h"
#include "ModbusInelco.h"
const int MAX485_DE_uart1   =  42; //14   (42-TxRx-uart0) = A1 B1
const int MAX485_DE_uart0   =  2;  //14   (2-TxRx-uart1) = A0 B0

// creo objeto para modbus
ModbusInelco modbusCpu;

// Arreglo de objetos "Zona"
LeerZonasAdc zonas[NUM_ZONAS];

// // Tiempo en milisegundos antes de reiniciar la ESP32
// #define WATCHDOG_TIMEOUT 22000
// hw_timer_t *watchdogTimer = NULL;


void cpuPreTransmission_uart1()
  {
  delay(1);  
  digitalWrite(MAX485_DE_uart1, 1);
  }

void cpuPostTransmission_uart1()
  {
  delay(1);
  digitalWrite(MAX485_DE_uart1, 0);
  }

  

void  cpuPreTransmission_uart0()
  {
  delay(1);  
  digitalWrite(MAX485_DE_uart0, 1);
  }

void cpuPostTransmission_uart0()
  {
  delay(1);
  digitalWrite(MAX485_DE_uart0, 0);
  }

void setup_ModuloControlador()
{
 
pinMode(MAX485_DE_uart0, OUTPUT);
pinMode(MAX485_DE_uart1, OUTPUT);
// Init in receive mode
digitalWrite(MAX485_DE_uart0, 0);
digitalWrite(MAX485_DE_uart1, 0);
// Modbus slave ID 1
modbusCpu.begin(1, Serial1);
// Serial0.begin(9600, SERIAL_8N1, 44,43); // Inicializa la UART10con baud rate de 9600 y pines GPIO16 (Rx) y GPIO17 (Tx) = A0 B0
// Callbacks allow us to configure the RS485 transceiver correctly
Serial1.begin(9600, SERIAL_8N1, 18,17); // Inicializa la UART1 con baud rate de 9600 y pines GPIO16 (Rx) y GPIO17 (Tx) = A1 B1
modbusCpu.preTransmission(cpuPreTransmission_uart1);
modbusCpu.postTransmission(cpuPostTransmission_uart1);
modbusCpu.resetStructRx();
}

void leer_Zonas(){
// StZonaSD Mi_zona;
int Mi_StFalla;
int Mi_StAlarma;

for (int i = 0; i < NUM_ZONAS; i++) {
  zonas[i].leerADC();
  String mi_estado = zonas[i].obtenerEstado();

    Serial.print("Zona ");
    Serial.print(i + 1);
    Serial.print(" ADC num.: ");
    Serial.print(pinesADC[i]);
    Serial.print(" ");


  if (mi_estado == "Falla") {
    // Mi_zona.stFalla = true;
    // Mi_StFalla= Mi_zona.stFalla;
    Serial.println(" Falla: ");
    // Serial.println(Mi_zona.stFalla);
  } else if (mi_estado == "Alarma") {
    // Mi_zona.stAlarma = true;
    // Mi_StAlarma= Mi_zona.stAlarma;
    Serial.println(" Alarma: ");
    // Serial.println(Mi_zona.stAlarma);
  } else {
    Serial.println(" Normal");
    // Si el estado es "Normal"
    // No se realiza ninguna acción, ya que los bits ya están en 0 por defecto.
  }
}
}

void IRAM_ATTR llamoInterrupcion(){

  digitalWrite(Led_2,HIGH);
  modbusCpu.capturoDatos();
  digitalWrite(Led_2,LOW);
 
}

bool StInicio=false;

void Inicio(){

  if (StInicio)
    return;
StInicio=true;
delay(6000);
Serial.println("Inicio Sistema");
delay(3000);
// setup_ModuloControlador();
ConfiguroLed();

{  // Crear objetos "Zona" automáticamente
  for (int i = 0; i < NUM_ZONAS; i++) {
    zonas[i] = LeerZonasAdc(pinesADC[i]);
  }
}

// {  // Pongo los dos drivers en modo lectura
//   digitalWrite(TXRX_uart1, LOW);//LOW =lee---HIGH =escribe uart1
//   digitalWrite(TXRX_uart0, LOW);//LOW =lee---HIGH =escribe uart0
// }



}


void setup() {
  // Commit

 {// Configuracion de Uart
  
  Serial.begin(115200);
  Serial0.begin(9600, SERIAL_8N1, 44,43); // Inicializa la UART0 con baud rate de 9600 y pines GPIO16 (Rx) y GPIO17 (Tx) = A0 B0
  Serial1.begin(9600, SERIAL_8N1, 18,17); // Inicializa la UART1 con baud rate de 9600 y pines GPIO16 (Rx) y GPIO17 (Tx) = A1 B1
  // Serial1.setPins(18,17,11,12);
  // Serial0.setPins(44,43,11,12);
  //Serial0.setHwFlowCtrlMode(HW_FLOWCTRL_DISABLE);
  // Serial.S
  
  // Serial0.onReceive(llamoInterrupcion);
  }


  { // Inicializar pines ADC
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
 }

  { // Configuración del watchdog
  // watchdogTimer = timerBegin(0, 80, true);  // Timer 0, prescaler 80 (1MHz)
  // timerAttachInterrupt(watchdogTimer, [](){ESP.restart();}, true);  // Reiniciar la ESP32 cuando se agote el tiempo del watchdog
  // timerAlarmWrite(watchdogTimer, WATCHDOG_TIMEOUT * 1000, false);  // Convertir a microsegundos
  // timerAlarmEnable(watchdogTimer);
 }


{  // Pongo los dos drivers en modo lectura
  // digitalWrite(TXRX_uart1, LOW);//LOW =lee---HIGH =escribe uart1
  // digitalWrite(TXRX_uart0, LOW);//LOW =lee---HIGH =escribe uart0
}
 
 
pinMode(MAX485_DE_uart0, OUTPUT);
pinMode(MAX485_DE_uart1, OUTPUT);
// Init in receive mode
digitalWrite(MAX485_DE_uart0, 0);
digitalWrite(MAX485_DE_uart1, 0);

} 



void loop() {
  // put your main code here, to run repeatedly:
Inicio();
Prendo_Led(Led_1);

// // reinicio watchdogTimer
// timerWrite(watchdogTimer, 0);

// modbusCpu.rxModbusControlador();
// delay(2000);
// modbusCpu.txControlador(0xBA,1,0,1,0);
// delay(8000);
leer_Zonas();
delay(1500);


}

