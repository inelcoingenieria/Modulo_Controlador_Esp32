
/* _____PROJECT INCLUDES_____________________________________________________ */
#include "ModbusInelco.h"
#include "DelayMT.h"
#include "Estructuras.h"
#include "LedOperaciones.h"

// Estructuras estructuras;

 /**
Constructor.

Creates class object; initialize it using ModbusMaster::begin().

@ingroup setup
*/
ModbusInelco::ModbusInelco(void)
{
  _idle = 0;
  _preTransmission = 0;
  _postTransmission = 0;
}

void ModbusInelco::begin(uint8_t slave, Stream &serial)
{
//  txBuffer = (uint16_t*) calloc(ku8MaxBufferSize, sizeof(uint16_t));
  _u8MBSlave = slave;
  _serial = &serial;
  _u8TransmitBufferIndex = 0;
  u16TransmitBufferLength = 0;

}


uint8_t ModbusInelco::available(void)
{
  return _u8ResponseBufferLength - _u8ResponseBufferIndex;
}

/**
Set idle time callback function (cooperative multitasking).

This function gets called in the idle time between transmission of data
and response from slave. Do not call functions that read from the serial
buffer that is used by ModbusMaster. Use of i2c/TWI, 1-Wire, other
serial ports, etc. is permitted within callback function.



/**
Set pre-transmission callback function.

This function gets called just before a Modbus message is sent over serial.
Typical usage of this callback is to enable an RS485 transceiver's
Driver Enable pin, and optionally disable its Receiver Enable pin.

@see ModbusMaster::ModbusMasterTransaction()
@see ModbusMaster::postTransmission()
*/
void ModbusInelco::preTransmission(void (*preTransmission)())
{
  _preTransmission = preTransmission;
}

/**
Set post-transmission callback function.

This function gets called after a Modbus message has finished sending
(i.e. after all data has been physically transmitted onto the serial
bus).

Typical usage of this callback is to enable an RS485 transceiver's
Receiver Enable pin, and disable its Driver Enable pin.

@see ModbusMaster::ModbusMasterTransaction()
@see ModbusMaster::preTransmission()
*/
void ModbusInelco::postTransmission(void (*postTransmission)())
{
  _postTransmission = postTransmission;
}

/**
Clear Modbus response buffer.

@see ModbusMaster::getResponseBuffer(uint8_t u8Index)
@ingroup buffer
*/
void ModbusInelco::clearResponseBuffer()
{
  uint8_t i;
  
  for (i = 0; i < ku8MaxBufferSize; i++)
  {
    _u16ResponseBuffer[i] = 0;
  }
}

/**
Clear Modbus transmit buffer.

@see ModbusInelco::setTransmitBuffer(uint8_t u8Index, uint16_t u16Value)
@ingroup buffer
*/
void ModbusInelco::clearTransmitBuffer()
{
  uint8_t i;
  
  for (i = 0; i < ku8MaxBufferSize; i++)
  {
    _u16TransmitBuffer[i] = 0;
  }
}

// funcion para transmitir los datos por uart
uint8_t ModbusInelco::ModbusStructTx(uint8_t u8MBOrigen,uint8_t u8MBDestino,uint8_t u8MBIdStruct,uint8_t u8MBLenghStruct)
{
  // pinMode(TXRX_uart1, OUTPUT);
  // pinMode(TXRX_uart0, OUTPUT);

  uint8_t u8ModbusADU[256];
  uint8_t u8ModbusADUSize = 0;
  uint8_t i;
  uint16_t u16CRC;
    
  // assemble Modbus Request Application Data Unit
  u8ModbusADU[u8ModbusADUSize++] = u8MBDestino;//Destino
  u8ModbusADU[u8ModbusADUSize++] = u8MBOrigen;//Origen
  u8ModbusADU[u8ModbusADUSize++] = ku8MBReadWriteStruct;
  u8ModbusADU[u8ModbusADUSize++] = u8MBIdStruct;
  u8ModbusADU[u8ModbusADUSize++] = u8MBLenghStruct;
  for (i = 0; i < u8MBLenghStruct; i++)
      {
      u8ModbusADU[u8ModbusADUSize++] = u8ModbusDataStruct[i];
      }
 
  // append CRC
  u16CRC = 0xFFFF;
  for (i = 0; i < u8ModbusADUSize; i++)
  {
    u16CRC = crc16_update(u16CRC, u8ModbusADU[i]);
  }
  u8ModbusADU[u8ModbusADUSize++] = lowByte(u16CRC);
  u8ModbusADU[u8ModbusADUSize++] = highByte(u16CRC);
  u8ModbusADU[u8ModbusADUSize] = 0;

  // flush receive buffer before transmitting request
  while (_serial->read() != -1);

  // transmit request
  if (_preTransmission)
  {
    _preTransmission();
  }
  for (i = 0; i < u8ModbusADUSize; i++)
  {
    _serial->write(u8ModbusADU[i]);
    Serial.printf("Valor en hexadecimal: 0x%X\n", u8ModbusADU[i]);
    // Serial.print((HEX)u8ModbusADU[i]);
  }
  
  u8ModbusADUSize = 0;
  _serial->flush();    // flush transmit buffer
  if (_postTransmission)
  {
    _postTransmission();
  }
  Serial.println(" ");
  _u8TransmitBufferIndex = 0;
  u16TransmitBufferLength = 0;
  _u8ResponseBufferIndex = 0;
  return 1;
}

