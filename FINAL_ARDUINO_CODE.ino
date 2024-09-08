#define BLYNK_TEMPLATE_ID "TMPL68o0UcinY"
#define BLYNK_AUTH_TOKEN "Ew28tAVt8PpRWNlihfZIo3USmiazjaaG"

#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#include <Stepper.h>

// WiFi credentials
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 38400

ESP8266 wifi(&EspSerial);

const int stepsPerRevolution = 2048;   // Steps per revolution for 28BYJ-48 motor
const int rainPin = 4;                // Pin connected to the rain sensor
const int motorPin1 = 8;              // Stepper motor pin 1
const int motorPin2 = 9;              // Stepper motor pin 2
const int motorPin3 = 10;             // Stepper motor pin 3
const int motorPin4 = 11;             // Stepper motor pin 4

Stepper myStepper(stepsPerRevolution, motorPin1, motorPin3, motorPin2, motorPin4);
boolean isMoving = false;              // Flag to track motor movement status
boolean hasRainStarted = false;        // Flag to track rain detection status

void setup() {
  Serial.begin(115200);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass, "blynk.cloud", 80);   // Connect to Blynk server

  pinMode(rainPin, INPUT);            // Set the rain sensor pin as input
  myStepper.setSpeed(20);             // Set the stepper motor speed in RPM
}

void loop() {
  Blynk.run();                        // Run Blynk background tasks

  if (!isMoving) {
    if (digitalRead(rainPin) == LOW && !hasRainStarted) {
      rotateClockwise();
      hasRainStarted = true;
      Blynk.setProperty(V1, "onLabel", "Rain Detected!");  // Update the label on the Value Display widget
      Blynk.virtualWrite(V1, 255);  // Set the Value Display widget to ON
    } else if (digitalRead(rainPin) == HIGH && hasRainStarted) {
      rotateCounterClockwise();
      hasRainStarted = false;
      Blynk.setProperty(V1, "onLabel", "No Rain");  // Update the label on the Value Display widget
      Blynk.virtualWrite(V1, 0);  // Set the Value Display widget to OFF
    }
  }
}

void rotateClockwise() {
  isMoving = true;
  myStepper.step(stepsPerRevolution);
  delay(1000);
  isMoving = false;
}

void rotateCounterClockwise() {
  isMoving = true;
  myStepper.step(-stepsPerRevolution);
  delay(1000);
  isMoving = false;
}
