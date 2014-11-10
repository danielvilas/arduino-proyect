#include <SoftwareSerial.h>

#define redPin 3
#define greenPin 6
#define bluePin 5

int red=0;
int green=0;
int blue=0;


SoftwareSerial mySerial(12, 13); // RX, TX

void setup()
{
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  
  Serial.begin(9600); //set baud rate
  mySerial.begin(9600);
  
  analogWrite(redPin,red);
  analogWrite(greenPin,green);
  analogWrite(bluePin,blue);
}
 
void loop()
{
  /*
  while(Serial.available())
  {//while there is data available on the serial monitor
    message+=char(Serial.read());//store string from serial command
  }
  if(!Serial.available())
  {
    if(message!="")
    {//if data is available
      Serial.println(message); //show the data
      message=""; //clear the data
    }else{
      Serial.println(tmp);
      tmp=(tmp+1)%255;
    }
  }
  delay(1000); //delay
*/

  if (mySerial.available()){
    //Serial.println("BT");
    char data = mySerial.read();
    if('R'==data){
       mySerial.print(red);
       mySerial.print(',');
       mySerial.print(green);
       mySerial.print(',');
       mySerial.println(blue);
       Serial.println("BT Asked data");
    }else if('S'==data){
       Serial.println("BT Sets Data");
       red = mySerial.parseInt();
       Serial.print("R:");
       Serial.println(red);
       green = mySerial.parseInt();
       Serial.print("G:");
       Serial.println(green);
       blue = mySerial.parseInt();
       Serial.print("B:");
       Serial.println(blue);
       red = constrain(red, 0, 255);
      green = constrain(green, 0, 255);
      blue = constrain(blue, 0, 255);
      setColor(red,green,blue);
    }else {
      Serial.write(data);
    }
  }
  if (Serial.available()){
    char data = Serial.read();
    if('R'==data){
       Serial.print(red);
       Serial.print(',');
       Serial.print(green);
       Serial.print(',');
       Serial.println(blue);
    }else if('S'==data){
      Serial.println("Parsing");
       red = Serial.parseInt();
       Serial.print("R:");
       Serial.println(red);
       green = Serial.parseInt();
       Serial.print("G:");
       Serial.println(green);
       blue = Serial.parseInt();
       Serial.print("B:");
       Serial.println(blue);
       red = constrain(red, 0, 255);
      green = constrain(green, 0, 255);
      blue = constrain(blue, 0, 255);
      setColor(red,green,blue);
    }else {
      mySerial.write(data);
    }
  }
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

