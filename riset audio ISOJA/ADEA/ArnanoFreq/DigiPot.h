
void pinDigiPot(uint8_t inc, uint8_t ud, uint8_t cs1, uint8_t cs2);
void reset1();
void setDigiPot(uint8_t val);
void extDigiPot1(uint8_t direction, uint8_t amount);
void reset2();
void setDigiPot2(uint8_t val);
void extDigiPot2(uint8_t direction, uint8_t amount);

void pinDigiPot(uint8_t inc, uint8_t ud, uint8_t cs1, uint8_t cs2) {
  //set pin DigiPot
  incPin = inc;
  udPin = ud;
  cs2Pin = cs2;
  cs1Pin = cs1;

  pinMode(incPin, OUTPUT);
  pinMode(udPin, OUTPUT);
  pinMode(cs1Pin, OUTPUT);
  digitalWrite(cs1Pin, HIGH);
  pinMode(cs2Pin, OUTPUT);
  digitalWrite(cs2Pin, HIGH);
}
void reset1() {
  // reset digipot menjadi posisi 0
  uint8_t  amount = constrain(DIGIPOT_MAX_AMOUNT, 0, DIGIPOT_MAX_AMOUNT);
  extDigiPot1(DIGIPOT_DOWN, amount);
  currentValue1 = 0;
}
void setDigiPot1(uint8_t val) {
  //set digipot
  val = constrain(val, 0, DIGIPOT_MAX_AMOUNT);
  if (currentValue1 == DIGIPOT_UNKNOWN) reset1();
  if (currentValue1 > val) {
    extDigiPot1(DIGIPOT_DOWN, currentValue1 - val);
  } else if (currentValue1 < val) {
    extDigiPot1(DIGIPOT_UP, val - currentValue1);
  }

}

void extDigiPot1(uint8_t direction, uint8_t amount) {
  amount = constrain(amount, 0, DIGIPOT_MAX_AMOUNT);
  digitalWrite(udPin, direction);
  digitalWrite(incPin, HIGH);
  digitalWrite(cs1Pin, LOW);

  for (uint8_t i = 0; i < amount; i++) {
    digitalWrite(incPin, LOW);
    delayMicroseconds(2);
    digitalWrite(incPin, HIGH);
    delayMicroseconds(2);
    if (currentValue1 != DIGIPOT_UNKNOWN) {
      currentValue1 += (direction == DIGIPOT_UP ? 1 : -1);
      currentValue1 = constrain(currentValue1, 0, DIGIPOT_MAX_AMOUNT);
    }

  }
  digitalWrite(cs1Pin, HIGH);
}


void reset2() {
  // reset digipot menjadi posisi 0
  uint8_t  amount = constrain(DIGIPOT_MAX_AMOUNT, 0, DIGIPOT_MAX_AMOUNT);
  extDigiPot2(DIGIPOT_DOWN, amount);
  currentValue2 = 0;
}
void setDigiPot2(uint8_t val) {
  //set digipot
  val = constrain(val, 0, DIGIPOT_MAX_AMOUNT);
  if (currentValue2 == DIGIPOT_UNKNOWN) reset2();
  if (currentValue2 > val) {
    extDigiPot2(DIGIPOT_DOWN, currentValue2 - val);
  } else if (currentValue2 < val) {
    extDigiPot2(DIGIPOT_UP, val - currentValue2);
  }

}

void extDigiPot2(uint8_t direction, uint8_t amount) {
  amount = constrain(amount, 0, DIGIPOT_MAX_AMOUNT);
  digitalWrite(udPin, direction);
  digitalWrite(incPin, HIGH);
  digitalWrite(cs2Pin, LOW);

  for (uint8_t i = 0; i < amount; i++) {
    digitalWrite(incPin, LOW);
    delayMicroseconds(2);
    digitalWrite(incPin, HIGH);
    delayMicroseconds(2);
    if (currentValue2 != DIGIPOT_UNKNOWN) {
      currentValue2 += (direction == DIGIPOT_UP ? 1 : -1);
      currentValue2 = constrain(currentValue2, 0, DIGIPOT_MAX_AMOUNT);
    }

  }
  digitalWrite(cs2Pin, HIGH);
}
