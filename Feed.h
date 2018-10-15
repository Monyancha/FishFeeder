#ifndef FEED_H
#define FEED_H

class Feed {
  public:
  private:
    /********** Settings ***************/
    double feedHour = 0.0;
    double poweringHour = 0.0; // Programmable plug powering hour (for relative time computation)
    /**********************************/
    static constexpr unsigned int stepsPerMotorRevolution = 32;
    static constexpr unsigned int stepsPerOutputRevolution = 32 * 64;
    static constexpr unsigned int pinManualFeedCmd = 12;
    static constexpr unsigned int pinGreenLed = 2;
    static constexpr unsigned int pinRedLed = 3;
    static constexpr unsigned int feedQuantity = 19;
    float elapsedHours = 0.0;
    bool feeded = false;
    int motorLoopsNb = 0; // Motor loops number setted by potentiometer (0 to 10 loops)
    Stepper *small_stepper;

  public:
    Feed(double poweringHour, double feedHour);
    void Init();
    void TurnLedToGreenIfFeeded();
    void ManualFeed();
    void AutomaticFeed();
    double ComputeElapsedTimeSincePowerOn();

  private:
    void InitPins();
    void InitTimer();
    void InitStepMotor();
    void TurnLedRed();
    void TurnLedGreen();
};
#endif
