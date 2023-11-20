#include "CNTRL.h"
Cntrl myCntrl(5, 2, 6, 3);
#include "processingData.h"
processingData procData;
#include "CheckStat.h"


void setup() {
  // Konfigurasi Timer 1 sesuai kebutuhan Anda
  Serial.begin(9600);
  pinMode(alarmAction, OUTPUT);
  pinMode(alarmRead, INPUT);
  pinMode(pinR, INPUT_PULLUP);
  pinMode(pinL, INPUT_PULLUP);
  pinMode(10, INPUT);
  inputString.reserve(200);

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
void loop() {

  currentMicrosW = micros(); // Dapatkan waktu saat ini
  currentMicrosG = micros(); // Dapatkan waktu saat ini

  currentMillis = millis();
  //  if (Serial.available()) {
  //    char a = Serial.read();
  //    if (a != '\n') {
  //      data += a;
  //    } else {
  //      procData.readData();
  //    }
  //  }
  if (stringComplete) {
    data = inputString;
    inputString = "";
    procData.readData();
    stringComplete = false;
  }
  checkIntervalWired();
  checkIntervalGuide();

  if (startG == 1) {
    myCntrl.fixSpeedG = procData.SpeedG(intervalG);
    myCntrl.motorG();
  }

  if (startW == 1) {
    //    millisStopW = currentMillis;
    procData.startSmooth();
    myCntrl.motorW();
  }
  if (stopW == 1) {
    intervalW = 1002;
    startW = 0;
    procData.timeoutW = 1020;
    millisW = currentMillis;
    stopW = 0;

  }

  if (startW == 1 || startG == 1) {
    if (currentMillis - MillisReadLimit >= 100) {

      readLimit();
      alarm();
      MillisReadLimit = currentMillis; // Perbarui waktu sebelumnya
    }
  }
}
