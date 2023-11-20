#include "Dec.h"
void(*mulai_reset) (void) = 0;
#include "ProcessData.h"


void alarm() {
  AlarmSend[0][0] = digitalRead(pinStatAlarm);
  if (AlarmSend[0][0] == 0 && AlarmSend[0][1] == 1) {
    Serial.println("Alarm activate!!");
    AlarmSend[0][1] = 0;
  } else if (AlarmSend[0][0] == 1)AlarmSend[0][1] = 1;

}

void readLimit() {
  uint8_t R = digitalRead(pinR);
  uint8_t L = digitalRead(pinL);
  if (R == 0) {
    LimitDirection[0][0] = 1;
  } else if (R == 1) {
    LimitDirection[1][0] = 1;
    LimitDirection[0][0] = 0;
  }
  if (L == 0) {
    LimitDirection[0][1] = 1;
  } else if (L == 1) {
    LimitDirection[1][1] = 1;
    LimitDirection[0][1] = 0;
  }
}

void startSmooth() {
  if (interval[0][0] < interval[1][0]) {
    if (currentMillis - previousMillis[0][2] >= 10) {
      interval[1][0] -= 10;
      ProcessSpeedW(interval[1][0]);
      previousMillis[0][2] = currentMillis;
    }
  } else if (interval[0][0] < interval[1][0]) {
    if (currentMillis - previousMillis[0][2] >= 10) {
      interval[1][0] += 10;
      ProcessSpeedW(interval[1][0]);
      previousMillis[0][2] = currentMillis;
    }
  }
}

void stopSmooth() {
  if (1010 > interval[1][0]) {
    if (currentMillis - previousMillis[0][2] >= 5) {
      interval[1][0] += 10;
      ProcessSpeedW(interval[1][0]);
      previousMillis[0][2] = currentMillis;
    }
  }
  if (interval[1][0] == 1010)startWired[0][0], startWired[0][1] = 0;
}

void CheckLimit() {
  if (Status[0][1] == 0) {
    if (LimitDirection[0][0] == 1 ) {
      LimitDirection[0][2] = 0;
      if (LimitDirection[1][0] == 1) {
        Serial.println("R1");
        LimitDirection[1][0] = 0;
      }
    }
  } else if (Status[0][1] == 1) {
    if (LimitDirection[0][1] == 1 ) {
      LimitDirection[0][2] = 0;
      if (LimitDirection[1][1] == 1) {
        Serial.println("L1");
        LimitDirection[1][1] = 0;
      }
    }
  }
}

void motorW() {
  if (currentMicros - previousMillis[0][0] >= Speed[0][1]) {
    output[0][0] = !output[0][0];
    digitalWrite(step1, output[0][0]);
    previousMillis[0][0] = currentMicros; // Perbarui waktu sebelumnya
  }
}

void motorG() {

  if (currentMicros - previousMillis[0][1] >= Speed[0][0]) {
    output[0][1] = !output[0][1];
    digitalWrite(step2, output[0][1]);
    previousMillis[0][1] = currentMicros; // Perbarui waktu sebelumnya
  }

}
