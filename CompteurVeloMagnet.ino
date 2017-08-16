#include "LedControl.h"
LedControl lc = LedControl(12,11,10,3);  //DIN, CLK, CS, nombre de 7 segments
int magnetSensor = 2;

int inchDiametre = 26;
double cmDiametre = 0;

double cmPourtour = 0;

double timeLastCheck = 0;

double speedCmPerCentSec = 0.0;
double speedMeterSecond = 0.0;
double speedKilometerHour = 0.0;

void setup() {
  lc.shutdown(0,false); //0 le nombre qui defini l'emplacement si il y a un chainage de max7219
  lc.setIntensity(0,2);
  lc.clearDisplay(0);
  lc.setDigit(0,0,'-', false);
  lc.setDigit(0,1,'-', false);
  pinMode(magnetSensor, INPUT);
  Serial.begin(9600);
  cmDiametre = (double)inchDiametre/0.39370;
  //Serial.println(cmDiametre);
  cmPourtour = (double)PI * cmDiametre;
  //Serial.println(cmPourtour);
  timeLastCheck = millis();
  pinMode(13, OUTPUT);
  digitalWrite(13, 1);
  writeNumber(123);
  delay(500);
  for(int i = 10; i > 0; i--){
    writeNumber(i);
    delay(250);
  }
  writeNumber(0);
  delay(2000);
}

void loop() {
  if(digitalRead(magnetSensor) == 0 && ((double)millis()-timeLastCheck*1000) > 5){
    //Serial.println((double)millis()/1000-timeLastCheck);
    speedMeterSecond = (cmPourtour/100)/((double)millis()/1000-timeLastCheck);
    timeLastCheck = (double)millis()/1000;
    //speedMeterSecond = speedCmPerCentSec/100;
    speedKilometerHour = speedMeterSecond*3.6;
    writeNumber((int)speedKilometerHour);
    digitalWrite(13, 1);
    delay(10);
  }else{
    if(millis() % 500 == 0){
      digitalWrite(13, 0);
    }
  }

  if((double)millis()/1000-timeLastCheck > 2){
    writeNumber(0);
  }
}

void writeNumber(int number){
  int digit[] = {0, 0, 0};
  int stat = 0;
  int i = 10;
  while (number > i / 10){
      digit[stat] = (number % i - number % (i / 10))/(i/10);
      i *= 10;
      stat++;
    }
    
  for(int i = 0; i < sizeof(digit); i++){
    lc.setDigit(0,i,digit[i], false);
  }
}

/*void writeNumber(int number){
  lc.setDigit(0,0,(int)(number/10), false);
  lc.setDigit(0,1,number-(int)(number/10)*10, false);
}*/
