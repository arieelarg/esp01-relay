#include <ESP8266WiFi.h>
//#include <WiFi.h>

const char *ssid = "Tu red wifi acá";
const char *password = "Tu password acá";

const int rele = 0; //GPIO0

WiFiServer server(80);

IPAddress ip(192, 168, 0, 200);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void setup()
{
    Serial.begin(9600);
    Serial.println();

    pinMode(rele, OUTPUT);
    digitalWrite(rele, LOW);

    Serial.printf("Conectando a %s", ssid);
    WiFi.begin(ssid, password);
    WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    server.begin();

    //Serial.println(WiFi.localIP());

    Serial.printf("Conectado a %s", ssid);
    Serial.print(" con IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("Servidor WEB activo ");
}

void loop()
{
    //comprueba si un cliente está conectado
    WiFiClient client = server.available();
    if (!client)
    {
        return;
    }

    //lee la primera línea de la petición
    String req = client.readStringUntil('\r');
    Serial.println(req);
    client.flush();
    //busca en las peticiones los comandos de encendido y apagado
    int val;
    if (req.indexOf("/off") != -1)
        val = 0;
    else if (req.indexOf("/on") != -1)
        val = 1;
    else
    {
        Serial.print("Petición inválida");
        client.stop();
        return;
    }
    digitalWrite(rele, val);
    client.flush();
    //prepara la respuesta a la petición
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nrele ";
    s += (val) ? "encendido" : "apagado";
    s += "</html>\n";
    //envía la respuesta
    client.print(s);
    delay(1);
    Serial.println("cliente desconectado");
}
