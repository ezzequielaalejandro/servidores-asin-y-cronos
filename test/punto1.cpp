#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

WebServer server(80);

  //en analog.write el maximo de salida es de 255, osea, 0 apagado y 255 alta
int salida_ventilador = 5; 
int lento = 50;
int medio = 150;
int alto = 255;

// pagina
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <title>Servidor Web ESP32</title>
  <style>
    body {  
        background-color: #ecf0f1;
        text-align: center; 
        background-attachment: fixed;
    }
    .titulo { 
        font-family: Arial black; 
        font-weight: bold;  
        font-size: 45px; 
        color: #2c3e50;
        text-decoration: underline; 
    }
    .estado{
        font-weight: bold;
    }
    .button{
        width: 180px;
        height: 60px;
        border-color: 1px;
        border-radius: 15px;
        font-weight: bold;
    }
    .alta{
        background-color: #f39c12;
    }
    .media{
        background-color: #27ae60;
    }
        .baja{
            background-color: #2980b9;
    }
    .apagado{
        background-color: #c0392b;
    }
  </style>
</head>
<body>
  <h1 class='titulo'>CONTROL DE VENTILADOR</h1>
   <span class="estado">Velocidad Actual: __VELOCIDAD_ACUTAL__</span>
  <p><a href='/alta'><button class='button alta'>Alta</button></a></p>
  <p><a href='/media'><button class='button media'>Media</button></a></p>
  <p><a href='/baja'><button class='button baja'>Baja</button></a></p>
  <p><a href='/apagado'><button class='button apagado'>Apagado</button></a></p>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  //configuracion de pines
  pinMode(salida_ventilador, OUTPUT);

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

    //aca puedo desarrolar lo que quiero que pase en el server
    request -> send(200, "text/html", pagina); //envio de pagina de manera asincrona
  });
  server.on("/alta", HTTP_GET, [](AsyncWebServerRequest *request){
    String pagina = pagina_template;

    //aca puedo desarrolar lo que quiero que pase en el server
    pagina.replace("__VELOCIDAD_ACUTAL__", "ALTA");
    analogWrite(salida_ventilador, alto);
    request -> redirect("/"); //envio de pagina de manera asincrona
  });
  server.on("/media", HTTP_GET, [] (AsyncWebServerRequest *request){
    String pagina = pagina_template;

    //aca puedo desarrolar lo que quiero que pase en el server
    pagina.replace("__VELOCIDAD_ACUTAL__", "MEDIA");
    analogWrite(salida_ventilador, medio);

    request -> redirect("/") //envio de pagina de manera asincrona
  });
  server.on("/baja", HTTP_GET, [] (AsyncWebServerRequest *request){
    String pagina = pagina_template;
    //aca puedo desarrolar lo que quiero que pase en el server
    pagina.replace("__VELOCIDAD_ACUTAL__", "BAJA");
    analogWrite(salida_ventilador, lento);

    request -> redirect("/") //envio de pagina de manera asincrona
  });
  server.on("/apagado", HTTP_GET, [] (AsyncWebServerRequest *request){
    String pagina = pagina_template;
    //aca puedo desarrolar lo que quiero que pase en el server
    pagina.replace("__VELOCIDAD_ACUTAL__", "APAGADO");
    analogWrite(salida_ventilador, 0);

    request -> redirect("/") //envio de pagina de manera asincrona
  });
  

  server.begin();
}

void loop(){  
}