#include "JetsonCommand.h"

class JetsonCom {
public:
	bool update();
	char buffer[50];
	int buffer_count = 0;
  JetsonCommand command;
};

/* Yes this is a hack of a CSV parser but shush */
bool JetsonCom::update() {
	if (Serial.available()) {
		buffer[buffer_count++] = Serial.read();

		if (buffer[buffer_count - 1] == '\n') {
			// End of line parse please
			char buf[10];
			int buf_cnt;
      int howmanycsv = 0;
			for (int i = 0; i < buffer_count; i++) {
				if (buffer[i] != ',' && i != buffer_count - 1) {
					buf[buf_cnt++] = buffer[i];
				} else {
					buf_cnt = 0;
					double val = atof(buf);
          if (howmanycsv == 0) {
            command.dead = val;
          } else if (howmanycsv == 1) {
            command.speed = val;
          } else if (howmanycsv == 2) {
            command.angle = val;
          } else if (howmanycsv == 3) {
            command.sensor_angle = val;
          }

          howmanycsv++;
					Serial.println(val);
				}
			}
      buffer_count = 0;
      return true;
		} else {
			return false;
		}
	}
}
