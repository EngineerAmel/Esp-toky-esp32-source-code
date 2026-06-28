#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

String message = "Hello from ESP32";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n");
  Serial.println("WiFi Connected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("--------------------------------");
  Serial.println("Type a message in Serial Monitor");
  Serial.println("Press Enter to update /data");
  Serial.println("--------------------------------");

  server.on("/data", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", message);
  });

  server.begin();

  Serial.println("Web Server Started");
  Serial.print("Open: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/data");
}

void loop() {

  server.handleClient();

  // Read text from Serial Monitor
  if (Serial.available()) {

    message = Serial.readStringUntil('\n');
    message.trim();

    Serial.print("New message set: ");
    Serial.println(message);
  }
}