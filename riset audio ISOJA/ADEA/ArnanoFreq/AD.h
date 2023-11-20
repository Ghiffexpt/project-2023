typedef struct {
  float    freq[2]; ///<Holds the frequencies of
  float    phase[2];
  float    mod_freq;
  uint8_t  freq_out;
  uint8_t  phase_out;
  uint8_t  mode;
  uint16_t command_reg;
} ad9833_settings_t;
const uint8_t  randomized[] PROGMEM =
{
  /*
    17,21,23,22,25,9,10,9,6,13,20,12,11,10,20,13,19,16,9,15,20,15,5,6,
    25,11,13,10,6,23,12,25,21,11,20,7,6,22,9,25,20,24,19,13,15,14,20,
    9,5,19,13,17,13,13,25,7,20,21,20,17,21,17,13,9,24,20,20,16,21,9,5,
    23,9,11,6,9,16,21,21,24,8,24,24,24,10,9,23,13,9,14,24,25,24,18,6,5,
    17,22,21,24,20,16,16,25,15,21,21,11,6,25,15,11,18,18,20,25,13,18,
    11,7,13,16,13,5,19,6,7,17,24,6,10,11,13,24,23,21,7,23,14,17,9,9,17,
    16,13,13,6,21,19,22,10,8,5,10,22,24,23,15,5,7,17,20,6,7,14,20,10,
    25,19,13,5,20,9,24,21,11,6,18,6,13,19,15,18,7,17,21,12,24,21,20,11,
    19,20,19,13,19,17,15,18,18,6,20,20,6,20,8,14,14,19,10,20,15,18,6,
    12,24,14,5,17,12,25,13,16,8,7,19,21,20,18,11,7,12,17,21,10,17,6,9,
    23,14,24,19,12,19,11,18,23,14,20,10,8,10,16,18,12,14,25,12,17,21,
    25,21,5,13,11,6,5,12,18,17,18,21,8,16,8,12,22,20,17,12,19,17,24,
    14,17,20,22,24,19,16,13,7,21,21,17,5,15,10,23,25,6,17,11,11,15,
    13,5,13,11,25,9,11,18,14,20,9,15,10,25,13,17,16,5,19,14,24,15,
    19,7,15,18,13,10,21,10,15,19,24,18,19,16,15,19,14,15,23,24,7,11,
    9,10,24,11,19,10,12,13,11,14,5,15,13,17,25,9,21,7,20,12,15,10,5,
    14,10,10,16,23,20,11,18,17,7,8,11,23,9,12,8,13,14,6,20,18,20,24,
    25,21,17,12,7,19,24,12,18,6,13,8,9,19,10,7,19,15,7,24,21,19,16,14,
    8,23,20,20,18,24,25,21,10,17,19,21,18,16,21,14,19,15,18,8,23,17,13,
    21,7,20,14,11,10,8,11,9,24,18,19,25,12,12,8,19,17,17,24,5,10,11,11,
    11,25,12,19,13,6,9,21,10,12,8,14,5,7,22,21,7,15,8,22,18,17,6,16,7
  */
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 19, 18, 17, 16, 15,
  14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3,
  4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 19, 18, 17, 16, 15, 14, 13,
  12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
  18, 19, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5,
  6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 19, 18, 17, 16, 15, 14, 13, 12,
  11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
  18, 19, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5,
  6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 19, 18, 17, 16, 15, 14, 13, 12,
  11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
  18, 19, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5,
  6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 19, 18, 17, 16, 15, 14, 13, 12,
  11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
  18, 19, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4,
  5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
};

ad9833_settings_t ad_settings; ///<This is used to store all settings.

int FreqAwal [] = {20, 100, 700, 100};
int FreqAkhir [] = {100, 700, 2000, 700};

static inline void ad9833_1_send(uint16_t packet)
{

  digitalWrite(FNC_PIN_1, LOW);  // FNCpin low to write to AD9833 over SPI

  //delayMicroseconds(2); // Some delay may be needed

  // TODO: Are we running at the highest clock rate?
  SPI.transfer((uint8_t)(packet >> 8)); // Transmit 16 bits 8 bits at a time
  SPI.transfer((uint8_t)packet);

  digitalWrite(FNC_PIN_1, HIGH);   // Write to SPI done - bring up this pin
}

static inline void ad9833_2_send(uint16_t packet)
{

  digitalWrite(FNC_PIN_2, LOW);  // FNCpin low to write to AD9833 over SPI

  //delayMicroseconds(2); // Some delay may be needed

  // TODO: Are we running at the highest clock rate?
  SPI.transfer((uint8_t)(packet >> 8)); // Transmit 16 bits 8 bits at a time
  SPI.transfer((uint8_t)packet);

  digitalWrite(FNC_PIN_2, HIGH);   // Write to SPI done - bring up this pin
}

