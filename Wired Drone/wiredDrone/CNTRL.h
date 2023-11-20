#include "Dec.h"


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
void ProcessSpeedG() {
  fixSpeedG = interval2 * 8.25;
}
void ProcessSpeedW() {
  fixSpeedW = interval1 * 0.5;
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
    interval1 = atoi(data.substring(2).c_str());
    Serial.print("SW");
    Serial.println(interval1);
  } else if (startsChar(data, "MW")) {
    statdir1 = atoi(data.substring(2).c_str());
    Serial.print("MW");
    Serial.println(statdir1);
    digitalWrite(dir1, statdir1);
  } else if (startsChar(data, "MG")) {
    statdir2 = atoi(data.substring(2).c_str());
    Serial.print("MG");
    Serial.println(statdir2);
    digitalWrite(dir2, statdir2);
  } else if (startsChar(data, "AU")) {
    Auto = atoi(data.substring(2).c_str());
    Serial.print("AU");
    Serial.println(Auto);
  } else if (startsChar(data, "SG")) {
    dirext = 1;
    //    extSG = 1;
    interval2 = atoi(data.substring(2).c_str());
    Serial.print("SG");
    Serial.println(interval2);
  }
  data = "";
}

//void smooth() {
//  if (interval1 <= timeoutW) {
//    if (currentMillis - millisW >= 10000) {
//      timeoutW--;
//      ProcessSpeedW(timeoutW);
//      millisW = currentMillis;
//    }
//  } if(interval1 > timeoutW) {
//    i = 15;
//    extSG = 0;
//  }
//}
void CheckLimit() {
  if (statdir2 == 0) {
    if (limitR == 1 ) {
      if (Auto == 1) {
        statdir2 = 1;
        digitalWrite(dir2, statdir2);
        limitR = 0;
      } else {
        dirext = 0;
        if (sendR == 1) {
          Serial.println("R1");
          sendR = 0;
        }
      }
    }
  }

  else if (statdir2 == 1) {
    if (limitL == 1 ) {
      if (Auto == 1) {
        statdir2 = 0;
        digitalWrite(dir2, statdir2);
        limitL = 0;
      } else {
        dirext = 0;
        if (sendL == 1) {
          Serial.println("L1");
          sendL = 0;
        }
      }
    }
  }
}
void motorW() {
  if (currentMillis - previousMillis1 >= fixSpeedW) {
    ext1 = !ext1;
    digitalWrite(dir1, statdir1);
    digitalWrite(step1, ext1);
    previousMillis1 = currentMillis; // Perbarui waktu sebelumnya
  }
}
void motorG() {

  if (currentMillis - previousMillis2 >= fixSpeedG) {
    ext2 = !ext2;
    digitalWrite(step2, ext2);
    previousMillis2 = currentMillis; // Perbarui waktu sebelumnya
  }

}
