//SERVIDOR SINCRONO

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid ="CESJT";
const char* password ="itisjtsmg";

WebServer server (80);

//variables propias
int estado = 0;
int led_verde = 10, led_rojo = 9;

//pagina

const char pagina_template [] PROGMEM = R"rawliteral(
<!DOCTYPE html>

<html>
    <head>
        <meta charset="UTF-8">
        <title>CONTROL DE ACCESO</title>
        <style>
            body{
                text-align: center;
                font-family: Consolas;
                background-color: __COLOR_FONDO__;
                color: __COLOR_TEXTO__;
                text-size-adjust: 36px;
            }
            .panel-acceso{
                width: 70%;
                padding: 35px;
                border-radius: 20px;
            }
            .btn-acceso{
                text-align: center;
                width: 170px;
                height: 55px; 
            }
            .panel-acceso indicadorfisico{
                color: __COLOR_FISICO__;
            }
        </style>
        <body>
            <h1>CONTROL DE ACCESO</h1>
            <div class="panel-acceso">
                <h3>ESTADO DE ACCESO: __ESTADO_ACCESO__</h3>
                <h7 class="panel-acceso indicadorfisico">INDICADOR FISICO: __INDICADOR_FISICO__</h7>
            </div>
            <div class="btn-acceso">
                <button><a href="/permitir">PERMITIR</a></button>
                <button><a href="/restringir">RESTRINGIR</a></button>
                <button><a href="/bloquear">BLOQUEAR</a></button>
            </div>
        </body>    
</head>
</html>
)rawliteral";

void setup(){

    pinMode(led_verde, OUTPUT);
    pinMode(led_rojo, OUTPUT);

    Serial.begin(115200);

    Serial.print("CONECTANDO A:");
    Serial.print(ssid);
    WiFi.begin(ssid, password);

    int timeout = 20;
    while(WiFi.status() != WL_CONNECTED && timeout > 0){
        Serial.print(".");
        delay(500);
        timeout--;
    }

    if(WiFi.status() != WL_CONNECTED){
        Serial.print("FALLO DE CONECCION, REINTENTANDO");
        delay(1000);
        ESP.restart();
    }

    Serial.print("CONECTADO!");
    Serial.print("direccion IP: https://");
    Serial.print(WiFi.localIP());

    server.on("/", [] (){
        String pagina = pagina_template;
    });

    server.on("/permitir", [] (){
        String pagina = pagina_template;
        
        pagina.replace("__COLOR_FONDO__", "green");
        pagina.replace("__COLOR_TEXTO__", "white");
        pagina.replace("__ESTADO_ACCESO__", "PERMITIR");
        pagina.replace("__INDICADOR_FISICO__", "VERDE");

        digitalWrite(led_rojo, LOW);
        digitalWrite(led_verde, HIGH);
    });
    server.on("/restringir", [] (){
        String pagina = pagina_template;
        
        pagina.replace("__COLOR_FONDO__", "yellow");
        pagina.replace("__COLOR_TEXTO__", "black");
        pagina.replace("__ESTADO_ACCESO__", "RESTRINGIR");
        pagina.replace("__INDICADOR_FISICO__", "ALTERNANDO");

        digitalWrite(led_rojo, estado);
        digitalWrite(led_verde, !estado);
        delay(500);
        estado= !estado;
    });
    server.on("/bloquear", [] (){
        String pagina = pagina_template;
        
        pagina.replace("__COLOR_FONDO__", "red");
        pagina.replace("__COLOR_TEXTO__", "white");
        pagina.replace("__ESTADO_ACCESO__", "BLOQUEAR");
        pagina.replace("__INDICADOR_FISICO__", "ROJO");

        digitalWrite(led_rojo, HIGH);
        digitalWrite(led_verde, LOW);
    });

    server.begin();
}

void loop(){
    server.handleClient();
}