void ad9833_set_mode(uint8_t mode) {
  ad_settings.mode = mode;
  switch (mode) {
    case AD_OFF:
      ad_settings.command_reg |= (1 << AD_SLEEP12);
      ad_settings.command_reg |= (1 << AD_SLEEP1);
      break;
    case AD_TRIANGLE:
      ad_settings.command_reg &= ~(1 << AD_OPBITEN);
      ad_settings.command_reg |=  (1 << AD_MODE);
      ad_settings.command_reg &= ~(1 << AD_SLEEP12);
      ad_settings.command_reg &= ~(1 << AD_SLEEP1);
      break;
    case AD_SQUARE:
      ad_settings.command_reg |=  (1 << AD_OPBITEN);
      ad_settings.command_reg &= ~(1 << AD_MODE);
      ad_settings.command_reg |=  (1 << AD_DIV2);
      ad_settings.command_reg &= ~(1 << AD_SLEEP12);
      ad_settings.command_reg &= ~(1 << AD_SLEEP1);
      break;
    case AD_SINE:
      ad_settings.command_reg &= ~(1 << AD_OPBITEN);
      ad_settings.command_reg &= ~(1 << AD_MODE);
      ad_settings.command_reg &= ~(1 << AD_SLEEP12);
      ad_settings.command_reg &= ~(1 << AD_SLEEP1);
      break;
  }
  ad9833_1_send(ad_settings.command_reg);
  ad9833_2_send(ad_settings.command_reg);

}

void ad9833_set_freq_out(uint8_t freq_out) {
  ad_settings.freq_out = freq_out;
  switch (freq_out) {
    case 0:
      ad_settings.command_reg &= ~(1 << AD_FSELECT);
      break;
    case 1:
      ad_settings.command_reg |= (1 << AD_FSELECT);
      break;
    case 2:
      //TODO
      break;
  }
  ad9833_1_send(ad_settings.command_reg);
  ad9833_2_send(ad_settings.command_reg);

}

void ad9833_set_phase(uint8_t reg, double phase) {
  uint16_t reg_reg; //probably should be renamed...
  if (reg == 1)
    reg_reg = AD_PHASE1;
  else
    reg_reg = AD_PHASE0;

  ad_settings.phase[reg] = phase;

  ad9833_1_send(reg_reg | AD_PHASE_CALC(ad_settings.phase[reg]));
  ad9833_2_send(reg_reg | AD_PHASE_CALC(ad_settings.phase[reg]));

}

void ad9833_set_phase_out(uint8_t phase_out) {
  ad_settings.phase_out = phase_out;
  switch (phase_out) {
    case 0:
      ad_settings.command_reg &= ~(1 << AD_PSELECT);
      break;
    case 1:
      ad_settings.command_reg |= (1 << AD_PSELECT);
      break;
    case 2:
      //TODO
      break;
  }
  ad9833_1_send(ad_settings.command_reg);
  ad9833_2_send(ad_settings.command_reg);

}

void ad9833_1_set_frequency(uint8_t reg, double freq) {
  uint32_t freq_reg;
  uint16_t reg_reg; //probably should be renamed...
  freq_reg = AD_FREQ_CALC(freq);
  ad_settings.freq[reg] = freq;

  if (reg == 1)
    reg_reg = AD_FREQ1;
  else
    reg_reg = AD_FREQ0;

  ad9833_1_send((1 << AD_B28) | ad_settings.command_reg);
  ad9833_1_send(reg_reg | (0x3FFF & (uint16_t)(freq_reg >> 2 )));
  ad9833_1_send(reg_reg | (0x3FFF & (uint16_t)(freq_reg >> 16)));

}
void ad9833_2_set_frequency(uint8_t reg, double freq) {
  uint32_t freq_reg;
  uint16_t reg_reg; //probably should be renamed...
  freq_reg = AD_FREQ_CALC(freq);
  ad_settings.freq[reg] = freq;

  if (reg == 1)
    reg_reg = AD_FREQ1;
  else
    reg_reg = AD_FREQ0;

  ad9833_2_send((1 << AD_B28) | ad_settings.command_reg);
  ad9833_2_send(reg_reg | (0x3FFF & (uint16_t)(freq_reg >> 2 )));
  ad9833_2_send(reg_reg | (0x3FFF & (uint16_t)(freq_reg >> 16)));

}
void ad9833_init(void)
{

  // set half awake state
  ad9833_1_send((1 << AD_SLEEP12) | (1 << AD_RESET));
  ad9833_2_send((1 << AD_SLEEP12) | (1 << AD_RESET));
  ad_settings.command_reg |= (1 << AD_SLEEP12);

  //set some nice default values
  ad9833_1_set_frequency(0, 0);
  ad9833_1_set_frequency(1, 0);
  ad9833_2_set_frequency(0, 0);
  ad9833_2_set_frequency(1, 0);
  ad9833_set_phase(0, 0);
  ad9833_set_phase(1, 0);
  ad9833_set_freq_out(0);
  ad9833_set_phase_out(0);

}
