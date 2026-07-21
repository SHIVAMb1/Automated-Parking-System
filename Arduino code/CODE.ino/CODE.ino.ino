#include <ESP32Servo.h>

const int sensorPin1 = 5;
const int sensorPin2 = 18;
const int sensorPin3 = 27;
const int sensorPin4 = 26;

Servo servo1;
Servo servo2;

const int servoPin1 = 19;
const int servoPin2 = 25;

int occupiedSlots = 0;
const int maxSlots = 10;

bool sensor1Triggered = false;
bool sensor2Triggered = false;
bool sensor3Triggered = false;
bool sensor4Triggered = false;

void setup() {
    Serial.begin(115200);

    servo1.attach(servoPin1);
    servo2.attach(servoPin2);

    pinMode(sensorPin1, INPUT);
    pinMode(sensorPin2, INPUT);
    pinMode(sensorPin3, INPUT);
    pinMode(sensorPin4, INPUT);

    servo1.write(0);
    servo2.write(0);

    Serial.println("10 slots are available.");
}

void loop() {

    int sensorValue1 = digitalRead(sensorPin1);
    int sensorValue2 = digitalRead(sensorPin2);
    int sensorValue3 = digitalRead(sensorPin3);
    int sensorValue4 = digitalRead(sensorPin4);

    // Vehicle Entry Detection
    if (sensorValue1 == HIGH && !sensor1Triggered) {

        sensor1Triggered = true;

        if (occupiedSlots < maxSlots) {
            servo1.write(90);
            occupiedSlots++;

            Serial.println(String(maxSlots - occupiedSlots) + " slots are available.");
        } else {
            Serial.println("Vehicle detected - All slots are full");
        }

        delay(2000);

    } else if (sensorValue1 == LOW) {
        sensor1Triggered = false;
    }

    // Entry Gate Close
    if (sensorValue2 == HIGH && !sensor2Triggered) {

        sensor2Triggered = true;

        servo1.write(0);

        delay(2000);

    } else if (sensorValue2 == LOW) {
        sensor2Triggered = false;
    }

    // Vehicle Exit Detection
    if (sensorValue3 == HIGH && !sensor3Triggered) {

        sensor3Triggered = true;

        if (occupiedSlots > 0) {

            servo2.write(90);

            occupiedSlots--;

            Serial.println(String(maxSlots - occupiedSlots) + " slots are available.");

        } else {

            Serial.println("Vehicle detected - All slots are empty");
        }

        delay(2000);

    } else if (sensorValue3 == LOW) {

        sensor3Triggered = false;
    }

    // Exit Gate Close
    if (sensorValue4 == HIGH && !sensor4Triggered) {

        sensor4Triggered = true;

        servo2.write(0);

        delay(2000);

    } else if (sensorValue4 == LOW) {

        sensor4Triggered = false;
    }

    delay(100);
}
