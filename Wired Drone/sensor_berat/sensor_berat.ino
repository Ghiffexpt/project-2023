#include "HX711.h"

HX711 scale(13, 4);
unsigned long currentMillis;
unsigned long timerWrite;
unsigned long timerRead;

float fixData;
float calibration_factor = 1000; // this calibration factor is adjusted according to my load cell
float units;
float ounces;
float avarage;

uint8_t readyAct;
uint8_t counter;

void setup()
{
  Serial.begin(9600);
  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading

}

void loop()
{
  currentMillis = millis(); // Dapatkan waktu saat ini
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  units = scale.get_units(), 1;
  if (units < 0)
  {
    units = 0.00;
  }
  if (currentMillis - timerWrite >= 100) {
 
    if (counter == 0 ) {
      Serial.print("v");
      Serial.println(units);

      timerWrite = currentMillis;
    }
  }
}
