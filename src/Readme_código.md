# Comunicacion LoRa entre ESP32

Este proyecto implementa comunicacion bidireccional entre dos modulos LoRa conectados a placas ESP32. Los dos programas funcionan en conjunto para establecer un enlace de comunicacion inalambrica de largo alcance.

## Que es LoRa

LoRa (Long Range) es una tecnologia de comunicacion inalambrica de largo alcance y bajo consumo. Permite transmitir datos a distancias de varios kilometros sin necesidad de conexion a internet ni infraestructura adicional.

## Descripcion de los programas

### Base.ino (Nodo B)

Este programa es el codigo basico para el segundo nodo de la comunicacion LoRa.

**Funcionalidad:**
- Envia mensajes cada 5 segundos con el formato "¡B dice Hola! #X" donde X es un contador
- Recibe y muestra por el monitor serial cualquier mensaje que llegue del otro nodo
- Funciona como transmisor y receptor simultaneamente

**Pines utilizados:**
- Pin 22: RXD (Recepcion del modulo LoRa)
- Pin 23: TXD (Transmision del modulo LoRa)
- Pin 18: M0 (Control de modo)
- Pin 19: M1 (Control de modo)
- Pin 4: AUX (Pin auxiliar del modulo)

### testeadorWifi.ino (Nodo A)

Este programa es una version avanzada que ademas de la comunicacion LoRa, crea un servidor web para monitorizar los mensajes en tiempo real.

**Funcionalidad:**
- Envia mensajes cada 5 segundos con el formato "¡A dice Hola! #X" donde X es un contador
- Recibe y muestra mensajes del otro nodo
- Crea un punto de acceso WiFi llamado "LoRa_Monitor"
- Publica una pagina web accesible desde cualquier dispositivo conectado al WiFi
- La pagina web muestra un historial de los ultimos 50 mensajes enviados y recibidos
- Actualiza la informacion automaticamente cada 2 segundos

**Pines utilizados:**
- Misma configuracion que Base.ino (pines 22, 23, 18, 19 y 4)

**Acceso a la interfaz web:**
1. Conectarse a la red WiFi "LoRa_Monitor" (contraseña: 12345678)
2. Abrir un navegador web
3. Ir a la direccion: http://192.168.4.1

## Hardware necesario

Para montar este sistema necesitas:

- 2 placas ESP32
- 2 modulos LoRa E22 (compatibles con comunicacion serial)
- Cables para conectar los modulos a las placas (dupont)
- Alimentacion para ambas placas (USB o bateria)

## Configuracion del hardware

Conecta cada modulo LoRa a su ESP32 siguiendo este esquema:

| Modulo LoRa | ESP32 |
|-------------|-------|
| RX          | Pin 23 (TX) 
| TX          | Pin 22 (RX) 
| M0          | Pin 18 
| M1          | Pin 19 
| AUX         | Pin 4 
| VCC         | 3.3V ó 5V (+ potencia) 
| GND         | GND 

## Como usar

1. Carga el programa `Base.ino` en el primer ESP32 (Nodo B)
2. Carga el programa `testeadorWifi.ino` en el segundo ESP32 (Nodo A)
3. Alimenta ambas placas (placa A, se puede alimentar con un cable tipo C-tipoC directamente al móvil para transporte)
4. Abre el monitor serial (115200 baudios) para ver los mensajes en tiempo real
5. En el Nodo A, conectate a la red WiFi "LoRa_Monitor" y accede a la interfaz web a través de la ip.

## Funcionamiento

Una vez en marcha, veras como:

- El Nodo A envia mensajes cada 5 segundos que dice "¡A dice Hola! #1", "¡A dice Hola! #2", etc.
- El Nodo B recibe estos mensajes y los muestra
- El Nodo B envia mensajes cada 5 segundos que dice "¡B dice Hola! #1", "¡B dice Hola! #2", etc.
- El Nodo A recibe estos mensajes y los muestra

En el monitor serial se ve:
```
-> ENVIADO: ¡A dice Hola! #1
<- RECIBIDO: ¡B dice Hola! #1
-> ENVIADO: ¡A dice Hola! #2
<- RECIBIDO: ¡B dice Hola! #2
```

## Notas tecnicas

- Los modulos LoRa funcionan a 9600 baudios
- La comunicacion serial con el ordenador es a 115200 baudios
- Los pines M0 y M1 se configuran en LOW para modo de transmision normal
- El sistema guarda los ultimos 50 mensajes en el buffer del servidor web
- La actualizacion de la pagina web es automatica cada 2 segundos

## Modificaciones posibles

Modificaciones para testeo:

- El intervalo de envio (cambia el valor 5000 en `millis() - ultimoEnvio > 5000`)
- El contenido de los mensajes (modifica la variable `mensaje`)
- El nombre y contraseña del WiFi (cambia las variables `ssid` y `password`)
- La cantidad de mensajes guardados (cambia `MAX_MENSAJES`)
- Los pines de conexion (modifica los `#define` al inicio del codigo)
