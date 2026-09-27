#include <Servo.h>
#include <IRremote.hpp>

// -------- PINS --------
const int SERVO_PIN = 9;
const int RED_LED = 7;
const int GREEN_LED = 12;
const int IR_PIN = 2;

// Remote POWER button
const uint8_t UNLOCK_BUTTON = 0x45;

// -------- LOCK SETTINGS --------
const int LOCKED_ANGLE = 0;
const int UNLOCKED_ANGLE = 90;

// Stay unlocked for 5 seconds
const int UNLOCK_TIME = 5000;

Servo lockServo;

void setup() {
  Serial.begin(9600);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  lockServo.attach(SERVO_PIN);

  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  // Start locked
  lockServo.write(LOCKED_ANGLE);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  Serial.println("Rinder Smart Lock Ready");
}

void unlockDoor() {
  Serial.println("UNLOCKED");

  // Unlock
  lockServo.write(UNLOCKED_ANGLE);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  // Stay unlocked for 5 seconds
  delay(UNLOCK_TIME);

  // Lock again
  lockServo.write(LOCKED_ANGLE);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);

  Serial.println("LOCKED");
}

void loop() {

  // -------- IR REMOTE --------
  if (IrReceiver.decode()) {

    uint8_t command = IrReceiver.decodedIRData.command;

    // POWER button
    if (command == UNLOCK_BUTTON) {

      Serial.println("Remote POWER button pressed");

      IrReceiver.resume();

      unlockDoor();

      return;
    }

    IrReceiver.resume();
  }

  // -------- FASTAPI / WEBSITE --------
  if (Serial.available() > 0) {

    char command = Serial.read();

    // FastAPI sends U
    if (command == 'U' || command == 'u') {

      Serial.println("Website unlock request");

      unlockDoor();
    }
  }
}