// funcion para cargar los datos a transmitir por ModbusStructTx
void ModbusInelco::txControlador(int _IdControl,int _stSalidas1,int _stSalidas2,int _stZonaAlarma,int _stZonaFalla){
 uControlSDEE myuControlSDEE;
 myuControlSDEE.controlSD.stSalida1=_stSalidas1;
 myuControlSDEE.controlSD.stSalida1=_stSalidas2;
 for(int n=0;n<8;n++){
 myuControlSDEE.controlSD.zona[n].stAlarma=bitRead(_stZonaAlarma,n);
 myuControlSDEE.controlSD.zona[n].stFalla=bitRead(_stZonaFalla,n);
 }

 for( int j=0;j<controlSDEE_size;j++)
     {
     u8ModbusDataStruct[j]=myuControlSDEE.byteArray[j];
     Serial.print(int(myuControlSDEE.byteArray[j]));
     } 
 ModbusStructTx(_IdControl,0x08,ku8MBIdStructControlSD,controlSDEE_size);     
}

DelayMT TimeModbusStruc(50000);
// funcion para resetear el controldor
void ModbusInelco::resetStructRx()
 {
  HayDato=false;
  TimeModbusStruc.resetDelayMT();
  u8ModbusStructSize=0;
  u8MBStatusStructRx=0;

 }

 //Funcion para leer el Controlador
