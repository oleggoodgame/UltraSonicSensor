#define ECHO_PIN 3
#define TRIGGER_PIN 4
#define LED_RED 10
#define LED_YELLOW 11
#define LED_GREEN 12

unsigned long lastTimeTrig = millis();
int triggerDelay = 100;
int limitToYellow = 190;
int limitToRed = 70;

volatile unsigned long beginPulse;
volatile unsigned long endPulse;
volatile bool newDistance= false;

void triggerUltraSonicSensor();
double getDistance();
void echoPinInterrupt();
void setUpLed();
void lightLed(double distance);
void setup()
{
    Serial.begin(9600);
    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIGGER_PIN, OUTPUT);
  setUpLed();
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN),echoPinInterrupt, CHANGE);
}

void loop()
{
    unsigned long timeNow = millis();
    
    if (timeNow - lastTimeTrig > triggerDelay)
    {
        lastTimeTrig += triggerDelay;
        triggerUltraSonicSensor();
        
    }
  
  if(newDistance){
  	Serial.print("Distance: ");
    double distance = getDistance();
    Serial.println(distance);
	lightLed(distance);
    newDistance = false;
  }
}


void setUpLed(){
	pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
}
void triggerUltraSonicSensor()
{
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
}


double getDistance()
{
    unsigned long timeBefore = millis();
    double duration = endPulse-beginPulse;
    unsigned long timeNow = millis();
    unsigned long time = timeNow - timeBefore;

    Serial.print("as if pulseIn(ECHO_PIN, HIGH) time:");
    Serial.println(time);
    double distance = duration * 0.036/2;

    return distance;
}

void lightLed(double distance){
  if(distance>limitToYellow){
  	digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
  }
  else if(distance<=limitToYellow && distance>limitToRed){
  	digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
        digitalWrite(LED_YELLOW, HIGH);

  }
  else{
  digitalWrite(LED_GREEN, LOW);
   digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, LOW);}
}


// double getDistance()
// {
//     unsigned long timeBefore = millis();
//     double duration = pulseIn(ECHO_PIN, HIGH);
//     unsigned long timeNow = millis();

//     unsigned long time = timeNow - timeBefore;

//     Serial.print("pulseIn(ECHO_PIN, HIGH) time:");
//     Serial.println(time);
//     double distance = duration * 0.017;

//     return distance;
// }
void echoPinInterrupt(){
  if(digitalRead(ECHO_PIN)==HIGH){
    beginPulse = micros();
  }
  else{
  	endPulse = micros();
    newDistance = true;
  }
}
