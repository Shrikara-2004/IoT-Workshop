#include <WiFi.h>

// WiFi credentials
const char* ssid = "IQOO z9s";
const char* password = "12345@ggggmmm";

// Web server
WiFiServer server(80);

// Pins
const int trigPin = 7;
const int echoPin = 8;
const int output26 = 5;    // Smart Switch 1
const int output27 = 17;   // Smart Switch 2
const int buzzerPin = 6;  // Buzzer

// States
String header;
String output26State = "off";
String output27State = "off";
String buzzerState = "off";

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);
  digitalWrite(buzzerPin, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // AJAX request for distance
            if (header.indexOf("GET /distance") >= 0) {
              int dist = measureDistance();
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/plain");
              client.println("Connection: close");
              client.println();
              client.println(dist);
              break;
            }

            // GPIO Controls
            if (header.indexOf("GET /26/on") >= 0) {
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              output26State = "off";
              digitalWrite(output26, LOW);
            }

            if (header.indexOf("GET /27/on") >= 0) {
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              output27State = "off";
              digitalWrite(output27, LOW);
            }

            if (header.indexOf("GET /buzzer/on") >= 0) {
              buzzerState = "on";
              digitalWrite(buzzerPin, HIGH);
            } else if (header.indexOf("GET /buzzer/off") >= 0) {
              buzzerState = "off";
              digitalWrite(buzzerPin, LOW);
            }

            // HTML Page
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!DOCTYPE html><html><head>");
            client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
            client.println("<style>body{text-align:center;font-family:sans-serif;} .button{padding:15px;margin:5px;font-size:18px;}</style>");
            client.println("<script>");
            client.println("setInterval(function() { fetch('/distance').then(res => res.text()).then(d => { document.getElementById('dist').innerText = d + ' cm'; }); }, 1000);");
            client.println("</script></head><body>");
            client.println("<h1>DLithe - Smart Switch Dashboard</h1>");
            client.println("<h2>Ultrasonic Sensor Distance: <span id='dist'>-- cm</span></h2>");

            // GPIO 26
            client.println("<p>GPIO 26 - State: " + output26State + "</p>");
            client.println("<p><a href='/26/" + String((output26State == "off") ? "on" : "off") + "'><button class='button'>Turn " + String((output26State == "off") ? "ON" : "OFF") + "</button></a></p>");

            // GPIO 27
            client.println("<p>GPIO 27 - State: " + output27State + "</p>");
            client.println("<p><a href='/27/" + String((output27State == "off") ? "on" : "off") + "'><button class='button'>Turn " + String((output27State == "off") ? "ON" : "OFF") + "</button></a></p>");

            // Buzzer
            client.println("<p>Buzzer - State: " + buzzerState + "</p>");
            client.println("<p><a href='/buzzer/" + String((buzzerState == "off") ? "on" : "off") + "'><button class='button'>Turn " + String((buzzerState == "off") ? "ON" : "OFF") + "</button></a></p>");

            client.println("</body></html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();
    Serial.println("Client disconnected.\n");
  }
}