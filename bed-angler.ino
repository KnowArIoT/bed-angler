
int PIN_POT_1 = A0;
int PIN_POT_2 = A1;
int PIN_UP_1 = D2;
int PIN_DOWN_1 = D3;
int PIN_UP_2 = D4;
int PIN_DOWN_2 = D5,

float angle1 = 0;
float angle2 = 0;

int POT_MAX_DEGREE = 300;
int ANGLE_HYSTERESIS = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_UP_1, OUTPUT);
  pinMode(PIN_DOWN_1, OUTPUT);
  pinMode(PIN_UP_2, OUTPUT);
  pinMode(PIN_DOWN_2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  update();
}

void update() {
  updateSerial();
  updateAngles();
}

void updateSerial() {
  if (Serial.available()) {
    char input = Serial.read();
    switch (input) {
      // who am i
      case 'W':
        Serial.println("Bed-angler");
        break;
      // set angle1
      case 'A': {
        while (!Serial.available());
        uint8_t newAngle1 = Serial.read();
        angle1 = newAngle1;
        break;
      }
      // set angle2
      case 'B': {
        while (!Serial.available());
        uint8_t newAngle2 = Serial.read();
        angle2 = newAngle2;
        break;
      }
    }
  }
}

void updateAngles() {
  if (getCurrentAngle1() < angle1 - DEGREE_HYSTERESIS) {
    digitalWrite(PIN_UP_1, HIGH);
  } else if (getCurrentAngle1() > angle1 + DEGREE_HYSTERESIS) {
    digitalWrite(PIN_DOWN_1, HIGH);
  } else {
    digitalWrite(PIN_UP_1, LOW);
    digitalWrite(PIN_DOWN_1, LOW);
  }

  if (getCurrentAngle2() < angle2 - DEGREE_HYSTERESIS) {
    digitalWrite(PIN_UP_2, HIGH);
  } else if (getCurrentAngle2() > angle2 + DEGREE_HYSTERESIS) {
    digitalWrite(PIN_DOWN_2, HIGH);
  } else {
    digitalWrite(PIN_UP_2, LOW);
    digitalWrite(PIN_DOWN_2, LOW);
  }
  
}
int getCurrentAngle1() {
  return getAngleOfPotmeter(PIN_POT_1);
}

int getCurrentAngle2() {
  return getAngleOfPotmeter(PIN_POT_2);
}

int getAngleOfPotmeter(int pin) {
  int adcValue = analogRead(pin);
  int angle = adcValue * (POT_MAX_DEGREE / 1024);
  return angle;
}

