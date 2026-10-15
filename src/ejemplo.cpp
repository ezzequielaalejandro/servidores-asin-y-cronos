#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

WebServer server(80);

int rojo = 10;
int amarillo = 12;
int verde = 13;

// pagina
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<
</head>
<body>
  <h1 class='titulo'>SEMAFORO</h1>
  <p><a href='/lento'><button class='lento'>LENTO</button></a></p>
  <p><a href='/normal'><button class='normal'>NORMAL</button></a></p>
  <p><a href='/rapido'><button class='rapido'>RAPIDO</button></a></p>
  <p><a href='/emergencia'><button class='emergencia'>EMERGENCIA</button></a></p>

  <p><a class="span">Modo: __MODO_SEMAFORO__</a></p>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  // conexion al wifi como antes
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int timeout = 20; // 20 intentos de 500ms = 10 segundos
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    Serial.print(".");
    timeout--;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFallo la conexion. Reiniciando...");
    delay(1000);
    ESP.restart();
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("Dirección IP: http://");
  Serial.println(WiFi.localIP());

  server.on("/lento", []() {
    String pagina = pagina_template;

    pagina.replace("__MODO_SEMAFORO__", "LENTO");

    digitalWrite(rojo, HIGH);
    digitalWrite(amarillo, LOW);
    digitalWrite(verde, LOW);
    delay(70000);
    digitalWrite(rojo, LOW);
    digitalWrite(amarillo, HIGH);
    digitalWrite(verde, LOW);
    delay(3000);
    digitalWrite(rojo, LOW);
    digitalWrite(amarillo, HIGH);
    digitalWrite(verde, LOW);
    delay(50000);

  });
  server.on("/normal", []() {
    String pagina = pagina_template;

    pagina.replace("__MODO_SEMAFORO__", "NORMAL");

    digitalWrite(rojo, HIGH);
    digitalWrite(amarillo, LOW);
    digitalWrite(verde, LOW);
    delay(45000);
    digitalWrite(rojo, LOW);
    digitalWrite(amarillo, HIGH);
    digitalWrite(verde, LOW);
    delay(2000);
    digitalWrite(rojo, LOW);
    digitalWrite(amarillo, HIGH);
    digitalWrite(verde, LOW);
    delay(65000);
  });
  server.on("/rapido", []() {
    String pagina = pagina_template;

    pagina.replace("__MODO_SEMAFORO__", "RAPIDO");

    digitalWrite(rojo, HIGH);
    digitalWrite(amarillo, LOW);
    digitalWrite(verde, LOW);
    delay(20000);
    digitalWrite(rojo, LOW);
    digitalWrite(amarillo, HIGH);
    digitalWrite(verde, LOW);
    delay(1000);
    digitalWrite(rojo, LOW);
    digitalWrite(amarillo, HIGH);
    digitalWrite(verde, LOW);
    delay(40000);
  });
  server.on("/emergencia", []() {
    String pagina = pagina_template;

    pagina.replace("__MODO_SEMAFORO__", "EMERGENCIA");

    digitalWrite(rojo, LOW);
    digitalWrite(amarillo, LOW);
    digitalWrite(verde, LOW);
    delay(1500);
    digitalWrite(rojo, HIGH);
    digitalWrite(amarillo, HIGH);
    digitalWrite(verde, LOW);
    delay(1500);
    
  });

  server.begin();
}

void loop(){
    server.handleClient();
}