#include <Stepper.h>
#define STEPS_PER_MOTOR_REVOLUTION 32
#define STEPS_PER_OUTPUT_REVOLUTION 32*64

/********** Settings ***************/
#define SET_FEED_HOUR 17.0
#define SET_POWERING_HOUR 8.5 // Programmable plug powering hour (for relative time computation)
int motorLoopsNb = 0; // Motor loops number setted by potentiometer (0 to 10 loops)
/**********************************/

bool feeded = false;

#define CMD 12
#define LED_GREEN 2
#define LED_RED 3
#define QUANTITY 19

float elapsedHours = 0.0;

Stepper small_stepper(STEPS_PER_MOTOR_REVOLUTION, 8, 10, 9, 11);

void setup()
{
  Serial.begin(115200);

  pinMode(CMD, INPUT_PULLUP);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(QUANTITY, INPUT);

  // initialize timer1
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 62500;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
  small_stepper.setSpeed(200);

  motorLoopsNb = analogRead(QUANTITY) / 50;
  Serial.println("Motor loops number when feeding: ");
  Serial.println(motorLoopsNb);

  feeded = false;

  delay(1000);

  Serial.println("Setup finished");
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  elapsedHours = elapsedHours + (1.0 / 3600.0);
}

void loop()
{
  if (digitalRead(CMD) == false) {
    small_stepper.step(STEPS_PER_OUTPUT_REVOLUTION);
    Serial.println("User choose to manually feed");
  }

  // Feed (Arduino is powered at 8 am)
  if (feeded == false) {
    if ( (elapsedHours + SET_POWERING_HOUR) >= SET_FEED_HOUR) {
      small_stepper.step(motorLoopsNb * STEPS_PER_OUTPUT_REVOLUTION);
      feeded = true;
    }
  }

  if (feeded) {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
  } else {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
  }
}
