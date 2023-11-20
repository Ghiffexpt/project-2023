class processingData {

  public:

    unsigned long timeoutW = 1020;
    
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

    unsigned long SpeedG(unsigned long interval) {
      unsigned long hasil = interval * 3;
      return hasil;
    }

    unsigned long SpeedW(unsigned long interval) {
      unsigned long hasil = interval * 0.5;
      return hasil;
    }

    unsigned long ProcessingDataWired(int inputValue) {
      int outputValue = 1000 - inputValue;
      unsigned long interval;
      if (inputValue < 10 || outputValue > 1000) {
        interval = 1001;
      } else interval = static_cast<unsigned long>(outputValue);
      return interval;
    }

    unsigned long ProcessingDataGuide(int inputValue) {
      int outputValue = 1000 - inputValue;
      unsigned long interval;
      if (inputValue < 10 || outputValue > 1000) {
        interval = 1001;
      } else interval = static_cast<unsigned long>(outputValue);
      return interval;
    }

    void readData() {
      if (startsChar(data, "SW")) {
        int interval  = atoi(data.substring(2).c_str());
        intervalW = ProcessingDataWired(interval);
        Serial.print("SW");
        Serial.println(intervalW);
      } else if (startsChar(data, "MW")) {
        statdir1 = atoi(data.substring(2).c_str());
        myCntrl.CntrlDirW(statdir1);
        Serial.print("MW");
        Serial.println(statdir1);
      } else if (startsChar(data, "MG")) {
        statdir2 = atoi(data.substring(2).c_str());
        myCntrl.CntrlDirG(statdir2);
        Serial.print("MG");
        Serial.println(statdir2);
      } else if (data == "AL") {
        digitalWrite(alarmAction, 1);
        delay(1000);
        digitalWrite(alarmAction, 0);
        mulai_reset();
      } else if (startsChar(data, "SG")) {
        startG = 1;
        sendR = 1;
        sendL = 1;
        int interval = atoi(data.substring(2).c_str());
        intervalG = ProcessingDataGuide(interval);
        Serial.print("SG");
        Serial.println(intervalG);
      }
      data = "";
    }

    void startSmooth() {
      if (intervalW < timeoutW) {
        if (currentMillis - millisW >= 10) {
          timeoutW -= 50;
          myCntrl.fixSpeedW = SpeedW(timeoutW);
          millisW = currentMillis;
        }
      } else if (intervalW > timeoutW) {
        if (currentMillis - millisW >= 10) {
          timeoutW += 50;
          myCntrl.fixSpeedW = SpeedW(timeoutW);

          millisW = currentMillis;
        }
      }
    }

    void stopSmooth() {
      millisW = currentMillis;
      if (1020 > timeoutW) {
        if (currentMillis - millisStopW >= 10) {
          timeoutW += 50;
          myCntrl.fixSpeedW = SpeedW(timeoutW);
          millisStopW = currentMillis;
        }
      }
      if (timeoutW > 1019)stopW = 1;
    }
};
