  #include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WiFi.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

AsyncWebServer server(80);

int lectura = 0;
String txt;
int ldr = 5;

// pagina
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <title>MONITOR DE LUZ AMBIENTAL</title>
  <style>
    body {  
            background-color: __LUZ_ABIENTAL__;
            text-align: center;
            padding-top: 50px;
         }

    .titulo {
            font-family: 'Segoe UI';
            font-weight: bold;
            font-style: italic;
            font-size: 32px; color: white;
     }
    .lectura { 
            border-color: white;
            border: 3px;
            border-style: dotted;
            border-radius: 20px;
            padding: 25px;
        }
        .valor{
        font-size: bold;
    }
  </style>
</head>
<body>
    <div class="lectura">
        <span><a class="valor">Nivel de luz: __NIVEL_DE_LUZ__ %</a></span>
    </div>
  <h1 class='titulo'>Monitor de Luz Ambiental</h1>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  //configuracion de pines
  pinMode(ldr, INPUT);

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
    pagina.replace("__NIVEL_DE_LUZ__", txt);
    if(lectura < 33){pagina.replace("__LUZ_AMBIENTAL__", "#2c3e50"); }
    if(lectura >= 33 && lectura < 66){pagina.replace("__LUZ_AMBIENTAL__", "#e67e22"); }
    if(lectura > 66){pagina.replace("__LUZ_AMBIENTAL__", "#f1c40f"); }

    request -> send(200, "text/html", pagina); //envio de pagina de manera asincrona
  });
  

  server.begin();
}

void loop(){
    lectura = map(analogRead(ldr), 0, 1023, 0, 100);
    txt=(String) lectura;
      
}