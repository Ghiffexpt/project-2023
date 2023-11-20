#define dir1 5
#define dir2 2
#define step1 6
#define step2 3
#define pinR 8
#define pinL 7

volatile unsigned long previousMillis1 = 0;
volatile unsigned long previousMillis2 = 0;
volatile unsigned long MillisReadLimit = 0;
volatile unsigned long millisG = 0;

unsigned long currentMillis;
unsigned long interval1 = 1001;
unsigned long interval2 = 1001;
unsigned long interval3 = 130000 ;
unsigned long fixSpeedG;
unsigned long fixSpeedW;

uint8_t extSG;
uint8_t i = 1001;
uint8_t shutdown;
uint8_t statdir1;
uint8_t statdir2; //kiri1 kanan0
uint8_t cmd = 0;
uint8_t ext1;
uint8_t ext2;
uint8_t limitR;
uint8_t limitL;
uint8_t DR;
uint8_t DL;
uint8_t R;
uint8_t L;
uint8_t dirext;
uint8_t sendR = 1;
uint8_t sendL = 1;
uint8_t extMH;
uint8_t extMW;
uint8_t Auto;
uint8_t intervalG;

String data;
