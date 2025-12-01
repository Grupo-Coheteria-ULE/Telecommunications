#include <HardwareSerial.h>

#define LORA_RXD 22
#define LORA_TXD 23

#define M0_PIN 18
#define M1_PIN 19
#define AUX_PIN 4

HardwareSerial E22(1);

// Variables para el Emisor
unsigned long ultimoEnvio = 0;
int contador = 0;

void setup() {
  pinMode(M0_PIN, OUTPUT);
  pinMode(M1_PIN, OUTPUT);
  pinMode(AUX_PIN, INPUT);

  Serial.begin(115200);
  while(!Serial);

  E22.begin(9600, SERIAL_8N1, LORA_RXD, LORA_TXD);

  digitalWrite(M0_PIN, LOW);
  digitalWrite(M1_PIN, LOW);

  Serial.println("--- Transmitter / Receiver ready! --");
}

void loop() {
  if (millis() - ultimoEnvio > 5000) {
    String mensaje = "¡B dice Hola! #" + String(contador);
    
    // Escribimos el mensaje al puerto Serial del módulo LoRa
    E22.print(mensaje); 
    
    Serial.print("-> ENVIADO: ");
    Serial.println(mensaje);
    
    contador++;
    ultimoEnvio = millis();
  }

  if (E22.available()) {
    String msg = E22.readString();

    Serial.print("<- RECIBIDO: ");
    Serial.println(msg);
  }

  delay(10);
}