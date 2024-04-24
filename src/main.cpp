#include <Arduino.h>
#include <WiFi.h>

const char* ssid     = "YourESP32AP";
const char* password = "password123";
WiFiServer server(80);                 // Set web server port to 80

const int ledPin = 2; // GPIO pin connected to the LED
bool ledState = false; // Current state of the LED

void setup() {
  Serial.begin(115200);
  
  pinMode(ledPin, OUTPUT);
  
  // Set up ESP32 as Access Point
  WiFi.softAP(ssid, password);
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available(); // Check for a client

  if (client) {
    Serial.println("New Client.");        // If there is a client,
    String currentLine = "";              // make a String to hold incoming data from the client
    while (client.connected()) {          // loop while the client's connected
      if (client.available()) {           // if there's bytes to read from the client,
        char c = client.read();           // read a byte, then
        Serial.write(c);                   // print it out the serial monitor
        if (c == '\n') {                   // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            // the content of the HTTP response follows the header:
            client.println("<html><body><h1>Hello, world!</h1>");
            client.println("<button onclick=\"window.location.href='/on'\">Turn LED On</button>");
            client.println("<button onclick=\"window.location.href='/off'\">Turn LED Off</button>");
            client.println("</body></html>");
            // The HTTP response ends with another blank line:
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    currentLine = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
  }
}

void turnLedOn() {
  digitalWrite(ledPin, HIGH);
  ledState = true;
}

void turnLedOff() {
  digitalWrite(ledPin, LOW);
  ledState = false;
}

void hello(){
  Serial.println("hello world");
}