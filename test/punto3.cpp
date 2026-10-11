#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WiFi.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

AsyncWebServer server(80);

int color = 0;
int pinR = 5;
int pinV = 6;
int pinA = 7;

// pagina
const char pagina_template[] PROGMEM = R"rawliteral(
 <!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <meta http-equiv="refresh" content="5">
  <title>SELECTOR DE COLOR RGB</title>
  <style>
    body {  
            text-align: center;
            background-color: black;
            color: white;
         }

    .botones{
//HOLAAA


        display: flex;
        justify-content: center;
    }
    .rojo{
        border-radius: 50%;
        width: 80px;
        height: 80px;
        border: 3px;
        border-color: white;
        background-color: red;
    }
    .apagado{
        border-radius: 50%;
        width: 80px;
        height: 80px;
        border: 3px;
        border-color: white;
        background-color: #34495e;
    }
    .verde{
        border-radius: 50%;
        width: 80px;
        height: 80px;
        border: 3px;
        border-color: white;
        background-color: green;
    }
    .azul{
        border-radius: 50%;
        width: 80px;
        height: 80px;
        border: 3px;
        border-color: white;
        background-color: blue;
    }
    .blanco{
        border-radius: 50%;
        width: 80px;
        height: 80px;
        border: 3px;
        border-color: white;
        background-color: white;
    }
  </style>
</head>
<body>
    <h1 class="titulo">Selector de Color RGB</h1>
    <span class="color">Color Actual: __COLOR_ACTUAL__</span>
    <div class="botones">
        <button class="botones rojo"><a href="/rojo">Rojo</a></button>
        <button class="botones verde"><a href="/verde">Verde</a></button>
        <button class="botones azul"><a href="/azul">Azul</a></button>
        <button class="botones blanco"><a href="/blanco">Blanco</a></button>
        <button class="botones apagado"><a href="/apagado">Apagado</a></button>
    </div>
</body>
</html>
)rawliteral";

void setup() {


  Serial.begin(115200);

  //configuracion de pines


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

  server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request){
    String pagina = pagina_template;
    request -> send(200, "text/html", pagina);
    });

    server.on("/rojo", HTTP_GET, [] (AsyncWebServerRequest *request){
        String pagina = pagina_template;
        color = 1;
        pagina.replace("__COLOR_ACTUAL__", "Rojo");
        request -> redirect("/");
    });
    server.on("/apagado", HTTP_GET, [] (AsyncWebServerRequest *request){
        String pagina = pagina_template;
        pagina.replace("__COLOR_ACTUAL__", "Apagado");
        color = 0;
        request -> redirect("/");
    });
    server.on("/verde", HTTP_GET, [] (AsyncWebServerRequest *request){
        String pagina = pagina_template;
        pagina.replace("__COLOR_ACTUAL__", "Verde");
        color = 2;
        request -> redirect("/");
    });
    server.on("/azul", HTTP_GET, [] (AsyncWebServerRequest *request){
        String pagina = pagina_template;
        pagina.replace("__COLOR_ACTUAL__", "Azul");
        color = 3;
        request -> redirect("/");
    });
    server.on("/blanco", HTTP_GET, [] (AsyncWebServerRequest *request){
        String pagina = pagina_template;
        pagina.replace("__COLOR_ACTUAL__", "Blanco");
        color = 4;
        request -> redirect("/");
    });

  server.begin();
}

void loop(){
    switch (color)
    {
    case 0:
        digitalWrite(pinR, LOW);
        digitalWrite(pinV, LOW);
        digitalWrite(pinA, LOW);
        break;
    case 1:
        digitalWrite(pinR, HIGH);
        digitalWrite(pinV, LOW);
        digitalWrite(pinA, LOW);
        break;
    case 2:
        digitalWrite(pinR, LOW);
        digitalWrite(pinV, HIGH);
        digitalWrite(pinA, LOW);
        break;
    case 3:
        digitalWrite(pinR, LOW);
        digitalWrite(pinV, LOW);
        digitalWrite(pinA, HIGH);
        break;
    case 4:
        digitalWrite(pinR, HIGH);
        digitalWrite(pinV, HIGH);
        digitalWrite(pinA, HIGH);
        break;
    };
}