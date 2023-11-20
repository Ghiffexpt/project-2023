/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-web-server-physical-button/

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "Love u";
const char* password = "admin.admin";

const char* PARAM_INPUT_1 = "state";
const char* PARAM_INPUT_2 = "input";

const int output = 2;
const int buttonPin = 4;

// Variables will change:
int ledState = LOW;          // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
 @import url("https://fonts.googleapis.com/css?family=Raleway");

:root {
  --glow-color: hsl(186 100% 69%);
}

*,
*::before,
*::after {
  box-sizing: border-box;
}

html,
body {
  height: 100%;
  width: 100%;
  overflow: hidden;
}

body {
  min-height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
  background: black;
}

.glowing-btn {
  position: relative;
  color: var(--glow-color);
  cursor: pointer;
  padding: 0.35em 1em;
  border: 0.15em solid var(--glow-color);
  border-radius: 0.45em;
  background: none;
  perspective: 2em;
  font-family: "Raleway", sans-serif;
  font-size: 2em;
  font-weight: 900;
  letter-spacing: 1em;

  -webkit-box-shadow: inset 0px 0px 0.5em 0px var(--glow-color),
    0px 0px 0.5em 0px var(--glow-color);
  -moz-box-shadow: inset 0px 0px 0.5em 0px var(--glow-color),
    0px 0px 0.5em 0px var(--glow-color);
  box-shadow: inset 0px 0px 0.5em 0px var(--glow-color),
    0px 0px 0.5em 0px var(--glow-color);
  animation: border-flicker 2s linear infinite;
}

.glowing-txt {
  float: left;
  margin-right: -0.8em;
  -webkit-text-shadow: 0 0 0.125em hsl(0 0% 100% / 0.3),
    0 0 0.45em var(--glow-color);
  -moz-text-shadow: 0 0 0.125em hsl(0 0% 100% / 0.3),
    0 0 0.45em var(--glow-color);
  text-shadow: 0 0 0.125em hsl(0 0% 100% / 0.3), 0 0 0.45em var(--glow-color);
  animation: text-flicker 3s linear infinite;
}

.faulty-letter {
  opacity: 0.5;
  animation: faulty-flicker 2s linear infinite;
}

.glowing-btn::before {
  content: "";
  position: absolute;
  top: 0;
  bottom: 0;
  left: 0;
  right: 0;
  opacity: 0.7;
  filter: blur(1em);
  transform: translateY(120%) rotateX(95deg) scale(1, 0.35);
  background: var(--glow-color);
  pointer-events: none;
}

.glowing-btn::after {
  content: "";
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  opacity: 0;
  z-index: -1;
  background-color: var(--glow-color);
  box-shadow: 0 0 2em 0.2em var(--glow-color);
  transition: opacity 100ms linear;
}

.glowing-btn:hover {
  color: rgba(0, 0, 0, 0.8);
  text-shadow: none;
  animation: none;
}

.glowing-btn:hover .glowing-txt {
  animation: none;
}

.glowing-btn:hover .faulty-letter {
  animation: none;
  text-shadow: none;
  opacity: 1;
}

.glowing-btn:hover:before {
  filter: blur(1.5em);
  opacity: 1;
}

.glowing-btn:hover:after {
  opacity: 1;
}

@keyframes faulty-flicker {
  0% {
    opacity: 0.1;
  }
  2% {
    opacity: 0.1;
  }
  4% {
    opacity: 0.5;
  }
  19% {
    opacity: 0.5;
  }
  21% {
    opacity: 0.1;
  }
  23% {
    opacity: 1;
  }
  80% {
    opacity: 0.5;
  }
  83% {
    opacity: 0.4;
  }

  87% {
    opacity: 1;
  }
}

@keyframes text-flicker {
  0% {
    opacity: 0.1;
  }

  2% {
    opacity: 1;
  }

  8% {
    opacity: 0.1;
  }

  9% {
    opacity: 1;
  }

  12% {
    opacity: 0.1;
  }
  20% {
    opacity: 1;
  }
  25% {
    opacity: 0.3;
  }
  30% {
    opacity: 1;
  }

  70% {
    opacity: 0.7;
  }
  72% {
    opacity: 0.2;
  }

  77% {
    opacity: 0.9;
  }
  100% {
    opacity: 0.9;
  }
}

@keyframes border-flicker {
  0% {
    opacity: 0.1;
  }
  2% {
    opacity: 1;
  }
  4% {
    opacity: 0.1;
  }

  8% {
    opacity: 1;
  }
  70% {
    opacity: 0.7;
  }
  100% {
    opacity: 1;
  }
}

