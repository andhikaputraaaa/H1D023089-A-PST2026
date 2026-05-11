#include <Servo.h> 

Servo myservo;

const int ledPin = 7;
const int btnPin = 2;
const int servoPin = 8;

int pos = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT_PULLUP);
  myservo.attach(servoPin);
}

void loop() {
  bool btnClick = digitalRead(btnPin);
  if (btnClick == LOW){
    pos = 90;
    myservo.write(pos);
    digitalWrite(ledPin, HIGH);
    Serial.println("DRS: ACTIVE");
  } else {
    pos = 0;
    myservo.write(pos);
    digitalWrite(ledPin, LOW);
    Serial.println("DRS: CLOSED");
  }
}