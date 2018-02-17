#define PTERM 2
#define ITERM 0

class Motor {
  public:
    Motor(Servo* servo, int feedback_pin);
    Motor();
    void update();
    void setTarget(int x);
  private:
    int motor_pin;
    int feedback_pin;
    int current_speed = 90;
    int target = 450;
    double error = 0;
    Servo* servo;
};

Motor::Motor() {
}

Motor::Motor(Servo* servo, int feedback_pin) {
  this->motor_pin = motor_pin;
  this->feedback_pin = feedback_pin;
  this->servo = servo;
  this->servo->write(90);
  pinMode(this->feedback_pin, INPUT);
}

void Motor::update() {
  int sensorPos = analogRead(this->feedback_pin);
  Serial.print(sensorPos);

  int diff = this->target - sensorPos;
  int term = 90 + diff*PTERM;
  Serial.print(" ");
  Serial.print(term);
  Serial.print("\n");

  // Limit to 180 and 0
  if (term > 180)
    term = 180;
  if (term < 0)
    term = 0;
  this->servo->write(term);
}

void Motor::setTarget(int x) {
  this->target = x;
}
