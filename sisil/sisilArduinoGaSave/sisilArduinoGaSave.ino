#include "data.h"
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


#include "Digipot.h"



int channel = 12;
int cmd;
int setPot1;
int setPot2;
int statPot1 = 0;
int statPot2 = 0;
int statChannel = 0;

char stat;

String CONFIG;
String ParsJSON;
String dataFIX;
String dataACT;
String statAwal;
String ip = ipdevice(channel);
String status;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(9, OUTPUT);
  pinDigiPot(13, 12, 11, 8);
  reset1();
  reset2();
}

void loop() {
  if (Serial.available() > 0) {
    static uint32_t head;
    static char msg[30];
    static int n = 0;
    byte x = Serial.read();
    n++;
    msg[n] = x;
    head |= x;
    if (( head & 0xFFFF0000) == 0x02140000) {
      char byte_arus[3];
      memcpy(byte_arus, &msg[1], 4);
      checkChannel(byte_arus);
      head = 0x00;
      n = 0;
    } else if (( head & 0xFFFF0000) == 0x01050000) {
      char byte_arus[3];
      memcpy(byte_arus, &msg[1], 4);
      CheckDigipot(byte_arus);
      head = 0x00;
      n = 0;

    } else if (( head & 0xFFFF0000) == 0x03040000) {
      char byte_arus[3];
      memcpy(byte_arus, &msg[1], 4);
      if (byte_arus[3] != stat && byte_arus[2] == 0x15) {
        Serial.print(statMicro());
        stat = byte_arus[3];
      }
      head = 0x00;
      n = 0;

    }
    head = head << 8;
  }
}

String statMicro() {
  String json = "";
  json += "{channel:";
  json += channel;
  json += ",status:";
  json += status;
  json += "}";
  return json;
}

void checkChannel(char msg[]) {
  if (msg[2] == 0x01 && msg[3] == 0x00) {
    digitalWrite(9, HIGH);
    status = "on";
    statChannel = 1;
    Serial.print(statMicro());

  } else if (msg[2] == 0x00 && msg[3] == 0x01) {
    digitalWrite(9, LOW);
    status = "off";
    statChannel = 0;
    Serial.print(statMicro());
  }

}

void CheckDigipot(char byte_arus[]) {
  int cmd = byte_arus[2];
  if (byte_arus[3] == 0x01) {
    setDigiPot1(cmd);
    statPot1 = cmd;

  } else if (byte_arus[3] == 0x02) {
    setDigiPot2(cmd);
    statPot2 = cmd;
  }
}
