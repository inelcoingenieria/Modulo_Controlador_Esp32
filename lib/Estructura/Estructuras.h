#ifndef __ESTRUCTURAS_H__
#define __ESTRUCTURAS_H__


#include <Arduino.h>
//**************************************************
//Estructuras en Eeprom

 const String version="1.0.0";
//  const int NroControladores = 100;


//  struct StControl
//  {
//   bool stActivo:1;
//   bool stExcluido:1;
//   bool stNuevo:1;
//   bool stModificar:1;
//   byte stTipo:4;
//  };
 
//  struct StControlCpu
//  {
//   StControl stContrl[NroControladores];
//  };
//  const int stControlCpu_size = sizeof(StControlCpu);
//  union uStControlCpu {
//  StControlCpu stControlCpu;
//  byte byteArray[stControlCpu_size];
// };

//******************************************************
 struct StZonaSD {
    bool stActiva:1;
    bool stExcluida:1;
    // bool stRetardo:1;//si no tiene retardo es instantanea
    bool stFalla:1;
    bool stAlarma:1;
    // bool ackAlarma:1;
    // bool ackFalla:1;
    bool reservado:1; 
 };
union uZonaSDEE {
StZonaSD zonaSDEE;
byte byteZona;
};


//*********************************************************
 struct StSalidaSD {
    bool normal:1;
    bool intermitente:1;
    // bool resetVcc:1;//reset Alimentacion o de sensores
    bool reservado:1;
 };
union uSalidaSDEE {
StSalidaSD salidaSDEE;
byte byteSalida;
};

static const uint8_t ku8MBIdStructControlSD                  = 0x01;
//**********************************************************

 struct ControlSD {

  byte stSalida1:4;
  byte stSalida2:4;

  StZonaSD zona[8];
};
const int controlSDEE_size = sizeof(ControlSD);
union uControlSDEE {
ControlSD controlSD;
byte byteArray[controlSDEE_size];
};


//**************************************************

// //**************************************************

// struct ControlZonaSD {
//   int  tiempoUltimoReporte;
//   int  timeoutEncuesta;
//   bool runEncuesta:1;
//   bool falloEncuesta:1;
//   // bool ackFalla:1;
//   bool stFalla:1;
//   bool hayDato:1;
//   ControlSD controlSD;

// };
// const int controlEE_size = sizeof(ControlZonaSD);

// //****************************************************
// static const uint8_t ku8MBIdStructDisplay                  = 0x05;
// //**********************************************************

//  struct  Display{
//   char mensaje[40];
//   byte dato[8];
//   byte idFuncion;
// };
// const int display_size = sizeof(Display);
// union uDisplay {
// Display display;
// byte byteArray[display_size];
// };

// //****************************************************
// static const uint8_t ku8MBIdStructTeclado                  = 0x06;
// //**********************************************************

//  struct  Teclado{
//   byte idFuncion;//panico/disparo
//   byte dato[8];
// };
// const int teclado_size = sizeof(Teclado);
// union uTeclado {
// Teclado teclado;
// byte byteArray[teclado_size];
// };
//****************************************************
static const uint8_t ku8MBIdStructGenirica                  = 0x07;
//**********************************************************

 struct  StructGenerica{
  byte idFuncion;//panico/disparo
  byte dato[4];
};
const int StructGenerica_size = sizeof(StructGenerica);
union uStructGenerica {
StructGenerica structGenerica;
byte byteArray[StructGenerica_size];
};


#endif // __ESTRUCTURAS_H__