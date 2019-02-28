/**
   Spektrum receiver adaptator to USB
   Version: test
*/

// ---------- Settings ---------- //
const byte PIN_RECEIVER_1 = A0;
const byte PIN_RECEIVER_2 = A1;
const byte PIN_RECEIVER_3 = A2;
const byte PIN_RECEIVER_4 = A3;
const bool EEPROM_CLEAR = false;

// ---------- Libraries ---------- //
#include <EEPROM.h>
#include <Joystick.h>

// ---------- Global variables ---------- //
float temp;
byte receiver_thro_input, receiver_aile_input, receiver_elev_input, receiver_rudd_input, receiver_thro_trim, receiver_aile_trim, receiver_elev_trim, receiver_rudd_trim, receiver_thro_deadband, receiver_aile_deadband, receiver_elev_deadband, receiver_rudd_deadband;
int receiver_thro, receiver_aile, receiver_elev, receiver_rudd, receiver_thro_prev = 1500, receiver_aile_prev = 1500, receiver_elev_prev = 1500, receiver_rudd_prev = 1500;
float receiver_thro_course, receiver_aile_course, receiver_elev_course, receiver_rudd_course;
bool receiver_thro_inversion, receiver_aile_inversion, receiver_elev_inversion, receiver_rudd_inversion;
byte joystick_throttle, joystick_rudder;
int joystick_xaxis_rotation, joystick_yaxis_rotation;
bool joystick = true;
byte serial_thro1, serial_aile1, serial_elev1, serial_rudd1, serial_thro2, serial_aile2, serial_elev2, serial_rudd2;
int serial_mode = 0;

// ---------- Functions ---------- //
// EEPROM
void eepromInit() {
  if (EEPROM_CLEAR) {
    for (int i = 0; i < EEPROM.length(); i++) {
      EEPROM.write(i, 0);
    }
  }
  if (EEPROM.read(0) != true) {
    EEPROM.put(0, (bool) true);
    EEPROM.put(1, (byte) 1);
    EEPROM.put(2, (byte) 2);
    EEPROM.put(3, (byte) 3);
    EEPROM.put(4, (byte) 4);
    EEPROM.put(5, (byte) 0);
    EEPROM.put(6, (byte) 0);
    EEPROM.put(7, (byte) 0);
    EEPROM.put(8, (byte) 0);
    EEPROM.put(9, (byte) 0);
    EEPROM.put(10, (byte) 0);
    EEPROM.put(11, (byte) 0);
    EEPROM.put(12, (byte) 0);
    EEPROM.put(13, (bool) false);
    EEPROM.put(14, (bool) false);
    EEPROM.put(15, (bool) false);
    EEPROM.put(16, (bool) false);
    EEPROM.put(17, (float) 1.00);
    EEPROM.put(21, (float) 1.00);
    EEPROM.put(25, (float) 1.00);
    EEPROM.put(29, (float) 1.00);
  }
  eepromRead();
}
void eepromRead() {
  receiver_thro_input = EEPROM.read(1);
  receiver_aile_input = EEPROM.read(2);
  receiver_elev_input = EEPROM.read(3);
  receiver_rudd_input = EEPROM.read(4);
  receiver_thro_deadband = EEPROM.read(5);
  receiver_aile_deadband = EEPROM.read(6);
  receiver_elev_deadband = EEPROM.read(7);
  receiver_rudd_deadband = EEPROM.read(8);
  receiver_thro_trim = EEPROM.read(9);
  receiver_aile_trim = EEPROM.read(10);
  receiver_elev_trim = EEPROM.read(11);
  receiver_rudd_trim = EEPROM.read(12);
  receiver_thro_inversion = EEPROM.read(13);
  receiver_aile_inversion = EEPROM.read(14);
  receiver_elev_inversion = EEPROM.read(15);
  receiver_rudd_inversion = EEPROM.read(16);
  float f = 0.000f;
  receiver_thro_course = EEPROM.get(17, f);
  receiver_aile_course = EEPROM.get(21, f);
  receiver_elev_course = EEPROM.get(25, f);
  receiver_rudd_course = EEPROM.get(29, f);
}

