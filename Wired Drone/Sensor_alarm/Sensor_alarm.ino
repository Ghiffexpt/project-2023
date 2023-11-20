#define pinPulse D3
#define pinClock D8
#define OutAlarm D2

class CNTRLpin {
private:
    unsigned long counterPulse;
    uint8_t Pulse, Clock, Alarm, LastPulse, LastClock;
    static CNTRLpin* instance; // Static member variable to store the instance

public:
    CNTRLpin(uint8_t Pulse, uint8_t Clock, uint8_t Alarm) {
        this->Pulse = Pulse;
        this->Clock = Clock;
        this->Alarm = Alarm;
        pinMode(Pulse, INPUT);
        pinMode(Clock, INPUT);
        pinMode(Alarm, OUTPUT);
        digitalWrite(Alarm, LOW);
        attachInterrupt(digitalPinToInterrupt(Clock), &CNTRLpin::extDataStatic, FALLING);
        instance = this; // Store the instance
    }

    unsigned long ReadCounter() {
        uint8_t Datapulse = digitalRead(Pulse);
        uint8_t Dataclock = digitalRead(Clock);
        Serial.println(Datapulse);
        if (Datapulse == 1 && LastPulse != Datapulse) {
            counterPulse = 0;
            LastPulse = Datapulse;
        }
        return counterPulse;
    }

    void SendData() {
        digitalWrite(Alarm, HIGH);
        delay(500);
        digitalWrite(Alarm, LOW);
        counterPulse = 0;
    }

private:
    static void ICACHE_RAM_ATTR extDataStatic() {
        if (instance) {
            instance->extData();
        }
    }

    void extData() {
        counterPulse += 1;
    }
};

CNTRLpin* CNTRLpin::instance = nullptr;

CNTRLpin AlarmCNTRL(pinPulse, pinClock, OutAlarm);

void setup() {
    Serial.begin(9600);
}

void loop() {
    unsigned long counter = AlarmCNTRL.ReadCounter();

    if (counter == 1000) {
        AlarmCNTRL.SendData();
        Serial.println(counter);
    }
}
