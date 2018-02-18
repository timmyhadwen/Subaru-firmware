#include <Servo.h>
//#include <Adafruit_GPS.h>
#include "Motor.h"
#include "JetsonCom.h"
#include "Accelerator.h"
#include "StateMachine.h"

/* Variables for each motor */
Motor gear_lever;
Motor brake_lever;
Motor steering_motor;
Accelerator accelerator;

/* Coms modules */
//Adafruit_GPS gps_module(&Serial1);
JetsonCom jetsoncom;

/* State machine */
StateMachine statemachine;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  Serial.print("Subaru init started....");

//  gps_module.begin(9600);
  
  Servo gear;
  gear.attach(12);
  gear_lever = Motor(&gear, A0);
  gear_lever.min = 0;
  gear_lever.max = 1023;
  gear_lever.PTERM = 1.0;

  Servo brake;
  brake.attach(11);
  brake_lever = Motor(&brake, A1);
  brake_lever.min = 0;
  brake_lever.max = 1023;

  Servo steering;
  steering.attach(10);
  steering_motor = Motor(&steering, A2);
  steering_motor.min = 0;
  steering_motor.max = 1023;
  steering_motor.invert = true;
  steering_motor.PTERM = 0.4;

  Servo __accl;
  __accl.attach(9); 
  accelerator = Accelerator(&__accl);
  __accl.write(ACCELERATOR_ZERO);

  struct Actuators actuators = {accelerator, gear_lever, brake_lever};
  statemachine = StateMachine(&actuators);
  
  Serial.println("Done");

  brake_lever.setTarget(BRAKE_ON_POS);
  gear_lever.setTarget(GEAR_P_POS);

  // 500 midpoint
  // 250 left
  // 750 right
  
  steering_motor.setTarget(500);

  gear_lever.setTarget(GEAR_P_POS);

//  while(true) {
//    gear_lever.update();
//    delay(1);
//  }

//  while(true) {
//    accelerator.setTarget(ACCELERATOR_ZERO);
//    delay(1000);
//    accelerator.setTarget(ACCELERATOR_MAX);
//    delay(1000);
//  }
//
//  int i = 0;
//  while (true) { 
//    i++;
//    Serial.println(steering_motor.getPosition());
//    steering_motor.update();
//
////    if (i > 200) { 
////      gear_lever.setTarget(GEAR_P_POS);
////    }
////
////    if (i > 400) { 
////      gear_lever.setTarget(GEAR_R_POS);
////    }
////
////    if (i > 600) { 
////      gear_lever.setTarget(GEAR_N_POS);
////    }
////
////    if (i > 800) { 
////      gear_lever.setTarget(GEAR_D_POS);
////    }
//    delay(10);
//  }
}

void loop() {  
  gear_lever.update();
  brake_lever.update();
  steering_motor.update();

  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  
  if (jetsoncom.update()) {
    // We have new coms
    if (jetsoncom.command.dead == 1 || IGNORE_DEAD_MAN) {
      // Dead man switch has been started, move to running
      // TODO: Start car, move to running state
      statemachine.requestState(RUNNING);
    } else if (statemachine.getState() == STOP) {
      statemachine.requestState(HALT);
    } else if (statemachine.getState() != HALT) {
      statemachine.requestState(STOP);
    }

    // If speed is on and we are in running state, move to driving
    if (statemachine.getState() == RUNNING && jetsoncom.command.speed > 0) {
      // Move to driving
      statemachine.requestState(DRIVING);
    }

    // If we're in driving then write accelerator values
    if (statemachine.getState() == DRIVING || statemachine.getState() == HALT) {
      // Write accelerator values
      accelerator.setTarget(mapfloat((float)jetsoncom.command.speed, 0, 100, ACCELERATOR_ZERO, ACCELERATOR_MAX));
    }

    if (statemachine.getState() == DRIVING || statemachine.getState() == RUNNING || statemachine.getState() == HALT ) {
      // Write steering angles
//      steering_motor.setTarget(mapfloat((float)jetsoncom.command.angle, -90, 90, 250, 750));
    }
  }
  
  delay(100);
}
