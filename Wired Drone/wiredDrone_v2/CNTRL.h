#include "Dec.h"
#include "processingData.h"

void(*mulai_reset) (void) = 0;

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
void alarm() {
  int alarmStat = digitalRead(alarmRead);
  if (alarmStat == 0 && alarmSend == 1) {
    Serial.println("Alarm activate!!");
    alarmSend = 0;
    interval1 = 1001;
    interval2 = 1001;
  } else if (alarmStat == 1)alarmSend = 1;

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
void readData() {
  if (startsChar(data, "SW")) {
    int interval  = atoi(data.substring(2).c_str());
    ProcessingDataWired(interval);
    Serial.print("SW");
    Serial.println(interval1);
  } else if (startsChar(data, "MW")) {
    statdir1 = atoi(data.substring(2).c_str());
    digitalWrite(dir1, statdir1);
    Serial.print("MW");
    Serial.println(statdir1);
  } else if (startsChar(data, "MG")) {
    statdir2 = atoi(data.substring(2).c_str());
    digitalWrite(dir2, statdir2);
    Serial.print("MG");
    Serial.println(statdir2);
  } else if (data == "AL") {
    digitalWrite(alarmAction, 1);
    delay(2000);
    digitalWrite(alarmAction, 0);
    mulai_reset();
  } else if (startsChar(data, "SG")) {
    dirext = 1;
    sendR = 1;
    sendL = 1;
    //    extSG = 1;
    int interval = atoi(data.substring(2).c_str());
    ProcessingDataGuide(interval);
    Serial.print("SG");
    Serial.println(interval2);
  }
  data = "";
}

void startSmooth() {
  if (interval1 < timeoutW) {
    if (currentMillis - millisW >= 10) {
      timeoutW -= 10;
      ProcessSpeedW(timeoutW);
      millisW = currentMillis;
    }
  } else if (interval1 < timeoutW) {
    if (currentMillis - millisW >= 10) {
      timeoutW += 10;
      ProcessSpeedW(timeoutW);
      millisW = currentMillis;
    }
  }
}
void stopSmooth() {
  if (1010 > timeoutW) {
    if (currentMillis - millisW >= 5) {
      timeoutW += 10;
      ProcessSpeedW(timeoutW);
      millisW = currentMillis;
    }
  }
  if (timeoutW == 1010)stopW = 1;
}
void CheckLimit() {
  if (statdir2 == 0) {
    if (limitR == 1 ) {
      dirext = 0;
      if (sendR == 1) {
        Serial.println("R1");
        sendR = 0;
      }
    }
  } else if (statdir2 == 1) {
    if (limitL == 1 ) {
      dirext = 0;
      if (sendL == 1) {
        Serial.println("L1");
        sendL = 0;
      }
    }
  }
}

void motorW() {
  if (currentMicros - previousMillis1 >= fixSpeedW) {
    ext1 = !ext1;
    digitalWrite(step1, ext1);
    previousMillis1 = currentMicros; // Perbarui waktu sebelumnya
  }
}
void motorG() {

  if (currentMicros - previousMillis2 >= fixSpeedG) {
    ext2 = !ext2;
    digitalWrite(step2, ext2);
    previousMillis2 = currentMicros; // Perbarui waktu sebelumnya
  }

}
