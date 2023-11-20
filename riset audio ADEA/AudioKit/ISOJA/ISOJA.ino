#include <WiFi.h>
#include <AsyncTCP.h>
#include "FS.h"
#include "SD_MMC.h"
#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "AudioLibs/AudioA2DP.h"
#include "AudioLibs/AudioSourceSDFAT.h" // or AudioSourceIdxSDFAT.h
#include "AudioCodecs/CodecMP3Helix.h"
#include <SPI.h>
#include "SD.h"

#define PIN_AUDIO_KIT_SD_CARD_CS 13
#define PIN_AUDIO_KIT_SD_CARD_MISO 2
#define PIN_AUDIO_KIT_SD_CARD_MOSI 15
#define PIN_AUDIO_KIT_SD_CARD_CLK 14

// Global Variables

const int chipSelect = SS;

String JSONMusic;
String ssid;
String password ;

String host = "www2.cs.uic.edu";
String extension = "/~i101/SoundFiles/PinkPanther60.wav";
String wav;
String pesan;
String deleteMP3;

AsyncClient tcpClient;

File file;

unsigned long notConnect = 0;
String data;
String statusAudio;
String playMP3;
String dataACT;
String dataFIX;
String ParsJSON;

int statList;
int MP3SD;
int configMP3;
int retry;
int statinstall = 1;
int play ;
int downloadMP3 = 0;
int RetryDownload = 0;
const char*startFilePath = "/";
const char* ext = "mp3";
float vol;

SdSpiConfig sdcfg(PIN_AUDIO_KIT_SD_CARD_CS, DEDICATED_SPI, SD_SCK_MHZ(10) , &AUDIOKIT_SD_SPI);
AudioSourceSDFAT source(startFilePath, ext, sdcfg);
AudioKitStream kit;

MP3DecoderHelix decoder;
AudioPlayer player(source, kit, decoder);
BluetoothA2DPSink a2dp_sink;

String bl = "sas";
String bluetooth;
String stream;


void sendStringToServer( String sendMsg, AsyncClient* tcpClient ) {
  tcpClient->add( sendMsg.c_str() , sendMsg.length() );
  tcpClient->send();
}

static void handleData(void* arg, AsyncClient* client, void *data, size_t len) {
  static bool first_response = true;
  File* file = (File*)arg;
  /* Server response information is being sent before the actual file.
     This extra information should not be saved into the file.*/
  if (first_response) {
    size_t cur_pos = 0;
    char* temp = (char*) data;
    for (int i = 0; i < len; ++i) {
      if (temp[i] == '\n') {
        cur_pos = i;
      }
    }
    ++cur_pos;
    file->write((uint8_t*)data + cur_pos, len - cur_pos);
    first_response = false;
    return;
  }

  file->write((uint8_t*)data, len);
}

static void handleError(void* arg, AsyncClient* client, int8_t error) {
  Serial.println("[CALLBACK] error");
  Serial.println("Download error, please check network or reset ISOJA");
  File* file = (File*)arg;
  file->close();
}

static void handleTimeOut(void* arg, AsyncClient* client, uint32_t time) {
  Serial.println("[CALLBACK] ACK timeout");
  Serial.println("Download timeout, please check network or reset ISOJA");
  File* file = (File*)arg;
  file->close();
  RetryDownload = 1;
}

static void handleDisconnect(void* arg, AsyncClient* client) {
  Serial.println("[CALLBACK] discconnected");
  Serial.println("Download file from URL already, please check the file");
  File* file = (File*)arg;
  file->close();

  //  file.close();
}

void next(bool, int, void*) {
  player.next();
}

void previous(bool, int, void*) {
  player.previous();
}

// Write data to ISOJA in callback
void read_data_stream(const uint8_t *data, uint32_t length) {
  kit.write(data, length);
}

//List file in directory
void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);
  JSONMusic = "{";
  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      statList++;
      String audio = file.name();
      JSONMusic += "\"";
      JSONMusic += statList;
      JSONMusic += "\":\"";
      JSONMusic += audio;
      JSONMusic += "\"";
      Serial.println(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    if (statList > 0) {
      JSONMusic += ",";
    }
    file = root.openNextFile();
  }
  JSONMusic += "\"List\":\"Music\"}";
  Serial.print("JSON Music: ");
  Serial.println(JSONMusic);
  ESP.restart();
}

void setup() {
  //  digitalWrite(21, HIGH);
  //  pinMode(21, OUTPUT);
  //
  Serial.begin(115200);
  //  Serial.println("");
  ConfigAudio();
  bluetoothAudio();
}

