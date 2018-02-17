/* Ignores the dead man switch variable from the jetson */
#define IGNORE_DEAD_MAN 0

//#define GPS_PRINTOUT 0

/* Defines for the gear positions */
#define GEAR_P_POS 900
#define GEAR_R_POS 800
#define GEAR_N_POS 700
#define GEAR_D_POS 600
#define GEAR_1_POS 500
#define GEAR_2_POS 400

/* Defines for the brake positions */
#define BRAKE_ON_POS 100
#define BRAKE_OFF_POS 0

#define ACCELERATOR_ZERO 50
#define ACCELERATOR_MAX 57

#define GEAR_OFFSET 10

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
  DRIVE,
  NEUTRAL,
  UKNOWN
};