// Receiver
void receiverUpdate() {
  switch (receiver_thro_input) {
    case 1:
      receiver_thro = pulseIn(PIN_RECEIVER_1, HIGH, 50000);
      break;
    case 2:
      receiver_thro = pulseIn(PIN_RECEIVER_2, HIGH, 50000);
      break;
    case 3:
      receiver_thro = pulseIn(PIN_RECEIVER_3, HIGH, 50000);
      break;
    case 4:
      receiver_thro = pulseIn(PIN_RECEIVER_4, HIGH, 50000);
      break;
  }
  switch (receiver_aile_input) {
    case 1:
      receiver_aile = pulseIn(PIN_RECEIVER_1, HIGH, 50000);
      break;
    case 2:
      receiver_aile = pulseIn(PIN_RECEIVER_2, HIGH, 50000);
      break;
    case 3:
      receiver_aile = pulseIn(PIN_RECEIVER_3, HIGH, 50000);
      break;
    case 4:
      receiver_aile = pulseIn(PIN_RECEIVER_4, HIGH, 50000);
      break;
  }
  switch (receiver_elev_input) {
    case 1:
      receiver_elev = pulseIn(PIN_RECEIVER_1, HIGH, 50000);
      break;
    case 2:
      receiver_elev = pulseIn(PIN_RECEIVER_2, HIGH, 50000);
      break;
    case 3:
      receiver_elev = pulseIn(PIN_RECEIVER_3, HIGH, 50000);
      break;
    case 4:
      receiver_elev = pulseIn(PIN_RECEIVER_4, HIGH, 50000);
      break;
  }
  switch (receiver_rudd_input) {
    case 1:
      receiver_rudd = pulseIn(PIN_RECEIVER_1, HIGH, 50000);
      break;
    case 2:
      receiver_rudd = pulseIn(PIN_RECEIVER_2, HIGH, 50000);
      break;
    case 3:
      receiver_rudd = pulseIn(PIN_RECEIVER_3, HIGH, 50000);
      break;
    case 4:
      receiver_rudd = pulseIn(PIN_RECEIVER_4, HIGH, 50000);
      break;
  }
}
void receiverTuning() {
  if (receiver_thro < 1500) {
    if (receiver_thro_course >= 1) temp = (receiver_thro - 1500) * -1 * (1-(receiver_thro_course-1));
    else temp = (receiver_thro - 1500) * -1 * (1-receiver_thro_course+1);
    temp = (temp - 500) * -1 + 1000;
    receiver_thro = (int) temp;
  } else if (receiver_thro > 1500) {
     receiver_thro_course;
    if (receiver_thro_course >= 1) temp = (receiver_thro - 1500) * (1-(receiver_thro_course-1));
    else temp = (receiver_thro - 1500) * (1-receiver_thro_course+1);
    temp = temp + 1500;
    receiver_thro = (int) temp;
  }
  if (receiver_aile < 1500) {
    if (receiver_aile_course >= 1) temp = (receiver_aile - 1500) * -1 * (1-(receiver_aile_course-1));
    else temp = (receiver_aile - 1500) * -1 * (1-receiver_aile_course+1);
    temp = (temp - 500) * -1 + 1000;
    receiver_aile = (int) temp;
  } else if (receiver_aile > 1500) {
     receiver_aile_course;
    if (receiver_aile_course >= 1) temp = (receiver_aile - 1500) * (1-(receiver_aile_course-1));
    else temp = (receiver_aile - 1500) * (1-receiver_aile_course+1);
    temp = temp + 1500;
    receiver_aile = (int) temp;
  }
  if (receiver_elev < 1500) {
    if (receiver_elev_course >= 1) temp = (receiver_elev - 1500) * -1 * (1-(receiver_elev_course-1));
    else temp = (receiver_elev - 1500) * -1 * (1-receiver_elev_course+1);
    temp = (temp - 500) * -1 + 1000;
    receiver_elev = (int) temp;
  } else if (receiver_elev > 1500) {
     receiver_elev_course;
    if (receiver_elev_course >= 1) temp = (receiver_elev - 1500) * (1-(receiver_elev_course-1));
    else temp = (receiver_elev - 1500) * (1-receiver_elev_course+1);
    temp = temp + 1500;
    receiver_elev = (int) temp;
  }
  if (receiver_rudd < 1500) {
    if (receiver_rudd_course >= 1) temp = (receiver_rudd - 1500) * -1 * (1-(receiver_rudd_course-1));
    else temp = (receiver_rudd - 1500) * -1 * (1-receiver_rudd_course+1);
    temp = (temp - 500) * -1 + 1000;
    receiver_rudd = (int) temp;
  } else if (receiver_rudd > 1500) {
     receiver_rudd_course;
    if (receiver_rudd_course >= 1) temp = (receiver_rudd - 1500) * (1-(receiver_rudd_course-1));
    else temp = (receiver_rudd - 1500) * (1-receiver_rudd_course+1);
    temp = temp + 1500;
    receiver_rudd = (int) temp;
  }
  receiver_thro = receiver_thro+receiver_thro_trim;
  receiver_aile = receiver_aile+receiver_aile_trim;
  receiver_elev = receiver_elev+receiver_elev_trim;
  receiver_rudd = receiver_rudd+receiver_rudd_trim;
  if (receiver_thro < 1000) receiver_thro = 1000;
  if (receiver_aile < 1000) receiver_aile = 1000;
  if (receiver_elev < 1000) receiver_elev = 1000;
  if (receiver_rudd < 1000) receiver_rudd = 1000;
  if (receiver_thro > 2000) receiver_thro = 2000;
  if (receiver_aile > 2000) receiver_aile = 2000;
  if (receiver_elev > 2000) receiver_elev = 2000;
  if (receiver_rudd > 2000) receiver_rudd = 2000;
  if (abs(receiver_thro - receiver_thro_prev) <= receiver_thro_deadband) receiver_thro = receiver_thro_prev;
  if (abs(receiver_aile - receiver_aile_prev) <= receiver_aile_deadband) receiver_aile = receiver_aile_prev;
  if (abs(receiver_elev - receiver_elev_prev) <= receiver_elev_deadband) receiver_elev = receiver_elev_prev;
  if (abs(receiver_rudd - receiver_rudd_prev) <= receiver_rudd_deadband) receiver_rudd = receiver_rudd_prev;
  if (receiver_thro_inversion) receiver_thro = (receiver_thro - 2000) * -1 + 1000;
  if (receiver_aile_inversion) receiver_aile = (receiver_aile - 2000) * -1 + 1000;
  if (receiver_elev_inversion) receiver_elev = (receiver_elev - 2000) * -1 + 1000;
  if (receiver_rudd_inversion) receiver_rudd = (receiver_rudd - 2000) * -1 + 1000;
  receiver_thro_prev = receiver_thro;
  receiver_aile_prev = receiver_aile;
  receiver_elev_prev = receiver_elev;
  receiver_rudd_prev = receiver_rudd;
}
bool receiverTest() {
  if (receiver_thro == 0 || receiver_aile == 0 || receiver_elev == 0 || receiver_rudd == 0) {
    return false;
  } else {
    return true;
  }
}

