#include "CNTRL.h"

void setup() {
  // Konfigurasi Timer 1 sesuai kebutuhan Anda
  Serial.begin(9600);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(alarmAction, OUTPUT);
  pinMode(alarmRead, INPUT);
  pinMode(pinR, INPUT_PULLUP);
  pinMode(pinL, INPUT_PULLUP);
}

void loop() {
  currentMicros = micros(); // Dapatkan waktu saat ini
  currentMillis = millis();
  alarm();
  if (Serial.available()) {
    char a = Serial.read();
    if (a != '\n') {
      data += a;
    } else {
      readData();
    }
  }
  if (startW == 1) {
    startSmooth();
    motorW();
  }
  if (stopW == 1) {
    startW = 0; 
    interval1 = 1002;
    timeoutW = 1010;
    stopW = 0;

  }
  if (interval1 == 1001)stopSmooth();
  if (interval1 <= 1000)startW = 1;

  if (interval2 <= 1000) {
    CheckLimit();
    if (dirext == 1) {
      ProcessSpeedG();
      motorG();
    }
  }

  if (currentMicros - MillisReadLimit >= 500) {

    readLimit();

    MillisReadLimit = currentMicros; // Perbarui waktu sebelumnya
  }
}
