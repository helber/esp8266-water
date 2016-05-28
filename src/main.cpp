#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

const char* ssid = "seu ssid";
const char* password = "sua senha wifi";
String json = "";
bool stringComplete = false;
// 4800 9600 19200 38400 57600 115200
#define SERIAL_BAUD 115200

ESP8266WebServer server(80);

void handleRoot() {
    char temp[350];
    int sec = millis() / 1000;
    int min = sec / 60;
    int hr = min / 60;

    snprintf ( temp, 350,
"<html><head><meta charset='UTF-8'>\
<script src='http://code.jquery.com/jquery-2.2.0.min.js'></script>\
<link href='http://10.0.0.10/sensor/main.css' rel='stylesheet'/>\
<script src='http://10.0.0.10/sensor/main.js'></script>\
</head><body>\
<p>Uptime: %02d:%02d:%02d</p>\
<div id='s1'></div>\
<div id='s2'></div>\
</body></html>",
        hr, min % 60, sec % 60
    );
    Serial.print("GET /");
    server.send ( 200, "text/html", temp );
};

void handleNotFound() {
    Serial.print("File Not Found ");
    Serial.print(server.uri());
    Serial.print("");
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for ( uint8_t i = 0; i < server.args(); i++ ) {
        message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
    }
    server.send ( 404, "text/plain", message );
};

void handleSensor() {
    int i = 0;
    json = "";
    Serial.write("R");
    while (stringComplete == false && i < 20000) {
        while (Serial.available() > 0) {
            char inChar = (char) Serial.read();
            json += inChar;
            if (inChar == '}') {
              stringComplete = true;
            }
        }
        i += 1;
        // delay(1);
    }
    stringComplete = false;
    // Serial.print("SENSOR");
    server.send( 200, "application/json", json );
};

void setup() {
    Serial.begin(SERIAL_BAUD);
    // Serial.setDebugOutput(true);
    // delay(1000);
    // while (!Serial) {
    //     ; // wait for serial port to connect. Needed for native USB port only
    // };
    Serial.flush();
    Serial.println("Serial UP");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        // Serial.println("Connection Failed! Rebooting... ");
        delay(100);
        // ESP.restart();
        // ESP.reset();
        Serial.print(".");
    };

    Serial.println("\nReady ...");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if ( MDNS.begin ( "esp8266" ) ) {
        Serial.println ( "MDNS responder started" );
    }

    server.on ( "/", handleRoot );
    server.on ( "/sensor", handleSensor );
    server.onNotFound ( handleNotFound );
    server.begin();
    // delay(500);
};

void loop() {
    char chLixo;
    while (Serial.available() > 0) {
        chLixo = (char) Serial.read();
    }
    Serial.flush();
    ESP.wdtDisable();
    server.handleClient();
};
