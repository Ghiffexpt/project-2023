#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
// Definition of FSYNC , CHIP SELECT pin in Arduino board
#define FNC_PIN_1 9
#define FNC_PIN_2 8

/** \name AD9833 waveform output modes
   Parameters of \ref ad9833_set_mode()
   @{
*/
#define AD_OFF      0
#define AD_TRIANGLE 1
#define AD_SQUARE   2
#define AD_SINE     3
/*@}/

  /** \name AD9833 command register bits
   @{
*/

#define AD_B28     13
#define AD_HLB     12
#define AD_FSELECT 11
#define AD_PSELECT 10
#define AD_RESET   8
#define AD_SLEEP1  7
#define AD_SLEEP12 6
#define AD_OPBITEN 5
#define AD_DIV2    3
#define AD_MODE    1

/* @}/

  /** \name AD9833 register addresses
   @{
*/
#define AD_FREQ0  (1<<14)
#define AD_FREQ1  (1<<15)
#define AD_PHASE0 (3<<14)
#define AD_PHASE1 ((3<<14)|(1<<13))

/* @}/

  /** \name AD9833 calculation macros
   @{ */
#define AD_F_MCLK 25000000 ///<Clock speed of the ad9833 reference clock
#define AD_2POW28 268435456 ///<used in calculating output freq

/** Macro that calculates the value for a ad9833 frequency register from a frequency */
#define AD_FREQ_CALC(freq) (uint32_t)(((double)AD_2POW28/(double)AD_F_MCLK*freq)*4)

/* Macro that calculates value for Timer1 output compare from a frequency/
  #define AD_MOD_FREQ_CALC(freq) (F_CPU/(64*(uint32_t)freq))

  /** Macro that calculates the value for a ad9833 phase register from a phase in degrees */
#define AD_PHASE_CALC(phase_deg) (uint16_t)((512*phase_deg)/45)
/** @} */
/** Struct that holds all the configuration it's initialized as a global variable
   in the ad9833.c file */

#include "AD.h"



#define DIGIPOT_UP   1
#define DIGIPOT_DOWN 0
#define DIGIPOT_MAX_AMOUNT 99
#define DIGIPOT_UNKNOWN 255

uint8_t incPin;
uint8_t udPin;
uint8_t cs2Pin;
uint8_t cs1Pin;
uint8_t currentValue1;
uint8_t currentValue2;

#include "DigiPot.h"

String dataFIX;
String dataACT;

int cmd ;
int setPot1;
int setPot2;
int StatStart;

void setup() {
  // put your setup code here, to run once:
  pinMode(FNC_PIN_1, OUTPUT);
  pinMode(FNC_PIN_2, OUTPUT);
  Serial.begin(115200);
  digitalWrite(FNC_PIN_1, HIGH);
  digitalWrite(FNC_PIN_2, HIGH);
  SPI.begin();
  SPI.setDataMode(SPI_MODE2);
  //  Serial.begin(9600);
  ad9833_init();
  ad9833_set_mode(AD_SINE);
  cmd = 1;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    char a = Serial.read();
    if (a != '-' && a != ';' && a != '#' && a != '*') {
      dataACT += String(a);
    } else if (a == '-') {
      dataACT = "";
      dataFIX = "";
    }  else if ( a == ';') {
      dataFIX = dataACT;
      cmd = atoi(dataFIX.c_str());
      Serial.print("cmd: ");
      Serial.println(cmd);
    }
  }
  switch (cmd) {
    case 1: {
        noiseIP();
      } break;

    case 2: {
        shadowIP();
      } break;

    case 3: {
        noshaIP();
      } break;
    case 4: {
        noiseIP();
      } break;

    case 5: {
        shadowIP();
      } break;

    case 6: {
        noshaIP();
      } break;
    default: {

      } break;
  }
}

void noise() {
  ad9833_set_mode(AD_SINE);
  ad9833_1_set_frequency(0, random(24000, 26000));
  ad9833_2_set_frequency(0, random(24000, 26000));
}
void shadow() {
  ad9833_set_mode(AD_SINE);
  ad9833_1_set_frequency(0, 25000);
  ad9833_2_set_frequency(0, 25000);
}
void nosha() {
  ad9833_set_mode(AD_SINE);
  ad9833_1_set_frequency(0, 25000);
  ad9833_2_set_frequency(0, random(24000, 26000));
}

void noiseIP() {
  Serial.println("Noise");
  ad9833_set_mode(AD_SINE);
  uint16_t i, j, sequence ;

  // generating RANDOMIZED 5-25Hz INFRASOUND over 25kHZ ULTRASOUND CARRIER


  sequence = 0;

  for (i = 26000; i > 24000; i = i - pgm_read_byte(randomized + sequence), sequence++)
  {
    Serial.println("NOISE");
    ad9833_1_set_frequency(0, (double)i );
    ad9833_2_set_frequency(0, (double)i );

  };
  sequence = 0;

  for (i = 24000; i < 26000; i = i + pgm_read_byte(randomized + sequence), sequence++ )
  {
    Serial.println("NOISE");
    ad9833_1_set_frequency(0, (double)i );
    ad9833_2_set_frequency(0, (double)i );

  };
}
void shadowIP() {
  Serial.println("Shadow");
  ad9833_set_mode(AD_SINE);
  ad9833_1_set_frequency(0, 25000);
  ad9833_2_set_frequency(0, 25000);
}
void noshaIP() {
  Serial.println("Nosha");
  ad9833_set_mode(AD_SINE);
  ad9833_1_set_frequency(0, 25000);

  uint16_t i, j, sequence ;

  sequence = 0;

  for (i = 26000; i > 24000; i = i - pgm_read_byte(randomized + sequence), sequence++)
  {
    ad9833_2_set_frequency(0, (double)i );

  };
  sequence = 0;

  for (i = 24000; i < 26000; i = i + pgm_read_byte(randomized + sequence), sequence++ )
  {
    ad9833_2_set_frequency(0, (double)i );

  };
}
