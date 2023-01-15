#ifndef POOL_MOTOR_CONTROLLER_H
#define POOL_MOTOR_CONTROLLER_H

#define MODE_MANUAL 0x01
#define MODE_TIMER  0x02
#define MODE_SCHEDULE 0x03
#define MODE_NOT_ALLOWED 0x04

class PoolMotorController {
  public:

    PoolMotorController(int pin);
    void turnOn();
    void turnOff();
    void manualTurnOn();
    void manualTurnOff();
    void setTimer(unsigned int onTime, unsigned int offTime);
    void setSchedule(String day, time_t hourStart, time_t hourEnd);
    bool getMotorStatus();
    void checkTimer();
    void checkSchedule(time_t currentTime);
    word getModeActive();

  private:
    WiFiUDP _ntpUDP;
    int _pin;
    bool _motorStatus;
    unsigned int _timerOnTime;
    unsigned int _timerOffTime;
    bool _timerModeActive;
    String _scheduledDay;
    time_t _scheduledHourStart;
    time_t _scheduledHourEnd;
    bool _scheduleModeActive;
};

void PoolMotorController::checkSchedule(time_t currentTime) {

  if (currentTime >= _scheduledHourStart && currentTime <= _scheduledHourEnd) {
      turnOn();
  } else {
      turnOff();
  }
}
void PoolMotorController::checkTimer() {
  if (!_timerModeActive) {
    return;
  }
      
  unsigned int currentTime = millis();
  if (currentTime >= _timerOnTime && currentTime <= _timerOffTime) {
    turnOn();
  } else {
    turnOff();
  }
  
}
bool PoolMotorController::getMotorStatus() {
      return _motorStatus;
}
void PoolMotorController::setSchedule(String day, time_t hourStart, time_t hourEnd) {
      _scheduledDay = day;
      _scheduledHourStart = hourStart;
      _scheduledHourEnd = hourEnd;

      _scheduleModeActive = true;
      _timerModeActive = false;
    }
void PoolMotorController::turnOn() {
      digitalWrite(_pin, HIGH);
      _motorStatus = true;
    }
void PoolMotorController::turnOff() {
      digitalWrite(_pin, LOW);
      _motorStatus = false;
    }
void PoolMotorController::setTimer(unsigned int onTime, unsigned int offTime) {
      _timerOnTime = onTime;
      _timerOffTime = offTime;
      _timerModeActive = true;
      _scheduleModeActive = false;
    }
void PoolMotorController::manualTurnOn(){

  turnOn();

  _scheduleModeActive = false;
  _timerModeActive = false;
}
void PoolMotorController::manualTurnOff(){

  turnOff();
  
  _scheduleModeActive = false;
  _timerModeActive = false;
}

word PoolMotorController::getModeActive(){

  if(_scheduleModeActive & _timerModeActive){
    return(MODE_NOT_ALLOWED);
  }
  if(!_scheduleModeActive & _timerModeActive){
    return(MODE_MANUAL);
  }
  if(_scheduleModeActive){
    return(MODE_SCHEDULE);
  }
  if(_timerModeActive){
    return(MODE_TIMER);
  }
return(MODE_NOT_ALLOWED);
}

PoolMotorController::PoolMotorController(int pin) {
_pin = pin;
pinMode(_pin, OUTPUT);
}

#endif
