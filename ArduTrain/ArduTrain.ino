#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <chall293.h>
#include "ArduTrain.h"


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

SoftwareSerial Serial1=SoftwareSerial(10,11);

int HA1=3,HB1=4,HEN1=5;
int HA2=7,HB2=8,HEN2=9;
ChalL293 l293(HA1,HB1,HEN1,  HA2,HB2,HEN2);


int ERR=2,VF=A0;
int RESET=6;

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);  // Used to type in characters

  lcd.begin(20,4);  
  lcd.clear();
    for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight();
  
  lcd.setCursor(3,0); //Start at character 4 on line 0
  lcd.print("Hello, world!");

  pinMode(HA1, OUTPUT);
  pinMode(HB1, OUTPUT);
  pinMode(HEN1, OUTPUT);

  // pull the enable pin LOW to start
  digitalWrite(HEN1, LOW);

  pinMode(HA2, OUTPUT);
  pinMode(HB2, OUTPUT);
  pinMode(HEN2, OUTPUT);

  // pull the enable pin LOW to start
  digitalWrite(HEN2, LOW);


  pinMode(VF,INPUT);
  pinMode(ERR, INPUT);
  digitalWrite(ERR,LOW);
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET,LOW);
}

void autoReset(boolean isError);

int cnt=0;
void loop(){
    //updateCh2(l293_forward,255);
    //updateCh1(l293_forward,255);
    //readCurrent();
    //delay(500);
  
    //updateCh2(l293_backward,200);
    //updateCh1(l293_backward,200);
    readCurrent();
    //delay(500);
    
    boolean isError = digitalRead(ERR);
    autoReset(isError);
    showStatus(isError);
    
    lcd.setCursor(0,3);
    if(Serial.available()){
      while(Serial.available()){
        char c = Serial.read();
        Serial1.write(c);
        //lcd.write(c);
      }
      delay(100);
      lcd.setCursor(0,3);
     while(Serial1.available()){
        char c = Serial1.read();
        Serial.write(c);
        //lcd.write(c);
      }
    }
    
    readCommand();
}

void readCommand(){
  char cmd[25];
  if(Serial1.available()){
    if(Serial1.available()<25){
      Serial.print("Available: ");
      Serial.println(Serial1.available());
    }else{
      Serial.print("Available: ");
      Serial.println(Serial1.available());
      int r = Serial1.readBytes(&cmd[0],25);
      Serial.print("Read: ");
      Serial.println(r);
      Serial.println(cmd);
    }
  }
}

long lastRead=0;
void readCurrent(){
    long currentMillis= millis();
    if(currentMillis - lastRead>=500){
      //Serial.print("reading from ");
      //Serial.print(currentMillis);
      //Serial.print(" - ");
      //Serial.println(lastRead);      
      byte valVf=analogRead(VF);
      lcd.setCursor(0,1);
      lcd.print("Read: ");
      lcd.print(valVf);
      lcd.print("   ");
      lastRead=currentMillis;
    }
}

void autoReset(boolean isError){
     if(cnt==11){
      lcd.setCursor(0,3);
      lcd.print("         ");
      delay(100);
      cnt=0;
    }
    if(isError&&cnt==10){
      lcd.setCursor(0,3);
      lcd.print("AutoReset");
      //cnt=11;
      digitalWrite(RESET,HIGH);
      delay(100);
      digitalWrite(RESET,LOW);
    }
}

void showStatus(boolean isError){
      if(isError){
      lcd.setCursor(0,2);
      lcd.print("Error!");
      cnt++;
    }else{
      lcd.setCursor(0,2);
      lcd.print("OK    ");
      cnt=0;
    }
}

void updateCh2(L293_MODE mode, byte en){
    l293.setModeCh2(mode);
    l293.setSpeedCh2(en);
}

void updateCh1(L293_MODE mode, byte en){
    l293.setModeCh1(mode);
    l293.setSpeedCh1(en);
}
