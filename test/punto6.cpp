#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>

const char* password = "itisjtsmg";
const char* ssid = "CESJT";
AsyncWebServer server(80);

//VARIABLES PROPIAS
int led = 12;
int t_actual, t_milis =600, t_previo = 0;

String modo_actual = "XXXX";
String temperatura_objetivo = "YYYY";
String estado_ventilador = "ZZZZ";
String color_panel = "#D3D3D3";
String color_texto = "black";

void setup(){

    pinMode(led, OUTPUT);


const char pagina_template [] PROGMEM =R"rawliteral(

<!DOCTYPE html>

<html>
    <head>
        <meta charset ="UTF-8">
        <title>SISTEMA DE REFRIGERACION</title>
        <style>
            body{
                text-align: center;
                font-family: Arial Black;
                background-color: #ecf0f1;
                font-size: 34px;
            }

            .panel-temp{
                width: 550px;
                padding: 30px;
                border-radius: 20px;
                font-size: 28px;
                background-color: __COLOR_PANEL__ ;
                color: __COLOR_TEXTO__ ;
            }

        </style>
        <body>
            <h1>SISTEMA DE REFRIGERACION</h1>
            <div class="panel-temp">
                <a>MODO ACTUAL: __MODO_ACTUAL__</a>
                <a>TEMPERATURA OBJETIVO: __TEMPERATURA_OBJETIVO_</a>
                <a>ESTADO VENTILADOR: __ESTADO_VENTILADOR__</a>
            </div>

            <button><a href="/detenido">Detenido</a></button>
            <button><a href="/ventilacion">Ventilacion</a></button>
            <button><a href="/intensivo">Intensivo</a></button>
            
        </body>
    </head>
</html>

)rawliteral";

// ENCIENDO PINES DE LA ESP32

Serial.begin(115200);

Serial.print("Conectando a:"); //me conectro a el wifi
Serial.print(ssid);
WiFi.begin(ssid, password);

int timeout = 20;
while(WiFi.status() != WL_CONNECTED && timeout > 0){
        delay(500);
        Serial.print (".");
        timeout-- ;
}

if(WiFi.status() != WL_CONNECTED){
    Serial.print("Fallo de coneccion, reintentendo");
    delay(1000);
    ESP.restart();
}

Serial.print("Conectado!");
Serial.print("direccion IP= https//:");
Serial.print(WiFi.localIP());

server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request){
    String pagina = pagina_template;

    pagina.replace("__MODO_ACTUAL__", modo_actual);
    pagina.replace("__TEMPERATURA_OBJETIVO__", temperatura_objetivo);
    pagina.replace("__ESTADO_VENTILADOR__", estado_ventilador);
    pagina.replace("__COLOR_TEXTO__", color_texto);
    pagina.replace("__COLOR_PANEL__", color_panel);

    request -> send(200, "text/html", pagina);
});

server.on("/detenido", HTTP_GET, [] (AsyncWebServerRequest *request){
    modo_actual ="DETENIDO";
    temperatura_objetivo ="--°C";
    estado_ventilador = "APAGADO";
    color_panel = "gray";
    color_texto ="black";
    digitalWrite(led, LOW);
    request -> redirect ("/");
});

server.on("/ventilacion", HTTP_GET, [] (AsyncWebServerRequest *request){
    modo_actual ="VENTILACION";
    temperatura_objetivo ="25°C";
    estado_ventilador = "VELOCIDAD MEDIA";
    color_panel = "sky-blue";
    color_texto ="black";

    int est = 0;
    if(t_previo - t_actual >= t_milis){
        digitalWrite(led, est);
        est = !est;
        t_previo = t_actual;
    }

    request -> redirect("/");
});

server.on("/intensivo", HTTP_GET, [] (AsyncWebServerRequest *request){
    modo_actual ="ENFRIAMIENTO INTENSIVO";
    temperatura_objetivo ="18°C";
    estado_ventilador = "VELOCIDAD ALTA";
    color_panel = "#111184";
    color_texto = "white";

    digitalWrite(led, HIGH);
});

 server.begin();
}

void loop(){
    t_actual = millis();
}