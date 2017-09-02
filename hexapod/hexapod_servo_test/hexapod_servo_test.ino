#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  150
#define SERVOMAX  530
#define SERVOFREQ 60

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
double pulselength;

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");

  pulselength = (1000000/SERVOFREQ)/4096;

  pwm.begin();

  pwm.setPWMFreq(SERVOFREQ);  // Analog servos run at ~60 Hz updates
  resetServos();
}

// you can use this function if you'd like to set the pulse length in useconds
// e.g. servoWrite(0, 1500) is a 1500usecond pulsewidth
void servoWrite(uint8_t n, double pulse) {
  double pulsetick;
  pulsetick = pulse / pulselength;
  Serial.println(pulsetick);
  pwm.setPWM(n, 0, pulsetick);
}

void testServo(uint8_t n) {
  Serial.println(n);
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(n, 0, pulselen);
  }

  delay(500);
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(n, 0, pulselen);
  }
  delay(500);
  resetServo(n); 
}

void resetServo(uint8_t n) {
    servoWrite(n, 1500);
}

void resetServos() {
  for (int i = 0; i < 12; i++)
  {
    resetServo(i);
  }
  delay(1000);
}

void loop() {
    for (int i = 0; i < 12; i++) {
      testServo(i);
      delay(500);
    }
}

