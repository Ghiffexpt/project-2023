#include "Dec.h"
void(*mulai_reset) (void) = 0;

class Cntrl {
  private:
    int dir1;
    int dir2;
    int step1;
    int step2;

  public:
    unsigned long fixSpeedW;
    unsigned long fixSpeedG;
    Cntrl(int dir1, int dir2, int step1, int step2) {
      this->dir1 = dir1;
      this->dir2 = dir2;
      this->step1 = step1;
      this->step2 = step2;
      pinMode(dir1, OUTPUT);
      pinMode(dir2, OUTPUT);
      pinMode(step1, OUTPUT);
      pinMode(step2, OUTPUT);
    }

    void motorW() {
      if (currentMicrosW - previousMillisW >= fixSpeedW) {
        ext1 = !ext1;
        digitalWrite(step1, ext1);
        previousMillisW = currentMicrosW; // Perbarui waktu sebelumnya
      }
    }
    void motorMillis() {
      if (currentMillis - previousMillisW >= fixSpeedW) {
        ext1 = !ext1;
        digitalWrite(step1, ext1);
        previousMillisW = currentMillis; // Perbarui waktu sebelumnya
      }
    }
    void motorG() {
      if (currentMicrosG - previousMillisG >= fixSpeedG) {
        ext2 = !ext2;
        digitalWrite(step2, ext2);
        previousMillisG = currentMicrosG; // Perbarui waktu sebelumnya
      }
    }
    void CntrlDirW(int output) {
      digitalWrite(dir1, output);
    }
    void CntrlDirG(int output) {
      digitalWrite(dir2, output);
    }
};
