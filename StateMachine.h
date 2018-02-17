//
// Created by rue011 on 17/02/18.
//
#ifndef SUBARU_CAR_STATE_MACHINE_HPP
#define SUBARU_CAR_STATE_MACHINE_HPP

#define ENGINE_ON_PIN 2
#define ENGINE_CRANK_PIN 3

#include <Servo.h>
#include "Defines.h"

struct Actuators {
  Accelerator servo_acc; // Accelerator
  Motor gear_lever;
  Motor brake_lever;
};

enum State {
  HALT,
  STOP,
  IGNITION,
  START,
  RUNNING,
  DRIVING
};

enum Gear{
  PARK,
  REVERSE,
  DRIVE
};



class StateMachine{
  public:
    StateMachine(Actuators *actuators);
    StateMachine();
    void initialize();
    void update();

    bool requestState(State requested_state);

    void achieveCurrentState();
    void changeState();
    void emergencyStop();

  State getState(){return state_;}


  private:
    State state_; // enumarotor of the state
    Gear  gear_;
    double brake_;
    bool engineCrankOn_, ignitionOn_, motorOn_; // igni_start: 0,1 to start the motor,
    double acc_;
    Actuators *actuators_;

    // We dont want to acces the states of the StateMachine
    //  void setState(State state){state_=state};
    // Checks if all defined coditoin in states
    bool checkState();
    bool checkStop();
    bool checkHalt();
    bool checkIgnition();
    bool checkStart();
    bool checkRunning();
    bool checkDrivinng();


    void setIgnitionON();
    void setIgnitionOFF();
    void setIgnitionCrankON();
    void setIgnitionCrankOFF();
};

StateMachine::StateMachine() {}

StateMachine::StateMachine(Actuators *actuators) {
  actuators_ = actuators;
  initialize();
}

void StateMachine::setIgnitionON(){
  ignitionOn_ = 1;
  digitalWrite(ENGINE_ON_PIN, 1);
};

void StateMachine::setIgnitionOFF(){
  ignitionOn_ =0;
  digitalWrite(ENGINE_ON_PIN, 0);
};

void StateMachine::setIgnitionCrankON(){
  engineCrankOn_ = 1;
  digitalWrite(ENGINE_CRANK_PIN, 1);
};
void StateMachine::setIgnitionCrankOFF(){
  engineCrankOn_ =0;
  digitalWrite(ENGINE_CRANK_PIN, 0);
};


// Intialisation of the StateMachine
void StateMachine::initialize(){
  state_ = HALT;
  setIgnitionOFF();
  setIgnitionCrankOFF();
  //ToDO:: Check MOTORS  for current state
  update();

}

// Updates the state of the motors
void StateMachine::update(){
//ToDo: Get the actuator values
    digitalWrite(ENGINE_ON_PIN, 0);
//  acc_ = actuator
 gear_ = actuators_->gear_lever.getGear();
 brake_ = actuators_->brake_lever.getPosition();
}

// State request from the main file,
bool StateMachine::requestState(State requested_state){
  update();
  if (STOP == requested_state){
    emergencyStop();
    return true;
  }
  else if(requested_state == state_ && checkState())
  { // Are in the requested state and all the state condiftions have been met
    return true;
  }
  else {
   // This is where Switch to the next state, and start acchiving the state;
    if( checkState() ){
      changeState();
      return false;
    }
    else if(!checkState()){
      achieveCurrentState();
      return false;
    }
    else{}
//  assert(false);
  }
//  assert(false);
};

void StateMachine::emergencyStop(){
  state_ = STOP;
  actuators_->brake_lever.setTarget(1023);
  setIgnitionOFF();
}


void StateMachine::achieveCurrentState(){
  double t = 0;
  double delta_t = 0.2;
  switch(state_){
    default:
//      assert(false);
      break;
    case STOP:
      // Only reset
    //ToDo:: Read motor values
      actuators_->brake_lever.setTarget(1023);
      digitalWrite(ENGINE_ON_PIN, 0);
      break;

    case HALT:
      actuators_->servo_acc.setTarget(ACCELERATOR_ZERO);
      actuators_->gear_lever.setTarget(GEAR_P_POS);
      setIgnitionOFF();
      setIgnitionCrankOFF();
      break;

    case IGNITION:
      setIgnitionON();
      break;

    case START:
      while(t < 2.0) {
        setIgnitionCrankON();
        t += delta_t;
      }
      setIgnitionCrankOFF();
      break;

    case RUNNING:
      actuators_->gear_lever.setTarget(GEAR_D_POS);
      actuators_->brake_lever.setTarget(BRAKE_OFF_POS);
      break;

    case DRIVING:
      break;
  }
}

void StateMachine::changeState(){
  switch(state_){

    default: //This should never happen
      emergencyStop();
      break;

    case STOP:
      state_ = HALT;
      achieveCurrentState();
      break;

    case HALT:
      state_ = IGNITION;
      achieveCurrentState();
      break;

    case IGNITION:
      state_ = START;
      achieveCurrentState();
      break;

    case START:
      state_ = RUNNING;
      achieveCurrentState();
      break;

    case RUNNING:
      state_ = DRIVING;
      achieveCurrentState();
      break;
  }
}


}

bool StateMachine::checkState(){
  switch(state_){
    default: //This should never happen
      emergencyStop();
      return False;
    case STOP:
      return checkStop();
    case HALT:
      return checkHalt();
    case IGNITION:
      return checkIgnition();
    case START:
      return checkStart();
    case RUNNING:
      return checkRunning();
    case DRIVING:
      return checkDrivinng();
  }
}


bool StateMachine::checkStop(){
  if (100 = break_ && false == ignition_){
    return true;
  }else{
    return false;
  }
}

bool StateMachine::checkHalt(){
  if (ignition_ == false && 0 == acc_ && gear_ == Park &&
      100 = break_ && false == ignitionStart_){
    return true;
  }else{
    return false;
  }

};

bool StateMachine::checkIgnition(){
  if (ignition_ == true && 0 == acc_ && gear_ == Park &&
      100 = break_ && false == ignitionStart_){
    return true;
  }else{
    return false;
  }

};
bool StateMachine::checkStart(){
  if (ignition_ == true && 0 == acc_ && gear_ == Park &&
      100 = break_ && false == ignitionStart_){
    return true;
  }else{
    return false;
  }
};
bool StateMachine::checkRunning() {
  if (ignition_ == true && 0 == acc_ && gear_ == Park &&
      0 = break_ && false == ignitionStart_){
    return true;
  }else{
    return false;
  }
};


bool StateMachine::checkDrivinng(){
  if (ignition_ == true && 0 == acc_ && gear_ == Park &&
      100 = break_ && false == ignitionStart_){
    return true;
  }else{
    return false;
  }
};

#endif //SUBARU_CAR_STATE_MACHINE_HPP

