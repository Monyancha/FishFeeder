#include <Stepper.h>
#include "Feed.h"

double feedHour = 17.0;
double poweringHour = 8.5; // Programmable plug powering hour (for relative time computation)

Feed feed(poweringHour, poweringHour);

void setup()
{
  Serial.begin(115200);
  feed.Init();
  delay(1000);
  Serial.println("Setup finished");
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  feed.ComputeElapsedTimeSincePowerOn();
}

void loop()
{
  feed.ManualFeed();
  feed.AutomaticFeed();
  feed.TurnLedToGreenIfFeeded();
}
