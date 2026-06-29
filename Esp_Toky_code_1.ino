
#include <WiFi.h>
#include <WebServer.h>

// =========================================================================
// NETWORK CONFIGURATION SETTING ARRAYS
// =========================================================================
const char* ssid     = "YOUR_WIFI_SSID";      // <--- Replace with your Wi-Fi Name
const char* password = "YOUR_WIFI_PASSWORD";  // <--- Replace with your Wi-Fi Password

// Instantiate the local web server layer using native TCP port 80
WebServer server(80);

// Global operational buffer string to preserve message arrays across clock cycles
String globalDataMessage = "Awaiting initial text stream transmission from ESP32 Serial Monitor...";

// =========================================================================
// WEB SERVER ENDPOINT HANDLERS (DATA STREAMING PIPELINES)
// =========================================================================

/**
 * Executes whenever the web application targets the 'http://<IP_ADDRESS>/data' endpoint.
 * Serves the current serial text buffer as plain text wrapped in security clearance parameters.
 */
void handleDataRoute() {
  // CRITICAL LAYER: Explicit CORS rules required to allow remote or external scripts (Netlify, Localhost)
  // to read this local hardware API response stream without browser firewall rejections.
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "*");
  
  // Respond with standard HTTP status code 200 OK and append data payload
  server.send(200, "text/plain", globalDataMessage);
  
  Serial.print("[Web Interface Sync] -> Data array pulled by host app: \"");
  Serial.print(globalDataMessage);
  Serial.println("\"");
}

/**
 * Catches invalid requests or options pre-flight validation scans elegantly.
 */
void handleNotFoundRoute() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(404, "text/plain", "Error 404: Endpoint Route Unmapped");
}

// =========================================================================
// SYSTEM INITIALIZATION CORE ARCHITECTURE (SETUP)
// =========================================================================
void setup() {
  // Initialize communication bus line at standard 115200 baud profile rate
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n================================================");
  Serial.println("   ESP TOKY - HARDWARE NODE INTERFACE SYSTEM");
  Serial.println("================================================");
  Serial.print("Connecting to target RF Access Point: ");
  Serial.println(ssid);

  // Initialize background network card configurations
  WiFi.begin(ssid, password);

  // Pool network status registers until structural handshake processes resolve
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("■"); // Connection progress metrics tracking
  }

  // Network connection parameters established successfully
  Serial.println("\n\n>>> Wi-Fi Layer Link Established!");
  Serial.print(">>> Hardware Node MAC Address: ");
  Serial.println(WiFi.macAddress());
  
  // -----------------------------------------------------------------------
  // TARGET OPERATIONAL IP ADDRESS DEPLOYMENT (COPY THIS INTO YOUR WEB APP)
  // -----------------------------------------------------------------------
  Serial.println("\n------------------------------------------------");
  Serial.print("  >>> TARGET WEB APP NODE IP: ");
  Serial.println(WiFi.localIP()); 
  Serial.println("------------------------------------------------\n");
  
  Serial.println("Ready for local operations.");
  Serial.println("Action: Type text message inside line input above and press SEND/ENTER:");
  Serial.println("===============================================================");

  // Define hardware system server routes mapping definitions
  server.on("/data", HTTP_GET, handleDataRoute);
  server.onNotFound(handleNotFoundRoute);
  
  // Power up listener hooks
  server.begin();
  Serial.println("[System Ready] HTTP Pipeline Server listening on Port 80.");
}

// =========================================================================
// RUNTIME PROCESSING SYSTEM EXECUTION ENGINE (LOOP)
// =========================================================================
void loop() {
  // Execute system polling hooks to dispatch server requests accurately
  server.handleClient();

  // Monitor Serial input buffers for new message packets
  if (Serial.available() > 0) {
    // Read trailing buffer string up until a Newline execution token delimiter is hit
    String inboundDataString = Serial.readStringUntil('\n');
    
    // Wipe standard system control symbols (Carriage Returns \r, whitespaces)
    inboundDataString.trim();
    
    // Verify payload validation check parameters
    if (inboundDataString.length() > 0) {
      globalDataMessage = inboundDataString;
      
      // Output system status update tracking metrics to visual log console
      Serial.print("[System Memory Matrix Updated] -> Current Message: \"");
      Serial.print(globalDataMessage);
      Serial.println("\"");
    }
  }
}
