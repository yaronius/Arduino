/* -----Flamewheel Bluetooth Control Program
//------2016.6.29 by LL
//------Suitable for Romeo BLE Mini MCU
//http://www.dfrobot.com/index.php?route=product/product&product_id=1367&search=ble+mini&description=true#.V8AR1q11Zfc
*/
#include "GoBLE.h"
#include <Romeo_m.h>

#define LED 13

//GoBLE Goble(Serial);
int joystickX, joystickY;
int buttonState[7];
unsigned int led_count;
void setup() {
  Serial.begin(9600);
  Romeo_m.Initialise();
  Goble.begin();
  pinMode(LED, OUTPUT);
}
void loop() {
  if (Goble.available())
  {
    readGoBle();
    motorContrl();
  }
  delayLedBlink();//delay 10ms and led blink
}
//Read GoBLE values
void readGoBle()
{
  // read joystick value when there's valid command from bluetooth
  joystickX = Goble.readJoystickX();
  joystickY = Goble.readJoystickY();
  // read button state when there's valid command from bluetooth
  buttonState[SWITCH_UP]     = Goble.readSwitchUp();
  buttonState[SWITCH_DOWN]   = Goble.readSwitchDown();
  buttonState[SWITCH_LEFT]   = Goble.readSwitchLeft();
  buttonState[SWITCH_RIGHT]  = Goble.readSwitchRight();
  buttonState[SWITCH_SELECT] = Goble.readSwitchSelect();
  buttonState[SWITCH_START]  = Goble.readSwitchStart();
  Serial.println("========================");
   Serial.print("Joystick Value: ");
  Serial.print(joystickX);
  Serial.print("  ");
  Serial.println(joystickY);
  for (int i = 1; i <= 6; i++) {
    if (buttonState[i] == PRESSED) {
      Serial.print(" ID: ");
      Serial.print(i);
      Serial.print("\t ");
      Serial.println("Pressed!");
    }
    if (buttonState[i] == RELEASED){
      Serial.print("ID: ");
      Serial.print(i);
      Serial.print("\t ");
      Serial.println("Released!");
    }
  }
}
//Move according to GoBLE value
//Joystick left and right to turn bends, left and right buttons to spin on the spot
void motorContrl()
{
  if ((buttonState[SWITCH_UP] == PRESSED) || ((joystickX > 128) && (joystickY >= 64) && (joystickY <= 192)))
  {
    Romeo_m.motorControl(Forward, 200, Forward, 200); //go forward
    return;//end function
  }
  if ((buttonState[SWITCH_DOWN] == PRESSED) || ((joystickX < 128) && (joystickY >= 64) && (joystickY <= 192)))
  {
    Romeo_m.motorControl(Reverse, 150, Reverse, 150); //go backwards
    return;//end function
  }
  if (buttonState[SWITCH_LEFT] == PRESSED)
  {
    Romeo_m.motorControl(Reverse, 100, Forward, 100); //turn left
    return;//end function
  }
  if ((joystickY < 128 ) && (joystickX >= 64 ) && ( joystickX <= 192) )
  {
    Romeo_m.motorControl_M1(Forward, 80); //turn left big bend
    Romeo_m.motorControl_M2(Forward, 200);
    return;//end function
  }
  if ( buttonState[SWITCH_RIGHT] == PRESSED)
  {
    Romeo_m.motorControl(Forward, 100, Reverse, 100); //turn right
    return;//end function
  }
  if ((joystickY > 128) && (joystickX >= 64) && (joystickX <= 192))
  {
    Romeo_m.motorControl_M2(Forward, 80); //turn right big bend
    Romeo_m.motorControl_M1(Forward, 200);
    return;//big bend
  }
  Romeo_m.motorStop();//no stop button is pressed
}
//led blink funtion, each execution delay 10ms. every 100 times level inverted
void delayLedBlink()
{
  delay(10);
  led_count++;
  if (led_count > 100)
  {
    digitalWrite(LED, !digitalRead(LED));
    led_count = 0;
    Serial.println("Blink!");
  }
}
