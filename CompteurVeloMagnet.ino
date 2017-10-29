/*List composant:
 * Arduino uno or Atmega328P-PU
 * If you use an Atmega328P-PU add 1 16Mhz quartz and two ceramic condensator 22 Pn
 * MAX7219 or MAX7221
 * 3 7segment digit anode common
 * magnet sensor A3144E 
 */

#include "LedControl.h"
LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, CS, number of 7 segments
int magnetSensor = 2;

int inchDiameter = 26;    //!!! enter inch of your wheel diameter !!!//
double cmDiameter = 0;

double cmPerimeter = 0;

double timeLastCheck = 0;

double speedCmPerCentSec = 0.0;
double speedMeterSecond = 0.0;
double speedKilometerHour = 0.0;

void setup() {
  lc.shutdown(0, false); //0 number of max7219 cascading
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  lc.setDigit(0, 0, 0, false);
  lc.setDigit(0, 1, 0, false);
  lc.setDigit(0, 2, 0, false);
  pinMode(magnetSensor, INPUT);
  Serial.begin(9600);
  cmDiameter = (double)inchDiameter / 0.39370;    // inch to centimeter
  //Serial.println(cmDiameter);
  cmPerimeter = (double)PI * cmDiameter;
  //Serial.println(cmPerimeter);
  timeLastCheck = millis();
  writeNumber(0);
  delay(100);
}

void loop() {
  if (digitalRead(magnetSensor) == 0) { // &&
    speedMeterSecond = (cmPerimeter / 100) / ((double)millis() / 1000 - timeLastCheck); // m/s = metre/(tempActuel(seconde)-dernierTmpEnregistré(seconde))
    timeLastCheck = (double)millis() / 1000;
    speedKilometerHour = speedMeterSecond * 3.6;
    writeNumber((int)speedKilometerHour);
  } else {
    if (((double)millis() / 1000 - timeLastCheck) >= 5) {
      writeNumber(0);
    }
  }

  delay(15);
}

void writeNumber(int number) {  //max 999 | 1342 -> 342
  int digit[] = {0, 0, 0};

  digit[0] = number % 10;
  digit[1] = number / 10 % 10;
  digit[2] = number / 100 % 10;

  for (int i = 0; i < sizeof(digit); i++) {
    lc.setDigit(0, i, digit[i], false);
  }
}