void ModbusInelco::rxModbusControlador(){
    uint8_t statusStructRx;
//  Apago_Led(TXRX_uart1);
//  Apago_Led(TXRX_uart0);
 statusStructRx=analizoCRC();
 if((statusStructRx==ku8MBInvalidCRC)||(statusStructRx==ku8MBResponseTimedOut))
   {
   if(statusStructRx==ku8MBInvalidCRC) 
    Serial.println("ku8MBInvalid"); 
  //  delay(20000);
   if(statusStructRx==ku8MBResponseTimedOut)
      Serial.println("ku8MBResponseTimedOut");  
   resetStructRx();
    // setIdEmul(0);
    // tipoRecepcion(1,0);
   }

  if(statusStructRx==ku8MBStructSuccessFull)
    {
    Serial.println("ku8MBStructSuccessFull"); 
    delay(2000);
    analizoDato();
    uint8_t idEquipo=u8MBIdOrigen;
    uint8_t idStruct=u8MBIdStructRx;
    switch (u8MBIdStructRx){
      case ku8MBIdStructControlSD:
          
          Serial.println("Entro para ver que hago con mis datos");
          delay(2000);
  
      break;
  
      
      default: 
        // if nothing else matches, do the default
        // default is optional 
      break;
     }
    resetStructRx();  
    // setIdEmul(0);
    // tipoRecepcion(1,0);
    }
}
// funcion para analizar los datos recibidos
void ModbusInelco::analizoDato(){

 
  u8ModbusStructSize=0;

          if ((buffer[u8ModbusStructSize] == 0xBA)) //modulo.ku8MBIdControlador 
              
              {
              
                u8ModbusStructSize=1;
                
                if(buffer[u8ModbusStructSize]==ku8MBIdCPU )
                        {

                        u8MBIdOrigen=buffer[1];  
                        u8MBIdStructRx=buffer[3];
                        u8MBLenghStruct=buffer[4]; 
                        u8ModbusStructSize=4;  
                       
                        }
                        else
                        {
                        resetStructRx(); 
                        }
              
              }
              else {

                  u8ModbusStructSize=1;
                  if(buffer[u8ModbusStructSize]==ku8MBIdCPU)
                    {
                      Serial.println("VOY A PASAR LOS DATOS AL SIGUIENTE MODULO");
                      txControlador(0xBA,0,1,0,1);
                    }
                  else
                    {
                      resetStructRx(); 
                    }
              }


}
// funcion para guardar los datos recididos por el driver 2(A0 B0) en un buffer la misma funciona por interrupcion.
 void ModbusInelco::capturoDatos() {
  
  
  u8ModbusStructSize = 0;
 
  while (true) {
    if (Serial0.available()) {
      HayDato=true;
      uint8_t dato = Serial0.read(); 
      buffer[u8ModbusStructSize] = dato;
      u8ModbusStructSize++;

       if (u8ModbusStructSize == 5) {
       
          longitudBuffer = buffer[4];
          Serial.println(" ");
          Serial.print("cantidad de longitud del buffers: ");Serial.println(dato);
          u8TotalData=5+longitudBuffer+2;

        }

        if (u8ModbusStructSize == u8TotalData) {

          // Imprimir todo el buffer en una sola línea
          Serial.print("MIS DATOS SON:  ");
          for (int i = 0; i < u8ModbusStructSize; i++) {
            Serial.print(buffer[i], HEX);
          }
          
          Serial.println(); // Nueva línea después de imprimir el buffer   
          Serial.println(" ");    
         // ...
          break; // Salir del bucle
        }
    }
  }
}
// realiza un chequeo del CRC y devuelve un estado para luego ser utilizado con alguna funcion
uint8_t ModbusInelco::analizoCRC(){


  uint8_t i;
  uint16_t u16CRC;

           if (!HayDato)
            {
              return 0;
            } 

        
                HayDato=false;
                uart0_flag = false;
                u16CRC = 0xFFFF;
                for (i = 0; i < (u8ModbusStructSize - 2); i++)
                {
                  u16CRC = crc16_update(u16CRC,buffer[i]);
                  if(i>4)
                    {
                    u8ModbusDataStruct[i-5]=buffer[i];
                    }

                }
        
                // verify CRC
                if (lowByte(u16CRC) != buffer[u8ModbusStructSize - 2] || highByte(u16CRC) != buffer[u8ModbusStructSize - 1])
                      {
                          u8MBStatusStructRx= ku8MBInvalidCRC;
                          return u8MBStatusStructRx;
                      }
                else
                      {
                          u8MBStatusStructRx= ku8MBStructSuccessFull; 
                          return u8MBStatusStructRx;

                      }
        
}

void ModbusInelco::procesarEstado() {
  
  switch (estadoActual) {
    case ESTADO_0:
      // Esperando recibir datos por UART1
      Serial.println("Esperando Leer datos por UART1----ESTADO 0------");
      estadoActual=ESTADO_1;
      // hayDatosUart0();
      // return;
      break;
    case ESTADO_1:
      // Esperando recibir datos por UART1
      Serial.println("Esperando recibir datos por UART1-------ESTADO 1-------");
      // recibirDatosUART0();
      estadoActual=ESTADO_2;
      break;
    case ESTADO_2:
      // Esperando recibir datos por UART1
      Serial.println("Capturando datos por UART2-------ESTADO 2------");
      // capturoDatos(buffer, longitudBuffer);
      estadoActual=ESTADO_8;
      break;
    case ESTADO_8:
      // Error de timeout
      contador = 0;
      Serial.println("Error de timeout------ESTADO 8--------");
      // resetStructRx();
      estadoActual=ESTADO_9;
      break;
    case ESTADO_9:
      // No hay datos disponibles
      contador = 0;
      Serial.println("No hay datos disponibles------ESTADO 9------");
      // delay(200);
      // resetStructRx();
      estadoActual=ESTADO_0;
      break;
    default:
      // Estado desconocido
      break;
  }
}