void loop() {
  unsigned long now = millis();
  notConnect = now;
  if (Serial.available()) {
    char a = Serial.read();
    Serial.println(a);
    if (a != '-' && a != '%' && a != ')' && a != '+' && a != '&') {
      dataACT += String(a);
    } else if (a == '-') {
      Serial.println("ext data");
      dataACT = "";
      dataFIX = "";
    } else if (a == '+') {
      Serial.println("List Music");
      SPI.begin(PIN_AUDIO_KIT_SD_CARD_CLK, PIN_AUDIO_KIT_SD_CARD_MISO, PIN_AUDIO_KIT_SD_CARD_MOSI, PIN_AUDIO_KIT_SD_CARD_CS);
      if (!SD.begin(PIN_AUDIO_KIT_SD_CARD_CS)) {
        Serial.println("Card Mount Failed");
        return;
      }
      listDir(SD, "/", 0);
    } else if ( a == '&') {
      play = 1;
      dataFIX = dataACT; // play mp3 in sd card
      Serial.println(dataFIX);
      PlayMP3();

    } else if (a == '%') {
      dataFIX = dataACT; // play mp3 in sd card
      Serial.println(dataFIX);
      parsJSON("Music");
      wav = ParsJSON;
      parsJSON("SSID");
      ssid = ParsJSON;
      parsJSON("PW");
      password = ParsJSON;
      parsJSON("Delete");
      deleteMP3 = ParsJSON;
      if (!SD.begin(PIN_AUDIO_KIT_SD_CARD_CS)) {
        Serial.println("Card Mount Failed");
      }
      if (deleteMP3 != "") {
        SD.remove("/" + deleteMP3);
        Serial.println("Delete file: " + deleteMP3);
      }
      if (ssid && password != "") {
        WiFi.begin(ssid.c_str(), password.c_str());
        Serial.print("[WIFI] connecting to network " + String(ssid) );
        while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.println("Waiting Connected");
          now = millis();
          if (now - notConnect > 30000) {
            notConnect = now;
            ESP.restart();
          }
        }
        Serial.println("[WIFI] connected with Ip: " + WiFi.localIP().toString() );

        // SD Card Init

        // Create a new file (downloaded data will be written here)
        file = SD.open("/" + wav, FILE_WRITE);
        download(host, extension, &tcpClient, &file);
      }
    } else if (a == ')') {
      play = 1;
      dataFIX = dataACT; // play mp3 in sd card
      Serial.println(dataFIX);
      String kill = dataFIX.substring(0, 1);
      int cmd = atoi(kill.c_str());
      Serial.print("cmd: ");
      Serial.println(cmd);

      switch (cmd) {
        case 1: {
            ConfigAudio();
            bluetoothAudio();
            break;
          }
        case 2: {
            retry = 1;
            break;
          }
        case 3: {
            retry = 0;
            break;
          }
        case 4: {
            play = 0;
            break;
          }
        case 5: {
            play = 1;
            break;
          }
        case 6: {
            playMP3 = "";
            play = 0;
            player.setPath(playMP3.c_str());
            break;
          }
        case 7: {
            String vl = dataFIX.substring(1, '?');
            vol = vl.toFloat();
            Serial.print("Volume: ");
            Serial.println(vol);
            player.setVolume(vol);
            break;
          }
        case 8: {
            //            mulai_reset();
            digitalWrite(21, LOW);
            break;
          }
      }
    }
  }

  if (RetryDownload == 1) {
    download(host, extension, &tcpClient, &file);
    Serial.println("RE-Download");
    RetryDownload = 0;
  }
  if (stream == "ON") {
    kit.processActions();
  }
  if (play ==  1) {
    if (statNext == 1) {
      if (retry == 1 ) {
        player.setPath(playMP3.c_str());
        statNext = 0;
      }
    }
    player.copy();
    kit.processActions();
  }
}
void bluetoothAudio() {
  a2dp_sink.set_stream_reader(read_data_stream, false);
  a2dp_sink.start("Little Ping");
  Serial.println("Please connect to bluetooth ISOJA-Sound");
  stream = "ON";
}
void ConfigAudio() {
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup output
  auto cfg = kit.defaultConfig(TX_MODE);
  kit.begin(cfg);

  kit.addAction(PIN_KEY4, next);
  kit.addAction(PIN_KEY3, previous);


  // setup player

}
void parsJSON(String key) {
  uint8_t a = dataFIX.indexOf(key);
  uint8_t b = dataFIX.indexOf(":", a);
  uint8_t c = dataFIX.indexOf(",", b);

  ParsJSON = dataFIX.substring(b + 1, c);
  Serial.print("JSON: ");
  Serial.println(ParsJSON);
}
void PlayMP3() {
  Serial.println("find");
  ConfigAudio();
  player.setVolume(vol);
  player.begin();
  playMP3 = dataFIX;
  player.setPath(playMP3.c_str());
  statusAudio = "stSD";
}
void download(String host, String extension, AsyncClient * tcpClient, File * file) {
  // Assign callbacks
  tcpClient->onData(&handleData, file);
  tcpClient->onError(&handleError, file);
  tcpClient->onTimeout(&handleTimeOut, file);
  tcpClient->onDisconnect(&handleDisconnect, file);

  // Connect to host
  tcpClient->connect(host.c_str(), 80);
  while (!tcpClient->connected()) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("Connected");
  Serial.println("Please wait for download File url MP3");
  // Generate TCP Download command
  String resp = String("GET ") +
                extension +
                String(" HTTP/1.1\r\n") +
                String("Host: ") +
                host +
                String("\r\n") +
                String("Icy-MetaData:1\r\n") +
                String("Connection: close\r\n\r\n");

  // Send download command
  sendStringToServer(resp, tcpClient);


  // After this point, onData Callback will receive the data and write it to SD card
}
