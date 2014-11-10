const int c4a=2;
const int c3a=4;
const int c34e=3;

void setup(){
    pinMode(c4a, OUTPUT);
  pinMode(c3a, OUTPUT);
  pinMode(c34e, OUTPUT);

  // pull the enable pin LOW to start
  digitalWrite(c34e, LOW);
}

void loop(){
    digitalWrite(c4a, HIGH);
    digitalWrite(c3a, LOW);
    analogWrite(c34e,255);
    delay(500);
    digitalWrite(c3a, HIGH);
    digitalWrite(c4a, LOW);
    analogWrite(c34e,255);
    delay(500);
}
