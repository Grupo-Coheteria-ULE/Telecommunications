#include <HardwareSerial.h>
#include <WiFi.h>
#include <WebServer.h>

#define LORA_RXD 22
#define LORA_TXD 23

#define M0_PIN 18
#define M1_PIN 19
#define AUX_PIN 4

// Configuración del Access Point
const char* ssid = "LoRa_Monitor";
const char* password = "12345678";

HardwareSerial E22(1);
WebServer server(80);

// Variables para el Emisor
unsigned long ultimoEnvio = 0;
int contador = 0;

// Buffer para almacenar mensajes (últimos 50)
#define MAX_MENSAJES 50
String mensajes[MAX_MENSAJES];
int indiceMensaje = 0;
int totalMensajes = 0;

void agregarMensaje(String tipo, String msg) {
  String timestamp = String(millis() / 1000) + "s";
  mensajes[indiceMensaje] = "[" + timestamp + "] " + tipo + ": " + msg;
  indiceMensaje = (indiceMensaje + 1) % MAX_MENSAJES;
  if (totalMensajes < MAX_MENSAJES) totalMensajes++;
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>LoRa Monitor</title>";
  html += "<style>";
  html += "body { font-family: monospace; background: #1a1a1a; color: #0f0; margin: 0; padding: 20px; }";
  html += ".header { background: #000; border: 2px solid #0f0; padding: 15px; margin-bottom: 20px; text-align: center; }";
  html += ".mensaje { background: #000; border: 1px solid #0f0; padding: 10px; margin: 5px 0; border-radius: 5px; }";
  html += ".enviado { color: #00ff00; }";
  html += ".recibido { color: #00ffff; }";
  html += ".container { max-width: 800px; margin: 0 auto; }";
  html += "h1 { margin: 0; color: #0f0; font-size: 1.2em; }";
  html += ".stats { font-size: 0.9em; color: #0f0; margin-top: 10px; }";
  html += "</style>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<div class='header'>";
  html += "<h1>LoRaW Telemetry Monitor v2.4</h1>";
  html += "<p>Nodo A &lt;--&gt; Nodo B (Grupo Cohetería ULE)</p>";
  html += "<div class='stats'>Mensajes totales: " + String(contador) + " | Conectado</div>";
  html += "</div>";
  
  // Mostrar mensajes en orden inverso (más recientes primero)
  int inicio = (indiceMensaje - 1 + MAX_MENSAJES) % MAX_MENSAJES;
  for (int i = 0; i < totalMensajes; i++) {
    int idx = (inicio - i + MAX_MENSAJES) % MAX_MENSAJES;
    String msg = mensajes[idx];
    String clase = msg.indexOf("ENVIADO") > 0 ? "enviado" : "recibido";
    html += "<div class='mensaje " + clase + "'>" + msg + "</div>";
  }
  
  html += "</div></body></html>";
  server.send(200, "text/html", html);
}

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

  // Configurar Access Point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Configurar servidor web
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Servidor web iniciado en http://192.168.4.1");
}

void loop() {
  server.handleClient();

  if (millis() - ultimoEnvio > 5000) {
    String mensaje = "¡A dice Hola! #" + String(contador);
    
    E22.print(mensaje); 
    
    Serial.print("-> ENVIADO: ");
    Serial.println(mensaje);
    
    agregarMensaje("ENVIADO", mensaje);
    
    contador++;
    ultimoEnvio = millis();
  }

  if (E22.available()) {
    String msg = E22.readString();

    Serial.print("<- RECIBIDO: ");
    Serial.println(msg);
    
    agregarMensaje("RECIBIDO", msg);
  }

  delay(10);
}