// Convertions
void convertReceiverToJoystick() {
  temp = (receiver_thro - 1000) * 0.255;
  joystick_throttle = (byte) temp;
  temp = (receiver_rudd - 1000) * 0.255;
  joystick_rudder = (byte) temp;
  temp = (receiver_aile - 1000) * 0.36;
  joystick_xaxis_rotation = (int) temp;
  temp = (receiver_elev - 1000) * 0.36;
  joystick_yaxis_rotation = (int) temp;
}
void convertReceiverToSerial() {
  temp = receiver_thro/100;
  serial_thro1 = (byte) temp;
  temp = receiver_thro-serial_thro1*100;
  serial_thro2 = (byte) temp;
  temp = receiver_aile/100;
  serial_aile1 = (byte) temp;
  temp = receiver_aile-serial_aile1*100;
  serial_aile2 = (byte) temp;
  temp = receiver_elev/100;
  serial_elev1 = (byte) temp;
  temp = receiver_elev-serial_elev1*100;
  serial_elev2 = (byte) temp;
  temp = receiver_rudd/100;
  serial_rudd1 = (byte) temp;
  temp = receiver_rudd-serial_rudd1*100;
  serial_rudd2 = (byte) temp;
}

// Joystick
void joystickUpdate() {
  Joystick.setThrottle(joystick_throttle);
  Joystick.setRudder(joystick_rudder);
  Joystick.setXAxisRotation(joystick_xaxis_rotation);
  Joystick.setYAxisRotation(joystick_yaxis_rotation);
}
void joystickCenter() {
  Joystick.setThrottle(0);
  Joystick.setRudder(127);
  Joystick.setXAxisRotation(180);
  Joystick.setYAxisRotation(180);
}

