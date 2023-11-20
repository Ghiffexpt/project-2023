#include "gipMillis.h"
#include "CNTRL.h"

void setup() {
  // Konfigurasi Timer 1 sesuai kebutuhan Anda
  Serial.begin(9600);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(pinR, INPUT_PULLUP);
  pinMode(pinL, INPUT_PULLUP);
  SetUpMillis();
}

void loop() {
  currentMillis = micros(); // Dapatkan waktu saat ini
  if (Serial.available()) {
    char a = Serial.read();
    if (a != '\n') {
      data += a;
    } else {
      readData();
    }
  }
  //  if (extSG == 1) {
  //    smooth();
  //  }
  if (interval1 <= 1000) {
    ProcessSpeedW();
    motorW();
  }
  if (interval2 <= 1000) {
    CheckLimit();
    if (dirext == 1) {
      ProcessSpeedG();
      motorG();
    }
  }

  if (currentMillis - MillisReadLimit >= 500) {

    readLimit();

    MillisReadLimit = currentMillis; // Perbarui waktu sebelumnya
  }
}
