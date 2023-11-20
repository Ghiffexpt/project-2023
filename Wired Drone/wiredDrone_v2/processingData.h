void ProcessSpeedG() {
  fixSpeedG = interval2 * 8.25;
}

void ProcessingDataWired(int inputValue) {
  int outputValue = 1000 - inputValue ;
  if (inputValue < 10 ) {
    interval1 = 1001;
  } else interval1 = static_cast<unsigned long>(outputValue);
}
void ProcessingDataGuide(int inputValue) {
  int outputValue = 1000 - inputValue;
  if (inputValue < 10) {
    interval2 = 1001;
  } else interval2 = static_cast<unsigned long>(outputValue);

}
void ProcessSpeedW(unsigned long interval) {
  fixSpeedW = interval * 0.5;
}
