volatile unsigned long timer1Millis = 0;
void SetUpMillis() {
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 0;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);      
  sei(); 
}
ISR(TIMER1_COMPA_vect) {
  timer1Millis++; 
}
unsigned long millisGip() {
  unsigned long millis;
  cli(); // Matikan interrupt sementara
  millis = timer1Millis;
  sei(); // Aktifkan interrupt kembali
  return millis;
}
