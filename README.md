# CESE 10ma Cohorte - Sistemas operativos de tiempo Real II - TP FINAL

- Luis Lebus
- Ignacio Majul
- César Cruz

El proyecto se dividio en 3 capas, la primera capa uartmanager es la que interactua con la SAPI (driver). Para poder configurarla usamos la estructura uartManagerConfig_t, la cual permite pasar los parametros de configuración de la uart que el usuario desee utilizar y también los caracteres de inicio y fin configurables.

## 1.- UART MANAGER

- La estructura uartManager_t contiene las direcciones que el pool de memoria otorga de a bloques para la transmisión y recepción. Contiene las colas de transmision y recepción y los handlers de los TimeOut. Todos los campos de esta estructura son estaticos.

Dentro de uartManager tenemos las funciones:
   ### - uartManagerInit
        Pasa el handle de configuración y se realizan las validaciones necesarias tanto en la uart como en 
        
        memoria. Por ultimo se hace la configuración de los timers de transmisión y recepción.
   ### - uartManagerDeInit
        Desinicializa lo que se ha usado de forma dinámica:Pool de memoria,colas,timers,etc.
   ### - uartManagerGet
        todo lo que refiere a asignación de memoria y punteros , realizan copias para las capas 
        
        superiores eso evita el problema de la liberación de memoria, posterior a la copia se hace liberación.

        Frente al problema de que el get desconoce la cantidad de caracteres a esperar. Lo que se hizo es 
        
        usar peck para no desencolarlo, esto para obtener el tamaño y posteriormente tener el mensaje

   ### - uartManagerPut

        se solicita un bloque de memoria, luego se calcula el CRC8 posterior a ello se arma el mensaje para 
        
        transmitir agregando los delimitadores. Una vez listo de encola para que sea transmitido por el 
        
        callback del timer.

   ### - rxCallback 
        usamos una maquina de estados para parsear los caracteres, se pide un bloque de memoria, para 
        
        luego analizar cada caracter, al llegar al caracter final se calcula el CRC8, solo si es correcto se 
        
        encola.

## 2.- Pool de memoria

   ### - poolDeinit
        Al inicializar hacemos un vportmalloc de cantidad 
        
        de bloques y su tamaño respectivo que se necesita.
   ### - poolGet
        Nos entrega las direcciones de memoria donde estan ubicados los bloques del pool libre
   ### - poolPut
        Coge el bloque asignado y lo señalisa para poder sobre escribirlo , esto permite que el poolGet 
        
        consulte y asigne,en caso este libre para sobre escribir

## 3.- Sep 
    Esta capa separa en campos para armar o despomponer los 
    
    string, posterior a ello los entrega a "app.c"
   ### - sepInit
        Recibe el handle del uart 
   ### - sepGet
        Para hacer un get, se realiza primero un get con NULL, en size tenemos el elemento a desencolar, con 
        
        el que hacemos la asignación de memoria para posteriormente desencolar, analizamos los elementos 
        
        si son "m" o "M"
   ### - sepPut
        recibe sepData_t y hace el proceso inverso de sepGet, arma el string UPPER o LOWER, luego de 
        
        armarlo completo segun corresponda hace el put

## En la APP

   ### - appInit 
        Inicializa una tarea 
   ### - rxTaskA0
        Es la tarea que ejecuta la aplicación, castea los parametros recibidos y creas los objetos activos.
       
        Se entience flags para indicar que estan los OAs.
        Al crear el objeto activo , indicamos el nombre de 
        
        OA y la funcion a representar
        Luego ejecutamos sepGet que es bloqueante, es decir 
        
        mientras no haya datos se comporta como un portmaxdelay.
        
        Posterior a ello tenemos una maquina de estados que tiene to_lower o to_upper. Crea los objetos activos 
        
        y entrega callbacks que se debe ejecutar segun corresponda, posterior a ello lo encola.
   ### - toLower
        callback de OA para minusculizar 
   ### - toUpper
        callback de OA para mayusculizar  

## AO
   ### - activeObjectCreate
        Si no esta creado el objeto activo, crea la cola y crea la tarea, returnando un true o false.
   ###  - sctiveObjectTask
        Recibe por parametro el OA y pregunta en la cola del OA si existe algo para procesar, en caso haya , 
        
        lo recibe y lo desencola. Ejecuta el callback para mayusculizar o minusculizar. Si no hubiera nada en 
        
        la cola, destruye el OA y apaga el flag de vida del OA 

# secuencia de commits
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
