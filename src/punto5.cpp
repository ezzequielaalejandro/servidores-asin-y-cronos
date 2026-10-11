#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <Wifi.h>

const char* ssid = "CESJT";
const char* password = "smgitisjt";

AsyncWebServer server (80);

int notado = 10;
int re = 50, mi = 80, sol = 120, la = 180, silencio = 0;
int buzzer = 17;
const char pagina_template [] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8'>
        <!DOCTYPE html>

<html>
    <head>
        <meta charset="utf-8">
        <title>GENERADOR DE TONOS</title>
        <style>
            .body{
                font-family: verdana;
                text-aling: center;
            }
            .est_nota{
                width: 400px;
                height: 80px;
                border: 2px dashed #000;
                text-aling: center;
            }

            .btn{
                display: flex;
                width: 60px;
                height: 150px;
            }
        </style>
    </head>
    <body>
        <h1>GENERADOR DE TONOS</h1>

        <div class="est_nota">TOCANDO: __NOTA_SONANDO__</div>

        <div class="btn do"> <button>DO</button></div>
        <div class="btn re"><button>RE</button></div>
        <div class="btn mi"><button>MI</button></div>
        <div class="btn sol"><button>SOL</button></div>
        <div class="btn la"><button>LA</button></div>
        <div class="btn silencio"><button>SILENCIO</button></div>
    </body>
</html>
</head>
</html>
)rawliteral";

void setup(){

    pinMode(buzzer, OUTPUT);

    Serial.begin (115200);

    Serial.print("Conectando a:");
    Serial.print(ssid);
    WiFi.begin(ssid, password);

    int timeout = 20;
    while(WiFi.status() != WL_CONNECTED && timeout > 0){
        delay(500);
        Serial.print(".");
        timeout --;
    }

    if(WiFi.status() != WL_CONNECTED){
        Serial.print ("fallo de conexion, Reinciando...");
        delay(1000);
        ESP.restart();
    }

    Serial.print("CONECTADO");
    Serial.print("Direccion de IP: http://");
    Serial.print(WiFi.localIP());

    server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request){
        String pagina = pagina_template;

        request -> send(200, "text/html", pagina); //envio la pagina
    });

    server.on("/do", HTTP_GET, [] (AsyncWebServerRequest *request){
        String pagina = pagina_template;
        analogWrite(buzzer, notado);
        pagina.replace("__NOTA_SONANDO__", "DO");
        request -> redirect ("/");
    });
    server.on("/re", HTTP_GET, [] (AsyncWebServerRequest *request){
        String pagina = pagina_template;
        analogWrite(buzzer, re);
        pagina.replace("__NOTA_SONANDO__", "RE");
        request -> redirect ("/");
    });
    server.on("/mi", HTTP_GET, [] (AsyncWebServerRequest *request){
        String pagina = pagina_template;
        analogWrite(buzzer, mi);
        pagina.replace("__NOTA_SONANDO__", "MI");
        request -> redirect ("/");
    });
    server.on("/sol", HTTP_GET, [] (AsyncWebServerRequest *request){
        String pagina = pagina_template;
        analogWrite(buzzer, sol);
        pagina.replace("__NOTA_SONANDO__", "SOL");
        request -> redirect ("/");
    });
    server.on("/la", HTTP_GET, [] (AsyncWebServerRequest *request){
        String pagina = pagina_template;
        analogWrite(buzzer, la);
        pagina.replace("__NOTA_SONANDO__", "LA");
        request -> redirect ("/");
    });
    server.on("/la", HTTP_GET, [] (AsyncWebServerRequest *request){
        String pagina = pagina_template;
        analogWrite(buzzer, silencio);
        pagina.replace("__NOTA_SONANDO__", "SILENCIO");
        request -> redirect ("/");
    });

    server.begin();
}

void loop(){}
