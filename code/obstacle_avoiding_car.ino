#include <Servo.h>

Servo myservo;

// Ultrasonic pins
#define trigPin 9
#define echoPin 10

// L298N pins
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5

// Get averaged distance
int getDistance() {
  int total = 0;

  for (int i = 0; i < 3; i++) {

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000);

    int distance = duration * 0.034 / 2;

    if (distance == 0)
      distance = 400;

    total += distance;

    delay(20);
  }

  return total / 3;
}

// Forward
void forward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Stop
void stopRobot() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Turn Left
void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  delay(450);

  stopRobot();
}

// Turn Right
void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  delay(450);

  stopRobot();
}

void setup() {

  Serial.begin(9600);

  myservo.attach(11);
  myservo.write(90);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  delay(1000);
}

void loop() {

  // Look forward
  myservo.write(90);
  delay(350);

  int frontDistance = getDistance();
  
if(frontDistance < 10){
  stopRobot();
  delay(200);
}

  Serial.print("Front: ");
  Serial.println(frontDistance);

  // No obstacle
  if (frontDistance > 40) {

    forward();
  }
  else {

    stopRobot();
    delay(300);

    // Look Left
    myservo.write(170);
    delay(500);
    int leftDistance = getDistance();

    // Center
    myservo.write(90);
    delay(200);

    // Look Right
    myservo.write(10);
    delay(500);
    int rightDistance = getDistance();

    // Center
    myservo.write(90);
    delay(200);

    Serial.print("Left: ");
    Serial.println(leftDistance);

    Serial.print("Right: ");
    Serial.println(rightDistance);

    if (leftDistance > rightDistance) {
      turnLeft();
    } else {
      turnRight();
    }
  }
}
