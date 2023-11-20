#define dir1 5
#define dir2 2
#define step1 6
#define step2 3
#define pinR 8
#define pinL 7
#define pinStatAlarm 12
#define pinActAlarm 13
//read 0 alarmhidup
//act 1 reset alarm alarm

volatile unsigned long previousMillis [1][2]; //[0][0] = previousMillis1,[0][1] = previousMillis2,[0][2] = MillisReadLimit,[1][0] = millisW

unsigned long currentMicros;
unsigned long currentMillis;//[0][0] = Micros,[0][1] = Millis
 long interval[1][2];//[0][0] = interval1,[0][1] = interval2,[0][2] =  interval3,[1][0] = timeoutW
 long Speed[0][1];//[0][0] = speedG, [0][1] = speedW 
unsigned long inputMin = 1000;  // Nilai minimum dalam rentang yang diinginkan
unsigned long inputMax = 10;    // Nilai maksimum dalam rentang yang diinginkan
unsigned long outputMin = 10;   // Nilai minimum dalam rentang awal
unsigned long outputMax = 1000; // Nilai maksimum dalam rentang awal

//kiri1 kanan0
uint8_t Status [0][1];//[0][0] = statdir1, [0][1] = statdir2
uint8_t output[0][1];//[0][0] = ext1, [0][0] = ext2
uint8_t LimitDirection [1][2];//[0][0] = limitR,[0][1] = limitL,[0][2] = dirext,[1][1] = sendR,[1][0] = sendL
uint8_t startWired[0][1]; 
uint8_t AlarmSend[1][1];
String data;
