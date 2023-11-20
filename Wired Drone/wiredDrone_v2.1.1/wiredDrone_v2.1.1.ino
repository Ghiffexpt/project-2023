#include "CNTRL.h"

void setup() {
  // Konfigurasi Timer 1 sesuai kebutuhan Anda
  Serial.begin(9600);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(pinActAlarm, OUTPUT);
  pinMode(pinStatAlarm, INPUT);
  pinMode(pinR, INPUT_PULLUP);
  pinMode(pinL, INPUT_PULLUP);
  SetUpData();
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
  if (startWired[0][0] == 1 && startWired[0][1] == 1) {
    Serial.println(interval[0][0]);
    startSmooth();
    motorW();
  }
  if (startWired[0][0] == 0 && startWired[0][1] == 0) {
    interval[0][0] = 1002;
    interval[1][0] = 1010;
    startWired[0][1] = 1;
  }
  if (interval[0][0] == 1001)stopSmooth();
  if (interval[0][0] <= 1000)startWired[0][0],startWired[0][1] = 1;

  if (interval[0][1] <= 1000) {
    CheckLimit();
    if (LimitDirection[0][2] == 1) {
      ProcessSpeedG();
      motorG();
    }
  }

  if (currentMicros - previousMillis[1][0] >= 500) {

    readLimit();

    previousMillis[1][0] = currentMicros; // Perbarui waktu sebelumnya
  }
}
