#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

SoftwareSerial Serial1=SoftwareSerial(10,11);

int HA1=3,HB1=4,HEN1=5;
int HA2=7,HB2=8,HEN2=9;

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
    updateCh2(HIGH,LOW,255);
    updateCh1(HIGH,LOW,255);
    readCurrent();
    delay(500);
  
    updateCh2(LOW,HIGH,200);
    updateCh1(LOW,HIGH,200);
    readCurrent();
    delay(500);
    
    boolean isError = digitalRead(ERR);
    autoReset(isError);
    showStatus(isError);
}

void readCurrent(){
    byte valVf=analogRead(VF);
    lcd.setCursor(0,1);
    lcd.print("Read: ");
    lcd.print(valVf);
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

void updateCh2(boolean a, boolean b, byte en){
    digitalWrite(HA2, a);
    digitalWrite(HB2, b);
    analogWrite(HEN2,en);
}

void updateCh1(boolean a, boolean b, byte en){
    digitalWrite(HA1, a);
    digitalWrite(HB1, b);
    analogWrite(HEN1,en);
}
