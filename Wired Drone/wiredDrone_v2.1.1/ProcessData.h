void ProcessSpeedG() {
  Speed[0][0] = interval[0][1] * 8.25;
}

void SetUpData() {
  interval[0][0] = 1002;
  interval[0][1] = 1001;
  Serial.println(interval[0][1]);
  interval[1][0] = 1010;
  LimitDirection[1][0] = 1;
  LimitDirection[1][1] = 1;
}

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

void ProcessSpeedW(unsigned long interval) {
  Speed[0][1] = interval * 0.5;
}

void ProcessingDataGuide(int inputValue) {
  int outputValue = 1000 - inputValue;
  if (inputValue < 10 || outputValue == 0) {
    interval[0][1] = 1001;
  } else interval[0][1] = static_cast<unsigned long>(outputValue);
}

void ProcessingDataWired(int inputValue) {
  int outputValue = 1000 - inputValue;
  if (inputValue < 10 || outputValue == 0) {
    interval[0][0] = 1001;
  } else interval[0][0] = static_cast<unsigned long>(outputValue);
}

void readData() {
  if (startsChar(data, "SW")) {
    int dataWired = atoi(data.substring(2).c_str());
    ProcessingDataWired(dataWired);
    Serial.print("SW");
    Serial.println(interval[0][0]);
  } else if (startsChar(data, "MW")) {
    Status[0][0] = atoi(data.substring(2).c_str());
    digitalWrite(dir1, Status[0][0]);
    Serial.print("MW");
    Serial.println(Status[0][0]);
  } else if (startsChar(data, "MG")) {
    Status[0][1] = atoi(data.substring(2).c_str());
    digitalWrite(dir2, Status[0][1]);
    Serial.print("MG");
    Serial.println(Status[0][1]);
  } else if (data == "AL") {
    digitalWrite(pinActAlarm, 1);
    delay(2000);
    digitalWrite(pinActAlarm, 0);
    mulai_reset();
  } else if (startsChar(data, "SG")) {
    LimitDirection[0][2] = 1;
    LimitDirection[1][0] = 1;
    LimitDirection[1][1] = 1;
    int dataGuide = atoi(data.substring(2).c_str());
    ProcessingDataGuide(dataGuide);
    Serial.print("SG");
    Serial.println(interval[0][1]);
  }
  data = "";
}
