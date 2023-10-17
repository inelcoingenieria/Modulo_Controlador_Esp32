#ifndef __MODBUSINELCO_H__
#define __MODBUSINELCO_H__

/* _____STANDARD INCLUDES____________________________________________________ */
// include types & constants of Wiring core API
#include "Arduino.h"

/* _____PROJECT INCLUDES_____________________________________________________ */
// functions to calculate Modbus Application Data Unit CRC
#include "util/crc16.h"

// functions to manipulate words
#include "util/word.h"


#define BUFFER_SIZE 100 // Tamaño máximo del buffer
const byte TXRX_uart1       = 42;
const byte TXRX_uart0       = 2;


class ModbusInelco
{
  public:
          ModbusInelco();
        
          void begin(uint8_t, Stream &serial);

          void preTransmission(void (*)());
          void postTransmission(void (*)());

          // Modbus exception codes
          /**
          Modbus protocol illegal function exception.
          
          The function code received in the query is not an allowable action for
          the server (or slave). This may be because the function code is only
          applicable to newer devices, and was not implemented in the unit
          selected. It could also indicate that the server (or slave) is in the
          wrong state to process a request of this type, for example because it is
          unconfigured and is being asked to return register values.
          
          @ingroup constant
          */
          static const uint8_t ku8MBIllegalFunction            = 0x01;

          /**
          Modbus protocol illegal data address exception.
          
          The data address received in the query is not an allowable address for 
          the server (or slave). More specifically, the combination of reference 
          number and transfer length is invalid. For a controller with 100 
          registers, the ADU addresses the first register as 0, and the last one 
          as 99. If a request is submitted with a starting register address of 96 
          and a quantity of registers of 4, then this request will successfully 
          operate (address-wise at least) on registers 96, 97, 98, 99. If a 
          request is submitted with a starting register address of 96 and a 
          quantity of registers of 5, then this request will fail with Exception 
          Code 0x02 "Illegal Data Address" since it attempts to operate on 
          registers 96, 97, 98, 99 and 100, and there is no register with address 
          100. 
          
          @ingroup constant
          */
          static const uint8_t ku8MBIllegalDataAddress         = 0x02;
          
          /**
          Modbus protocol illegal data value exception.
          
          A value contained in the query data field is not an allowable value for 
          server (or slave). This indicates a fault in the structure of the 
          remainder of a complex request, such as that the implied length is 
          incorrect. It specifically does NOT mean that a data item submitted for 
          storage in a register has a value outside the expectation of the 
          application program, since the MODBUS protocol is unaware of the 
          significance of any particular value of any particular register.
          
          @ingroup constant
          */
          static const uint8_t ku8MBIllegalDataValue           = 0x03;
          
        
          // Class-defined success/exception codes
          /**
          ModbusMaster success.
          
          Modbus transaction was successful; the following checks were valid:
            - slave ID
            - function code
            - response code
            - data
            - CRC
            
          @ingroup constant
          */
          static const uint8_t ku8MBSuccess                    = 0x00;
          
          
          /**
          ModbusMaster invalid response function exception.
          
          The function code in the response does not match that of the request.
          
          @ingroup constant
          */
          static const uint8_t ku8MBInvalidFunction            = 0xE1;
          
          /**
          ModbusMaster response timed out exception.
          
          The entire response was not received within the timeout period, 
          ModbusMaster::ku8MBResponseTimeout. 
          
          @ingroup constant
          */
          static const uint8_t ku8MBResponseTimedOut           = 0xE2;
          
          /**
          ModbusMaster invalid response CRC exception.
          
          The CRC in the response does not match the one calculated.
          
          @ingroup constant
          */
          static const uint8_t ku8MBInvalidCRC                 = 0xE3;
          
          //constates Struct

          static const uint8_t ku8MBStructSuccessFull          = 0xA0; 

          // Modbus function codes for 8 bit access Struct
          static const uint8_t ku8MBReadWriteStruct             = 0x1A; ///< Modbus function 0x0A Read and Write Struct
          
            // Modbus timeout [milliseconds]
          static const uint16_t ku16MBResponseTimeout          = 2000; ///< Modbus timeout [milliseconds]

