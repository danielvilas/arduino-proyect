#include <chal74hc595.h>

//**************************************************************//
//  Name    : shiftOutCode, Hello World                                
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis 
//  Date    : 25 Oct, 2006    
//  Modified: 23 Mar 2010                                 
//  Version : 2.0                                             
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                           
//****************************************************************

Chal74hc595 reg = Chal74hc595(1,2);



void setup() {
  //set pins to output so you can control the shift register
  reg.setup  (11,12,8);
}

void loop() {
  /*
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(500);
  }*/
  int a[]={0xFF,0xFF};
  reg.shift(a);
  delay(500);
  a[0]=0b10101010;
  a[1]=a[0];
  reg.shift(a);
  delay(500);
  a[0]=0b01010101;
  a[1]=a[0];
  reg.shift(a);
  delay(500);
  a[0]=1;
  a[1]=0;
  reg.shift(a);
  for(int i=0;i<16;i++){
    a[0]=a[0]<<1;
    a[1]=a[1]<<1;
    if(i==7)a[1]=1;
    reg.shift(a);
    delay(250);
  }
  delay(2-50);
}
