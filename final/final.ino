/**
 * Spektrum satellite USB
 * version : 0.0.1
 */

// ---------- Settings ---------- //
const bool forcing_update = true;
const bool bind = false;
const byte bind_pulses = 9;

// ---------- Libraries ---------- //
#include <Spektrum_Satellite.h>
Spektrum_Satellite rx;
#include <Joystick.h>
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK, 2, 0, true, true, true, true, true, true, false, false, false, false, false);

// ---------- Functions ---------- //
void setup() {
  rxInit();
  joystickInit();
}

void loop() {
  if (Serial1.available()) {
    uint8_t c = Serial1.read();
    if (rx.update(c) || forcing_update) {
      joystickUpdate();
    }
  }
}

// Receiver
void rxInit() {
  if (bind) {
    pinMode(0, OUTPUT);
    for (int i=0; i<bind_pulses; i++) {
      digitalWrite(0, LOW);
      delayMicroseconds(100);
      digitalWrite(0, HIGH);
      delayMicroseconds(100);
    }
    pinMode(0, INPUT);
  }
  Serial1.begin(115200);
}

// Joystick
void joystickInit() {
  Joystick.setXAxisRange(0, 2048);
  Joystick.setYAxisRange(0, 2048);
  Joystick.setZAxisRange(0, 2048);
  Joystick.setRxAxisRange(0, 2048);
  Joystick.setRyAxisRange(0, 2048);
  Joystick.setRzAxisRange(0, 2048);
  Joystick.begin(false);
}
void joystickUpdate() {
  Joystick.setYAxis(rx.getChannel(0));
  Joystick.setZAxis(rx.getChannel(1));
  Joystick.setRzAxis(rx.getChannel(2));
  Joystick.setXAxis(rx.getChannel(3));
  Joystick.setRxAxis(rx.getChannel(4));
  if (rx.getChannel(4) < 1024) {
    Joystick.setButton(0, 0);
  } else {
    Joystick.setButton(0, 1);
  }
  Joystick.setRyAxis(rx.getChannel(5));
  if (rx.getChannel(5) < 1024) {
    Joystick.setButton(1, 0);
  } else {
    Joystick.setButton(1, 1);
  }
  Joystick.sendState();
}
