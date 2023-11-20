#include <EEPROM.h>

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

String CONFIG;
String ParsJSON;
String dataFIX;
String dataACT;
String statAwal;

int cmd ;
int setPot1;
int setPot2;
int statPot1 = 0;
int statPot2 = 0;
int statChannel = 0;

char stat;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(9, OUTPUT);
  pinDigiPot(13, 12, 11, 8);
  reset1();
  reset2();
  CheckStat();
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
        Serial.print("Connect dev 2 IP: 172.16.1.2");
        stat = byte_arus[3];
      }
      head = 0x00;
      n = 0;

    }
    head = head << 8;
  }
}
void deleteStringToROM()
{
  Serial.println("delete");
  byte dataToDelete = 0; // This is the data you want to "delete"
  int addressToDelete = 0; // Replace with the correct address

  EEPROM.write(addressToDelete, dataToDelete);
  delay(1000);
}
void writeStringToROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);

  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}
void updateStringToROM(int addrOffset, const String &strToUpdate)
{
  byte len = strToUpdate.length();
  EEPROM.update(addrOffset, len);

  for (int i = 0; i < len; i++)
  {
    EEPROM.update(addrOffset + 1 + i, strToUpdate[i]);
  }
}

String readStringFromROM(int addrOffset)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];

  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\ 0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)

  return String(data);
}

void checkChannel(char msg[]) {
  if (msg[2] == 0x01 && msg[3] == 0x00) {
    digitalWrite(9, HIGH);
    Serial.print("Channel 2 Activate");
    statChannel = 1;
    String ext = "";
    ext += "{SC:";
    ext += statChannel;
    ext += ",1PT:";
    ext += statPot1;
    ext += ",2PT:";
    ext += statPot2;
    ext += ",}";
    writeStringToROM(0, ext);
  } else if (msg[2] == 0x00 && msg[3] == 0x01) {
    digitalWrite(9, LOW);
    Serial.print("Channel 2 Non activate");
    statChannel = 0;
    String ext = "";
    ext += "{SC:";
    ext += statChannel;
    ext += ",1PT:";
    ext += statPot1;
    ext += ",2PT:";
    ext += statPot2;
    ext += ",}";
    writeStringToROM(0, ext);
  }

}
void CheckStat() {
  ActJSON("SC");
  int cmd = atoi(ParsJSON.c_str());
  digitalWrite(9, cmd);
  Serial.print("Stat channel : ");
  Serial.print(cmd);
  ActJSON("1PT");
  cmd = atoi(ParsJSON.c_str());
  setDigiPot1(cmd);
  Serial.print("Value potensio 1 : ");
  Serial.print(cmd);
  ActJSON("2PT");
  cmd = atoi(ParsJSON.c_str());
  setDigiPot2(cmd);
  Serial.print("Value potensio 2 : ");
  Serial.print(cmd);
}
void CheckDigipot(char byte_arus[]) {
  int cmd = byte_arus[2];
  if (byte_arus[3] == 0x01) {
    setDigiPot1(cmd);
    statPot1 = cmd;
    Serial.print("Value potensio 1 : ");
    Serial.print(statPot1);
    String ext = "";
    ext += "{SC:";
    ext += statChannel;
    ext += ",1PT:";
    ext += statPot1;
    ext += ",2PT:";
    ext += statPot2;
    ext += ",}";
    writeStringToROM(0, ext);
  } else if (byte_arus[3] == 0x02) {
    setDigiPot2(cmd);
    statPot2 = cmd;
    Serial.print("Value potensio 2 : ");
    Serial.print(statPot2);
    String ext = "";
    ext += "{SC:";
    ext += statChannel;
    ext += ",1PT:";
    ext += statPot1;
    ext += ",2PT:";
    ext += statPot2;
    ext += ",}";
    writeStringToROM(0, ext);
  }
}
void ActJSON(String key) {
  statAwal =  readStringFromROM(0);
  uint8_t a = statAwal.indexOf(key);
  uint8_t b = statAwal.indexOf(":", a);
  uint8_t c = statAwal.indexOf(",", b);

  ParsJSON = statAwal.substring(b + 1, c);

  //  Serial.print("JSON: ");
  //  Serial.println(ParsJSON);
}
