#include <Servo.h>

const int trigPin1 = 2;  // Trigger pin for person detection (U1)
const int echoPin1 = 3;  // Echo pin for person detection (U1)
const int trigPin2 = 5;  // Trigger pin for bin level detection (U2)
const int echoPin2 = 6;  // Echo pin for bin level detection (U2)
const int redLED = 11;    // Red LED pin (indicating bin is full)
const int BLUELED = 10;    // Blue LED pin (indicating bin is functioning) 
const int greenLED = 12;  // Green LED pin (indicating bin is not full)
const int servoPin = 8;  // Servo motor pin (controls the lid)

Servo lidServo; // Create Servo object
const int lidOpenAngle = 180;  // Lid open angle for servo motor
const int lidCloseAngle = 0;  // Lid close angle for servo motor

// Function to measure distance using Ultrasonic sensor
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;  // Calculate distance in cm
  return distance;
}

void setup() {
  // Pin modes
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(BLUELED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // Initialize Servo
  lidServo.attach(servoPin);
  lidServo.write(lidCloseAngle);  // Keep lid closed at the start
  
  // Start Serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  long personDistance = measureDistance(trigPin1, echoPin1);  // Person detection (U1)
  long binLevelDistance = measureDistance(trigPin2, echoPin2);  // Bin level detection (U2)

  // Check if the bin is not full
  if (binLevelDistance >= 5) {  
    digitalWrite(greenLED, HIGH); 
    digitalWrite(redLED, LOW);     
    digitalWrite(BLUELED, LOW);

    // Check if a person is detected within range
    if (personDistance >= 2 && personDistance <= 50)  
    {
      Serial.println("Person detected!");
      Serial.print("Person Distance: ");
      Serial.println(personDistance);
      digitalWrite(BLUELED, HIGH);     // Blue LED on
      delay(1000); // Wait 1 seconds to allow person to reach the bin
      lidServo.write(lidOpenAngle);  // Open the lid
      Serial.println("Bin is not full. Lid opened!");
      delay(6000);  // Keep the lid open for 8 seconds
      lidServo.write(lidCloseAngle);  // Close the lid
      digitalWrite(BLUELED, LOW);     // Blue LED off
      Serial.println("Lid closed.");
    }

    else  {
      lidServo.write(lidCloseAngle);  // Ensure the lid stays closed
    
    }
  } else {
    
    digitalWrite(redLED, HIGH);    
    digitalWrite(BLUELED, LOW);    
    digitalWrite(greenLED, LOW);   
    lidServo.write(lidCloseAngle);  
  }
}