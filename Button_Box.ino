//Buttons code
//by AJ 4/7/2022

#include <Wire.h>
#include "REC.h"

#define jogQButton      1
#define jogLTButton     2
#define jogDTButton     3
#define jogBridgeButton 4


int jQButtonState;
int lastJQButtonState = LOW;

int jLTButtonState;
int lastJLTButtonState = LOW;

int jDTButtonState;
int lastJDTButtonState = LOW;

int jBButtonState;
int lastJBButtonState = LOW;

long lastQDebounce = 0;
long lastLTDebounce = 0;
long lastDTDebounce = 0;
long lastBDebounce = 0;
long debounceDelay = 50;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(jogQButton, INPUT);
  pinMode(jogLTButton, INPUT);
  pinMode(jogDTButton, INPUT);
  pinMode(jogBridgeButton, INPUT);
  pinMode(stopButton, INPUT);

  
}

void loop() {
  int readingJogQ = digitalRead(jogQButton);
  int readingJogLT = digitalRead(jogLTButton);
  int readingJogDT = digitalRead(jogDTButton);
  int readingJogB = digitalRead(jogBridgeButton);

  if(readingJogQ != lastJQButtonState){
    lastQDebounce = millis();
  }
  if(readingJogLT != lastJLTButtonState){
    lastLTDebounce = millis();
  }
  if(readingJogDT != lastJDTButtonState){
    lastDTDebounce = millis();
  }
  if(readingJogB != lastJBButtonState){
    lastBDebounce = millis();
  }

  if((millis() - lastQDebounceTime) > debounceDelay) {
    if(readingJogQ != jQButtonState){
      jQButtonState = readingJogQ;
    }
  }

  if((millis() - lastLTDebounceTime) > debounceDelay) {
    if(readingJogLT != jLTButtonState){
      jLTButtonState = readingJogLT;
    }
  }

  if((millis() - lastDTDebounceTime) > debounceDelay) {
    if(readingJogDT != jDTButtonState){
      jDTButtonState = readingJogDT;
    }
  }

  if((millis() - lastBDebounceTime) > debounceDelay) {
    if(readingJogB != jBButtonState){
      jBButtonState = readingJogB;
    }
  }

  

}