// Serial
void serialWait() {
  while (Serial.available() < 1);
}
void serialWrite(byte value) {
  while (Serial.availableForWrite() < 1);
  Serial.write(value);
}
void serialRead() {
  byte command[] = {0,0,0,0,0,0};
  int i = 0;
  while (Serial.available() > 0) {
    command[i] = Serial.read();
    i++;
  }
  if (command[0] == 114 && command[1] == 101 && command[2] == 97 && command[3] == 100 && command[4] == 121) serial_mode = 1; // Command ready
  if (command[0] == 101 && command[1] == 110 && command[2] == 100) serial_mode = 0; // Command end
  if (command[0] == 117 && command[1] == 112 && command[2] == 100 && command[3] == 97 && command[4] == 116 && command[5] == 101) serial_mode = 3; // Command update
}
void serialSend() {
  if (serial_mode > 0) {
    if (joystick) {
      Joystick.end();
      joystick = false;
    }
    delay(1000);
  } else {
    if (!joystick) {
      Joystick.begin();
      joystick = true;
    }
  }
  switch (serial_mode) {
    case 1:
      for (int i=0; i<33; i++) serialWrite(EEPROM.read(i));
      serial_mode = 2;
      break;
    case 2:
      convertReceiverToSerial();
      if (receiverTest()) serialWrite((byte) 1);
      else serialWrite((byte) 0);
      serialWrite(serial_thro1);
      serialWrite(serial_thro2);
      serialWrite(serial_aile1);
      serialWrite(serial_aile2);
      serialWrite(serial_elev1);
      serialWrite(serial_elev2);
      serialWrite(serial_rudd1);
      serialWrite(serial_rudd2);
      break;
    case 3:
      for (int i=0; i<33; i++) {
        serialWait();
        EEPROM.put(i, Serial.read());
      }
      eepromRead();
      serial_mode = 2;
      break;
  }
}

// Arduino
void setup() {
  delay(1000);
  Serial.begin(9600);
  pinMode(PIN_RECEIVER_1, INPUT);
  pinMode(PIN_RECEIVER_2, INPUT);
  pinMode(PIN_RECEIVER_3, INPUT);
  pinMode(PIN_RECEIVER_4, INPUT);
  Joystick.begin();
  eepromInit();
}
void loop() {
  receiverUpdate();
  if (receiverTest()) {
    receiverTuning();
    convertReceiverToJoystick();
    joystickUpdate();
  } else {
    joystickCenter();
  }
  serialRead();
  serialSend();
}
