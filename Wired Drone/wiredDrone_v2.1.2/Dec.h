
#define pinR 8
#define pinL 7
#define alarmRead 12
#define alarmAction 13

volatile unsigned long previousMillisW = 0;
volatile unsigned long previousMillisG = 0;
volatile unsigned long MillisReadLimit = 0;
volatile unsigned long millisW = 0;
volatile unsigned long millisStopW = 0;

unsigned long currentMillis;
unsigned long currentMicrosW;
unsigned long currentMicrosG;

unsigned long intervalW = 1002;
unsigned long intervalG = 1001;

uint8_t statdir1;
uint8_t statdir2; //kiri1 kanan0
uint8_t cmd = 0;
uint8_t ext1;
uint8_t ext2;
uint8_t limitR;
uint8_t limitL;
uint8_t R;
uint8_t L;
uint8_t sendR = 1;
uint8_t sendL = 1;
uint8_t alarmSend;
uint8_t startG;
uint8_t startW;
uint8_t stopW;

String data;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;
