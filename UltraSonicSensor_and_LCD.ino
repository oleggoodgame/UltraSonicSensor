#include <LiquidCrystal.h>
#define LCD_RS_PIN A5
#define LCD_E_PIN A4
#define LCD_D4_PIN 6
#define LCD_D5_PIN 5
#define LCD_D6_PIN 4
#define LCD_D7_PIN 3

#define TRIGGER_PIN 8
#define ECHO_PIN 2

LiquidCrystal lcd(LCD_RS_PIN, LCD_E_PIN, LCD_D4_PIN,
                  LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

unsigned long lastTimeTrig = millis();
int triggerDelay = 100;

volatile unsigned long startHearing;
volatile unsigned long endHearing;
volatile bool ifMeasured = false;


void triggerUltraSonicSensor();
void echoInterrupt();
double getDistance();
void printOnLCD(double distance);

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);
	pinMode(TRIGGER_PIN,OUTPUT);
  pinMode(ECHO_PIN, INPUT);
Serial.println("Start");
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN),echoInterrupt, CHANGE);
  lcd.begin(16, 2);
}

void loop() {
  unsigned long timeNow = millis();
    if(timeNow - lastTimeTrig >triggerDelay){
  	lastTimeTrig+=triggerDelay;
      triggerUltraSonicSensor();
  }
  
  if(ifMeasured){
    double distance = getDistance();
    printOnLCD(distance);
  	ifMeasured = false;
  }
  //if (Serial.available() > 0) {
  //  String text = Serial.readString();
  //  printUserTextOnDisplay(text);
  //}
}

void triggerUltraSonicSensor(){
	digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
}
void echoInterrupt(){
  if(digitalRead(ECHO_PIN)==HIGH){
    startHearing = micros();
  }
  else{  	
endHearing = micros();
    ifMeasured = true;
  }
}

double getDistance()
{
    double duration = endHearing-startHearing;
    double distance = duration * 0.0343 / 2;
Serial.println(distance);
    return distance;
}

void printOnLCD(double distance){
  unsigned long time = endHearing-startHearing;
  lcd.clear();
  lcd.setCursor(0,0);      
 lcd.print("Distance:");
  lcd.setCursor(9,0);      
  lcd.print(distance);
  lcd.setCursor(0,1);      
  lcd.print("Time:");
  lcd.setCursor(5,1);      
  lcd.print(time);
}
// void printUserTextOnDisplay(String text)
// {
//   if (text.length() > 16) {
//     text = "Text too long.";
//   }
//   for (int i = text.length(); i < 16; i++ ) {
//     text += " ";
//   }
//   // set cursor line O or 1
//   lcd.setCursor(0, cursorLine);
//   // print text
//   lcd.print(text);
//   if (cursorLine == 0) {
//     cursorLine = 1;
//   }
//   else {
//     cursorLine = 0;
//   }
// }