          static const uint8_t ku8MBIdCPU                 = 0xC9;
          
   
          static const uint8_t ku8MBIdAlarmaZona          = 0x01;
          static const uint8_t ku8MBIdAckAlarmaZona       = 0x02;
          static const uint8_t ku8MBIdFallaZona           = 0x03;
          static const uint8_t ku8MBIdAckFallaZona        = 0x04;
      
          static const uint8_t ku8MBIdDisparoSirena       = 0x07;
          static const uint8_t ku8MBIdResetAlimentacion   = 0x08;
          static const uint8_t ku8MBIdDatoOk              = 0x09;
          static const uint8_t ku8MBIdEvacuacion          = 0x0A;

     
          void clearResponseBuffer();
          void clearTransmitBuffer();
          uint8_t setTransmitBuffer(uint8_t, uint16_t);
          uint8_t available(void);
          uint16_t receive(void);
          uint16_t getResponseBuffer(uint8_t);


          //Extencion Modbus Inelco
          uint8_t ModbusStructTx(uint8_t,uint8_t ,uint8_t ,uint8_t);
          void resetStructRx();
          
          // utilizo para recibir datos por uart
          uint8_t hayDatosUart0();
          uint8_t recibirDatosUART0();
          void capturoDatos();
          void analizoDato();
          uint8_t analizoCRC();

          // proceso los datos
          void procesarEstado();
          void rxModbusControlador();
          void txControlador(int _IdControl,int _stSalidas1,int _stSalidas2,int _stZonaAlarma,int _stZonaFalla);

          // variables para recibir datos
          
          bool capturarDatos = false;
          uint8_t buffer[BUFFER_SIZE];
          uint8_t longitudBuffer = 0;
          uint8_t u8TotalData;
          uint8_t u8ModbusStructSize = 0;
          uint8_t u8ModbusStrucRx[256];
          uint8_t u8ModbusDataStruct[256];
          uint8_t u8MBStatusStructRx;
          uint8_t u8MBIdOrigen;
          uint8_t u8MBIdStructRx;
          uint8_t u8MBLenghStruct;
          uint8_t contador = 0;
          
          // Función de interrupción para UART0
          bool MiDato=true;
          bool HayDato=false;
          bool uart0_flag = false;  // bandera para la UART0
          bool uart1_flag = false;  // bandera para la UART0
          int banderita;
          int banderita1;
    
          // Definición de los estados
          enum Estado {
            ESTADO_0,
            ESTADO_1,
            ESTADO_2,
            ESTADO_8,
            ESTADO_9
          };

          // Variables globales
          Estado estadoActual = ESTADO_0;
        

    
     private:

          Stream* _serial;                                             ///< reference to serial port object
          uint8_t  _u8MBSlave;                                         ///< Modbus slave (1..255) initialized in begin()
          static const uint8_t ku8MaxBufferSize = 64;   ///< size of response/transmit buffers    
          // uint16_t _u16ReadAddress;                                    ///< slave register from which to read
          // uint16_t _u16ReadQty;                                        ///< quantity of words to read
          uint16_t _u16ResponseBuffer[ku8MaxBufferSize];               ///< buffer to store Modbus slave response; read via GetResponseBuffer()
          // uint16_t _u16WriteAddress;                                   ///< slave register to which to write
          // uint16_t _u16WriteQty;                                       ///< quantity of words to write
          uint16_t _u16TransmitBuffer[ku8MaxBufferSize];               ///< buffer containing data to transmit to Modbus slave; set via SetTransmitBuffer()
          uint16_t* txBuffer; // from Wire.h -- need to clean this up Rx
          uint8_t _u8TransmitBufferIndex;
          uint16_t u16TransmitBufferLength;
          uint16_t* rxBuffer; // from Wire.h -- need to clean this up Rx
          uint8_t _u8ResponseBufferIndex;
          uint8_t _u8ResponseBufferLength;

          // idle callback function; gets called during idle time between TX and RX
          void (*_idle)();
          // preTransmission callback function; gets called before writing a Modbus message
          void (*_preTransmission)();
          // postTransmission callback function; gets called after a Modbus message has been sent
          void (*_postTransmission)();
   

};
#endif // __MODBUSINELCO_H__