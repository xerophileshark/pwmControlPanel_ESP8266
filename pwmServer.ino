/*
* pwmServer.ino
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "Your wifi router SSID"
#define STAPSK  "Your wifi password"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

void handleRoot() {
  String html ="<!DOCTYPE html> <html> <head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <style> .slidecontainer { width: 100%; } .slider { -webkit-appearance: none; width: 100%; height: 50px; background: #d3d3d3; outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s; } .slider:hover { opacity: 1; } .slider::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 35px; height: 50px; background: purple; cursor: pointer; } .slider::-moz-range-thumb { width: 35px; height: 50px; background: purple; cursor: pointer; } </style> </head> <body> <h1 style=\"text-align: center;\">PWM control panel using esp8266</h1> <p>Drag the slider to change the PWM percentage.</p> <div class=\"slidecontainer\"> <input type=\"range\" min=\"0\" max=\"100\" value=\"0\" class=\"slider\" id=\"myRange\"> <p>Value: %<span id=\"demo\"></span></p> <p>response: <span id=\"resp\"></span></p> </div> <script> setInterval(function(){ document.getElementById('resp').innerHTML = ''; }, 4000); var slider = document.getElementById(\"myRange\"); var output = document.getElementById(\"demo\"); output.innerHTML = slider.value; slider.oninput = function() { output.innerHTML = this.value; var xhr = new XMLHttpRequest(); xhr.open('post', '/pwm'); xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded'); xhr.send('percent=' + this.value); xhr.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById('resp').visibility = 'visible'; document.getElementById(\"resp\").innerHTML = this.responseText; } }; } </script> </body> </html>";
  server.send(200, "text/html", html);
}

void handlePWM() {
  int value = (server.arg("percent")).toInt();
  // do some PWM with this value :
    //In here ...
  //On success : 
  digitalWrite(2, HIGH);
  server.send(200, "text/plain", "ok");
  delay(100);
  digitalWrite(2, LOW);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/pwm", handlePWM);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
