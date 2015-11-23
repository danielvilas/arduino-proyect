/*
  An example analogue/digital clock using a TFT LCD screen to show the 
  use of some of the drawing commands with the Adafruit_ST7735 library.
  For a more accurate clock, it would be better to use the RTClib library.
  But this is just a demo. 

  This examples uses the hardware SPI only. Non-hardware SPI
  is just too slow (~8 times slower!)

  Gilchrist 6/2/2014 1.0
*/

#define P_sclk 13  // Don't change
#define mosi 11  // Don't change
#define TFT_cs   9
#define dc   8
#define rst  7  // you can also connect this to the Arduino reset
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include <Thermocouple.h>

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_cs, dc, rst);       // Invoke custom library

Thermocouple tc1 = Thermocouple(3); 
// High when a value is ready to be read
volatile bool readFlag;

// Value to store analog result
volatile int analogVal;
volatile uint8_t cnt=0;
float temperatureMean=0;
uint8_t temperatureCount=0;

void setup_timer2() {
  TCCR2B = 0x00;        //Disbale Timer2 while we set it up
  TCNT2 = 0;
  //Reset Timer Count to 130 out of 255
  TIFR2 = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
  TIMSK2 = 0x02;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
  TCCR2A = 0x00;        //Timer2 Control Reg A: Wave Gen Mode normal
  TCCR2B = (1 << CS22) | (1 << CS20); //Timer2 Control Reg B: Timer Prescaler set to 128 cnt = 0;
  OCR2A = 125; //1MS
}

void setup(void) {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("failed!");
    //return;
  }
  Serial.println("OK!");
  tft.initR(INITR_BLACKTAB); 
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);  // Adding a black background colour erases previous text automatically
  tft.setCursor (2, 2);
  tft.print("TEMP: ");
  tft.drawFastVLine(5,11,112,ST7735_WHITE);
  tft.drawFastHLine(5,123,150,ST7735_WHITE);


  setupAdc();
  setup_timer2();
  sei();
}

void setupAdc(){
  // clear ADLAR in ADMUX (0x7C) to right-adjust the result
  // ADCL will contain lower 8 bits, ADCH upper 2 (in last two bits)
  ADMUX &= B11011111;
  
  // Set REFS1..0 in ADMUX (0x7C) to change reference voltage to the
  // proper source (01)
  ADMUX |= B01000000;
  
  // Clear MUX3..0 in ADMUX (0x7C) in preparation for setting the analog
  // input
  ADMUX &= B11110000;
  
  ADMUX |= 0;//A0
  // ADMUX |= B00001000; // Binary equivalent
  
  // Set ADEN in ADCSRA (0x7A) to enable the ADC.
  // Note, this instruction takes 12 ADC clocks to execute
  ADCSRA = B10000000;

  // Set the Prescaler to 128 (16000KHz/128 = 125KHz)
  // Above 200KHz 10-bit results are not reliable.
  ADCSRA |= B00000111;
  
  // Set ADIE in ADCSRA (0x7A) to enable the ADC interrupt.
  // Without this, the internal interrupt will not trigger.
  ADCSRA |= B00001000;
  
  readFlag = 0;
  // Set ADSC in ADCSRA (0x7A) to start the ADC conversion
  //ADCSRA |=B01000000;
}

void loop() {
 //getting the voltage reading from the temperature sensor
 if(readFlag==1){
  int reading = analogVal;  
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 5.0;
 voltage /= 1024.0; 
 
 // now print out the temperature
 //float temperatureC = (voltage - 0.550) * 100 ;  //converting from 10 mv per degree wit 750 mV offset
                                               //to degrees ((voltage - 750mV) times 100)
  float temperatureC = (voltage - 0.550) * 20 ;  //Tomaremos 5 muestras de 50ms, para considear una Tmp OL
  temperatureMean+=temperatureC;
  temperatureCount++;
  if(temperatureCount==5){
    //Serial.print(millis());
    //Serial.print("\t");
    //Serial.println(temperatureMean);
    tft.setTextColor(ST7735_RED, ST7735_BLACK);  
    tft.setCursor (2+6*7, 2);
    tft.print(temperatureMean);
    temperatureMean=0;
    temperatureCount=0;
    tft.setCursor(2+12*7+2,2);
    float max66775 = tc1.readC();
    tft.print(max66775);
    
  } 
  readFlag=0;
 }
}

ISR(TIMER2_COMPA_vect) {
  //digitalWrite(13, LOW);
  cnt++;
  TCNT2 = 0;
  if (cnt == 50) { // 50ms
    uint8_t tmp = SREG;
    cli();
    ADCSRA |=B01000000;
    SREG = tmp;
    cnt = 0;
  }

}


// Interrupt service routine for the ADC completion
ISR(ADC_vect){

  // Done reading
  readFlag = 1;
  
  // Must read low first
  analogVal = ADCL | (ADCH << 8);
}
