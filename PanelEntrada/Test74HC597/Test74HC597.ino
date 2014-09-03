//define where your pins are
int PlPin = 8;
int StcpPin = 9;
int ShcpPin = 10;
int QPin=11;

//Define variables to hold the data 
//for each shift register.
//starting with non-zero numbers can help
//troubleshoot
byte switchVar1 = 72;  //01001000

void setup() {
  //start serial
  Serial.begin(9600);

  //define pin modes
  pinMode(PlPin, OUTPUT);
  pinMode(StcpPin, OUTPUT);
  pinMode(ShcpPin, OUTPUT);
  pinMode(QPin, INPUT);

  digitalWrite(PlPin,1);
  digitalWrite(StcpPin,0);
}

void loop() {

  //Un pulso de reloj de Lacth para capturar la entrada
  digitalWrite(StcpPin,0);
  digitalWrite(StcpPin,1);
  digitalWrite(StcpPin,0);
 //Otro pulso de reloj latch para setear 
  digitalWrite(PlPin,1);
  digitalWrite(PlPin,0);
  digitalWrite(PlPin,1);
  // Dejamos la cfg para shift
  digitalWrite(PlPin,1);
  digitalWrite(StcpPin,0);
  
  switchVar1 = shiftIn(QPin, ShcpPin);
  
  Serial.println(switchVar1, BIN);
 

Serial.println("-------------------");
delay(500);

}

byte shiftIn(int myDataPin, int myClockPin) { 
  int i;
  int temp = 0;
  int pinState;
  byte myDataIn = 0;
  digitalWrite(myClockPin, 0);
  for (i=7; i>=0; i--)
  {
    temp = digitalRead(myDataPin);
    if (temp) {
      pinState = 1; 
      //set the bit to 0 no matter what
      myDataIn = myDataIn | (1 << i);
    }
    else {
      pinState = 0;
    }

    digitalWrite(myClockPin, 1);
    digitalWrite(myClockPin, 0);

  }
  return myDataIn;
}
