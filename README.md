# CESE 10ma Co Sistemas operativos de tiempo Real II - TP FINAL

- Luis Lebus
- Ignacio Majul
- Cesar Cruz

## 1 Manejo del pool de memoria
    commit:8506012 Mar 26, 2020 at 20:01

 ###  Agregados poolInit,poolDeinit,poolGet,poolPut


## 2 Creación de UarManager
    commit:4c3aa41 Mar 27, 2020 at 17:14

 ###  Agregados uartManagerInit,uartManagerDeinit,uartManagerGet,uartManagerPut,rxCallBack,txTask
 ###  Agregados appInit, rxTask

## 3 Agregando CRC
    commit:2883861 Mar 27, 2020 at 15:01
### Agregados crc8_init,crc8_calc

## 4 pool de memoria actualización
    commit:06e8794 Apr 3, 2020 at 06:37

### Se pone en cero la memoria solicitada antes de entregarla, se asigna NULL al puntero principal luego de liberarlo

## 5 uart Manager corrección de errores
    commit:9991973  Apr 3, 2020 at 17:21

### Agregados onRxTimeoutCallback,onTxTimeoutCallback

## 6 Actualizaciones en sep
    commit:a17ff17 Apr 4, 2020 at 11:16

### se agregan funcionalidades a sepGet. Se modifica la estructura sepHandle_t.

### Agregados sepInit,sepGet,sepPut

## 7 Correcciones sep
    commit: ec49361 Apr 4, 2020 at 11:34

### se modifica sepGet

## 8 Actualizaciones sep & app
    commit:1fff247 Apr 4, 2020 at 11:48

### actualizaciones sepGet & sepPut

## 9 Actualizaciones sep & app
    commit: a17ff17 Apr 4, 2020 at 11:16

### se agregan funcionalidades a sepGet. Se modifica la estructura sepHandle_t.

## 10 Integración CRC8 con UartManager
    commit : a4bbdd6 Apr 4, 2020 at 14:32

### Se agrega el calculo de crc8 en uartManager	 

## 11 Se agrega OA aun sin integrar
    commit:4d7735c Apr 17, 2020 at 16:50

### creacion de funciones del AO

## 12 Callbacks de OA  
    commit:b5e2e90 Apr 17, 2020 at 23:26

## 13 se corrige prototipo del callback de AO UpperCallback
    commit:	2c2f125	Apr 21, 2020 at 15:31

## 14 se independiza la App del contenido de los AO
    commit: 4e7f7c6	Apr 21, 2020 at 21:37

## 15 se agrega la api para destruccion de los AO
    commit:f8757b6	Apr 23, 2020 at 19:32
