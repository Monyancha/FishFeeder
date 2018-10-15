#include <Stepper.h>
#include "Feed.h"
#include "Arduino.h"

Feed::Feed(double _poweringHour, double _feedHour)
    : poweringHour(_poweringHour), feedHour(_feedHour) {
}

void Feed::Init() {
    InitPins();
    InitTimer();
    InitStepMotor();
    feeded = false;
    TurnLedRed();
}

void Feed::InitPins() {
    pinMode(pinManualFeedCmd, INPUT_PULLUP);
    pinMode(pinGreenLed, OUTPUT);
    pinMode(pinRedLed, OUTPUT);
    pinMode(feedQuantity, INPUT);
}

void Feed::InitTimer() {
    noInterrupts(); // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    OCR1A = 62500;           // compare match register 16MHz/256/2Hz
    TCCR1B |= (1 << WGM12);  // CTC mode
    TCCR1B |= (1 << CS12);   // 256 prescaler
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
    interrupts();            // enable all interrupts
}

void Feed::InitStepMotor() {
    small_stepper = new Stepper(stepsPerMotorRevolution, 8, 10, 9, 11);
    small_stepper->setSpeed(200);
    motorLoopsNb = analogRead(feedQuantity) / 50;
    Serial.println("Motor loops number when feeding: ");
    Serial.println(motorLoopsNb);
}

void Feed::TurnLedRed() {
    digitalWrite(pinGreenLed, LOW);
    digitalWrite(pinRedLed, HIGH);
}

void Feed::TurnLedGreen() {
    digitalWrite(pinGreenLed, HIGH);
    digitalWrite(pinRedLed, LOW);
}

double Feed::ComputeElapsedTimeSincePowerOn() {
    elapsedHours = elapsedHours + (1.0 / 3600.0);
}

void Feed::TurnLedToGreenIfFeeded() {
    if (feeded) {
        TurnLedGreen();
    } else {
        TurnLedRed();
    }
}

void Feed::ManualFeed() {
    if (digitalRead(pinManualFeedCmd) == false) {
        small_stepper->step(stepsPerOutputRevolution);
        Serial.println("User choose to manually feed");
    }
}

void Feed::AutomaticFeed() {
    if (feeded == true)
        return;

    // Feed (Arduino is powered at 8 am)
    if ((elapsedHours + poweringHour) >= feedHour) {
        small_stepper->step(motorLoopsNb * stepsPerOutputRevolution);
        feeded = true;
    }
}
