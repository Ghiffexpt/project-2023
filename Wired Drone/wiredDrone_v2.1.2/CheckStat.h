
void alarm() {
  int alarmStat = digitalRead(10);
  if (alarmStat == 1) {
    Serial.println("Alarm activate!!");
  }

}

void readLimit() {
  uint8_t R = digitalRead(pinR);
  uint8_t L = digitalRead(pinL);
  if (R == 0) {
    limitR = 1;
  } else if (R == 1) {
    sendR = 1;
    limitR = 0;
  }
  if (L == 0) {
    limitL = 1;
  } else if (L == 1) {
    sendL = 1;
    limitL = 0;
  }
}

void CheckLimit() {
  if (statdir2 == 0) {
    if (limitR == 1 ) {
      startG = 0;
      if (sendR == 1) {
        Serial.println("R1");
        sendR = 0;
      }
    }
  } else if (statdir2 == 1) {
    if (limitL == 1 ) {
      startG = 0;
      if (sendL == 1) {
        Serial.println("L1");
        sendL = 0;
      }
    }
  }
}

void checkIntervalWired() {
  if (intervalW <= 1000)startW = 1;
  if (intervalW == 1001)procData.stopSmooth();
}

void checkIntervalGuide() {
  if (intervalG <= 1000)CheckLimit();
  if (intervalG > 1000) startG = 0;
}
