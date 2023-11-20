/* Encoder Library - TwoKnobs Example
   http://www.pjrc.com/teensy/td_libs_Encoder.html

   This example code is in the public domain.
*/

#include <Encoder.h>
#include "HX711.h"

HX711 scale(13, 4);

Encoder knob(3, 2);

unsigned long currentMillis;
unsigned long timerWrite;
unsigned long timerRead;

float fixData;
float calibration_factor = 1000; // this calibration factor is adjusted according to my load cell
float units;
float ounces;
float avarage;

uint8_t readyAct;
uint8_t counter;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;

volatile unsigned long millisValue = 0;

long newLeft, newRight;
long New , lastBouncing, lastStat;

String Value, recv;

void setup() {
  Serial.begin(9600);
  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
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
  New = knob.read();
  if (stringComplete) {
    if (startsChar(inputString, "vb")) {
      if (New > lastBouncing && New > lastStat) {
        Value = "+";
        lastBouncing = New - 1000;
      } else if (New < lastBouncing && New < lastStat) {
        Value = "-";
        lastBouncing = New + 1000;
      }
      Serial.print("v" + Value);
      Serial.println(units);
      Serial.print("R");
      Serial.println(New);
      lastStat = New;
    }
    inputString = "";
    stringComplete = false;
  }
  //    knobRight.write(0);


  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  units = scale.get_units(), 1;

  if (units < 0)
  {
    units = 0.00;
  }


}
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