@media only screen and (max-width: 600px) {
  .glowing-btn{
    font-size: 1em;
  }
}
  </style>
</head>
<body>

  <h2>ISOJA</h2>
  
  <form action="/get">
    input1: <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br>
  
<button class='glowing-btn'onclick="cntrlISOJA(1)"><span class='glowing-txt'><span class='faulty-letter'>Bluetooth Stream</span></span></button>

<script>
var Volume = 0;
function cntrlISOJA(element) {
  var xhr = new XMLHttpRequest();
  switch(element){
        case 1:
            xhr.open("GET", "/get?state=-1)", true); 
            xhr.send();
            break;
        case 2:
            xhr.open("GET", "/get?state=-2)", true); 
            xhr.send();
        case 3:
            xhr.open("GET", "/get?state=-3)", true); 
            xhr.send();
            break;
        case 4:
            xhr.open("GET", "/get?state=-4)", true); 
            xhr.send();
            break;
        case 5:
            xhr.open("GET", "/get?state=-5)", true); 
            xhr.send();
            break;
        case 6:
            xhr.open("GET", "/get?state=-8)", true); 
            xhr.send();
            break;  
        case 7:
            Vol(Volume+1);
            break;   
        case 8:
            Vol(Volume-1);
            break;              
            default:
                break;
    }  
}
function Vol(stat){
    var xhr = new XMLHttpRequest();
    switch(stat){
        case 10:
            xhr.open("GET", "/get?state=-71)", true); 
            xhr.send();
            break;
        case 9:
            xhr.open("GET", "/get?state=-70.9)", true); 
            xhr.send();
            break;
        case 8:
            xhr.open("GET", "/get?state=-70.8)", true); 
            xhr.send();
            break;
        case 7:
            xhr.open("GET", "/get?state=-70.7)", true); 
            xhr.send();
            break;
        case 6:
            xhr.open("GET", "/get?state=-70.6)", true); 
            xhr.send();
            break;
        case 5:
            xhr.open("GET", "/get?state=-70.5)", true); 
            xhr.send();
            break;
        case 4:
            xhr.open("GET", "/get?state=-70.4)", true); 
            xhr.send();
            break;
        case 3:
            xhr.open("GET", "/get?state=-70.3)", true); 
            xhr.send();
            break;
        case 2:
            xhr.open("GET", "/get?state=-70.2)", true); 
            xhr.send();
            break;
        case 1:
            xhr.open("GET", "/get?state=-70.1)", true); 
            xhr.send();
            break;
        case 0:
            xhr.open("GET", "/get?state=-70.0)", true); 
            xhr.send();
            break;
        default:
            console.log(stat);
            // message = new Paho.MQTT.Message("Suhu AC tidak terdeteksi");
            // message.destinationName = topic_sharp;
            // client.send(message);
            window.alert("Volume diluar rentang!");
            break;
    } 
}
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page



void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  pinMode(buttonPin, INPUT);
  
  // Connect to Wi-Fi
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      if(inputMessage.lastIndexOf('&')||inputMessage.lastIndexOf(')')||inputMessage.lastIndexOf('+')||inputMessage.lastIndexOf('%')){
         Serial.println(inputMessage);
      } 
      if(inputMessage.lastIndexOf(';')){
         Serial2.println(inputMessage);
      }
    }else  if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      String a  ="";
             a +="-/";
             a +=inputMessage;
             a +="&"; 
      Serial.println(a);
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
//    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/state
  server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(digitalRead(output)).c_str());
  });
  // Start server
  server.begin();
}
  
void loop() {
  // read the state of the switch into a local variable:
//  int reading = digitalRead(buttonPin);
//
//  // check to see if you just pressed the button
//  // (i.e. the input went from LOW to HIGH), and you've waited long enough
//  // since the last press to ignore any noise:
//
//  // If the switch changed, due to noise or pressing:
//  if (reading != lastButtonState) {
//    // reset the debouncing timer
//    lastDebounceTime = millis();
//  }
//
//  if ((millis() - lastDebounceTime) > debounceDelay) {
//    // whatever the reading is at, it's been there for longer than the debounce
//    // delay, so take it as the actual current state:
//
//    // if the button state has changed:
//    if (reading != buttonState) {
//      buttonState = reading;
//
//      // only toggle the LED if the new button state is HIGH
//      if (buttonState == HIGH) {
//        ledState = !ledState;
//      }
//    }
//  }
//
//  // set the LED:
//  digitalWrite(output, ledState);
//
//  // save the reading. Next time through the loop, it'll be the lastButtonState:
//  lastButtonState = reading;
}
