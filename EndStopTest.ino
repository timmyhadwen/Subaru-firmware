#include <Servo.h>
#include "Motor.h"
#include "GPS.h"

Motor gear_lever;
Motor brake_lever;
SubyGPS gps;

enum State {
  HALT,
  STOP,
  IGNITION,
  START,
  RUNNING,
  DRIVING
};

State currentState  = HALT;

#define HALT_STATE 1

int state = 0;

void setup() {
  Serial.begin(9600);
  Servo gear;
  gear.attach(12);
  gear_lever = Motor(&gear, A0);

  Servo brake;
  brake.attach(11);
  brake_lever = Motor(&brake, A1);
  Serial.println("Starting...");
}

void loop() {
  Serial.println("Loop");
  gear_lever.update();
  brake_lever.update();
  gps.update();

  // if deadmanswitch = 0
  // currentState = stop;

  switch (currentState) {
    case STOP:
      // If brake not fully depressed
        // Depress brake
      // If ignition_on = 1
        // ignition_on = 0
      // else 
        // Move to HALT
      break;
    case HALT:
      // If brake not fully depressed
        // Depress brake
      // else If ignition_on = 1
        // ignition_on = 0
      // else If accelrator not 0
        // accelerator = 0
      // else If ignition_start = 1
        // ignition_start = 0
      // else If gear != P
        // Move to gear P
      // else If deadmanswitch = 1 && vel > 0
        // Move to ignition
      break;
    case IGNITION:
      // 
      break;
    case START:
      break;
    case RUNNING:
      break;
    case DRIVING:
      break;
    default:
      currentState = HALT;
      break;
  }

  delay(100);
}
