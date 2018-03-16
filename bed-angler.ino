
int PIN_POT_1 = A0;
int PIN_POT_2 = A1;
int PIN_UP_1 = 2;
int PIN_DOWN_1 = 4;
int PIN_UP_2 = 3;
int PIN_DOWN_2 = 5;

float angle1 = 0;
float angle2 = 0;

float pot1angle = 0;
float pot2angle = 0;

int POT_MAX_DEGREE = 300;
int DEGREE_HYSTERESIS = 2;
int TIME_DELTA_MS = 500;

int last_update = millis();

const int numReadings = 10;

int readings1[numReadings];      // the readings from the analog input
int readIndex1 = 0;              // the index of the current reading
int total1 = 0;                  // the running total
int readings2[numReadings];      // the readings from the analog input
int readIndex2 = 0;              // the index of the current reading
int total2 = 0;                  // the running total

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_UP_1, OUTPUT);
  pinMode(PIN_DOWN_1, OUTPUT);
  pinMode(PIN_UP_2, OUTPUT);
  pinMode(PIN_DOWN_2, OUTPUT);
  pinMode(PIN_POT_1, INPUT);
  pinMode(PIN_POT_2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  update();
}

void update() {
  updateSerial();
  updateAngles();
  updateMotors();
  //Serial.println(getAngleOfPotmeter(PIN_POT_1));
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

void updateMotors() {
  if (last_update + TIME_DELTA_MS < millis()) return;
  if (pot1angle < angle1 - DEGREE_HYSTERESIS) {
    digitalWrite(PIN_DOWN_1, LOW);
    digitalWrite(PIN_UP_1, HIGH);
  } else if (pot1angle > angle1 + DEGREE_HYSTERESIS) {
    digitalWrite(PIN_UP_1, LOW);
    digitalWrite(PIN_DOWN_1, HIGH);
  } else {
    digitalWrite(PIN_UP_1, LOW);
    digitalWrite(PIN_DOWN_1, LOW);
  }

    if (pot2angle < angle2 - DEGREE_HYSTERESIS) {
    digitalWrite(PIN_DOWN_2, LOW);
    digitalWrite(PIN_UP_2, HIGH);
  } else if (pot2angle > angle2 + DEGREE_HYSTERESIS) {
    digitalWrite(PIN_UP_2, LOW);
    digitalWrite(PIN_DOWN_2, HIGH);
  } else {
    digitalWrite(PIN_UP_2, LOW);
    digitalWrite(PIN_DOWN_2, LOW);
  }

  last_update = millis();
  
}

void updateAngles() {
  getCurrentAngle1();
  getCurrentAngle2();
}

void getCurrentAngle1() {
  total1 = total1 - readings1[readIndex1];
  // read from the sensor:
  readings1[readIndex1] = analogRead(PIN_POT_1);
  // add the reading to the total:
  total1 = total1 + readings1[readIndex1];
  // advance to the next position in the array:
  readIndex1 = readIndex1 + 1;

  // if we're at the end of the array...
  if (readIndex1 >= numReadings) {
    // ...wrap around to the beginning:
    readIndex1 = 0;
  }

  // calculate the average:
  pot1angle = float(float(total1 / numReadings) * (float(POT_MAX_DEGREE) / 1024.0));
}

int getCurrentAngle2() {
  total2 = total2 - readings2[readIndex2];
  // read from the sensor:
  readings2[readIndex2] = analogRead(PIN_POT_2);
  // add the reading to the total:
  total2 = total2 + readings2[readIndex2];
  // advance to the next position in the array:
  readIndex2 = readIndex2 + 1;

  // if we're at the end of the array...
  if (readIndex2 >= numReadings) {
    // ...wrap around to the beginning:
    readIndex2 = 0;
  }

  // calculate the average:
  pot2angle = float(float(total2 / numReadings) * (float(POT_MAX_DEGREE) / 1024.0));
}

int getAngleOfPotmeter(int pin) {
  int adcValue = analogRead(pin);
  int angle = int(float(adcValue) * (float(POT_MAX_DEGREE) / 1024.0));
  return angle;
}

