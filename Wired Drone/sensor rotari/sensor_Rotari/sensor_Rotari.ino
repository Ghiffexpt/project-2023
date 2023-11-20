/* Encoder Library - TwoKnobs Example
   http://www.pjrc.com/teensy/td_libs_Encoder.html

   This example code is in the public domain.
*/

#include <Encoder.h>

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder knobLeft(3,2);
//Encoder knobRight(3, 10);
//   avoid using pins with LEDs attached
volatile unsigned long millisG = 0;
long New, newRight, lastBouncing, lastStat;
uint8_t out;
String recv;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  inputString.reserve(200);
}

long positionLeft  = -999;
long positionRight = -999;

bool startsChar(const String &str, const String &prefix) {
      if (str.length() < prefix.length()) {
        return false;
      }

      for (int i = 0; i < prefix.length(); i++) {
        if (str.charAt(i) != prefix.charAt(i)) {
          return false;
        }
      }

      return true; // Semua karakter cocok
    }
    
void loop() {
  unsigned long now = millis();
  New = knobLeft.read();

  if (stringComplete) {
    if (startsChar(inputString, "gd")) {
      Serial.print("G");
      Serial.println(New);
    }
    inputString = "";
    stringComplete = false;
  }
  //    knobRight.write(0);
}
//  delayMicroseconds(10000);

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
