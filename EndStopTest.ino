#include <Servo.h>
#include <Adafruit_GPS.h>
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
Adafruit_GPS gps_module(&Serial1);
JetsonCom jetsoncom;

/* State machine */
StateMachine statemachine;

void setup() {
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  Serial.print("Subaru init started....");

  gps_module.begin(9600);
  
  Servo gear;
  gear.attach(12);
  gear_lever = Motor(&gear, A0);
  gear_lever.min = 0;
  gear_lever.max = 1023;

  Servo brake;
  brake.attach(11);
  brake_lever = Motor(&brake, A1);
  brake_lever.min = 0;
  brake_lever.max = 1023;

  brake_lever.setTarget(BRAKE_ON_POS);

  Servo steering;
  steering.attach(10);
  steering_motor = Motor(&brake, A2);
  steering_motor.min = 0;
  steering_motor.max = 1023;

  Servo __accl;
  __accl.attach(9); 
  accelerator = Accelerator(&__accl);

  struct Actuators actuators = {accelerator, gear_lever, brake_lever};
  statemachine = StateMachine(&actuators);
  
  Serial.println("Done");
}

void loop() {
//  Serial.print("State: ");
//  Serial.println(statemachine.getState());
  gear_lever.update();
  brake_lever.update();
  steering_motor.update();
  
  if (jetsoncom.update()) {
    // We have new coms
    if (jetsoncom.command.dead == 1 || IGNORE_DEAD_MAN) {
      // Dead man switch has been started, move to running
      // TODO: Start car, move to running state
      statemachine.requestState(RUNNING);
    } else {
      statemachine.requestState(STOP);
    }

    // If speed is on and we are in running state, move to driving
    if (statemachine.getState() == RUNNING && jetsoncom.command.speed > 0) {
      // Move to driving
      statemachine.requestState(DRIVING);
    }

    // If we're in driving then write accelerator values
    if (statemachine.getState() == DRIVING) {
      // Write accelerator values
      accelerator.setTarget(map(jetsoncom.command.speed, 0, 100, 0, ACCELERATOR_MAX));
    }

    if (statemachine.getState() == DRIVING || statemachine.getState() == RUNNING) {
      // Write steering angles
      steering_motor.setTarget(map(jetsoncom.command.angle, -180, 180, 0, 1023));
    }
  }

  char c = gps_module.read();
  if (gps_module.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    
    gps_module.parse(gps_module.lastNMEA());
    #ifdef GPS_PRINTOUT
    Serial.println(gps_module.latitude); // this also sets the newNMEAreceived() flag to false
    Serial.println(gps_module.longitude); // this also sets the newNMEAreceived() flag to false
    Serial.println(gps_module.satellites);
    #endif
  }
  delay(100);
}
