const int TRAFFIC_LIGHT1[] = {2,3,4};
const int TRAFFIC_LIGHT2[] = {8,12,13};
const int SENSOR1 = 11;
const int SENSOR2 = 7;
const int LEDSENSOR1 = A5;
const int LEDSENSOR2 = A4;


enum {RED1_GREEN2, YELLOW1_RED2, RED_BOTH, GREEN1_RED2, RED1_YELLOW2} trafficState = RED1_GREEN2;
const char* stateNames[] = {"RED1_GREEN2", "YELLOW1_RED2", "RED_BOTH", "GREEN1_RED2", "RED1_YELLOW2"};

unsigned long interval;
unsigned long previousMillis = 0;

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(TRAFFIC_LIGHT1[i], OUTPUT);
    pinMode(TRAFFIC_LIGHT2[i], OUTPUT);
  }
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(LEDSENSOR1, OUTPUT);
  pinMode(LEDSENSOR2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  int presence1 = digitalRead(SENSOR1);
  int presence2 = digitalRead(SENSOR2);
  if (presence1) {
    analogWrite(LEDSENSOR1, 255);
  } else {
    analogWrite(LEDSENSOR1, 0);
  }
  if (presence2) {
    analogWrite(LEDSENSOR2, 255);
  } else {
    analogWrite(LEDSENSOR2, 0);
  }

  if (presence1 && !presence2) {
    quickChange(TRAFFIC_LIGHT1, TRAFFIC_LIGHT2);
    trafficState = RED1_GREEN2;
  } else if (presence2 && !presence1) {
    quickChange(TRAFFIC_LIGHT2, TRAFFIC_LIGHT1);
    trafficState = GREEN1_RED2;
  } else if (currentMillis - previousMillis >= interval) {
    standardTraffic();
  }
}

void turnOnLight(const int LIGHT[], String color) {
  int lightIndex;
  if (color == "red") {
    lightIndex = 2;
  } else if (color == "yellow") {
    lightIndex = 1;
  } else if (color == "green") {
    lightIndex = 0;
  }
  Serial.print("lightIndex");
  Serial.println(lightIndex);
  for (int i = 0; i < 3; i++) {
    Serial.print("light[i]");
    Serial.println(LIGHT[i]);
    digitalWrite(LIGHT[i], i == lightIndex ? HIGH : LOW);
  }
}

void quickChange(const int PRIORITY_LIGHT[], const int SECONDARY_LIGHT[]) {
  turnOnLight(SECONDARY_LIGHT, "yellow");
  delay(2000);
  turnOnLight(SECONDARY_LIGHT, "red");
  turnOnLight(PRIORITY_LIGHT, "green");
  previousMillis = millis();
  interval = 5000;
}

void standardTraffic() {
  Serial.print("Estado: ");
  Serial.println(stateNames[trafficState]);
  switch (trafficState) {
    case RED1_GREEN2:
      turnOnLight(TRAFFIC_LIGHT1, "red");
      turnOnLight(TRAFFIC_LIGHT2, "green");
      interval = 5000;
      trafficState = RED1_YELLOW2;
      break;
    case YELLOW1_RED2:
      turnOnLight(TRAFFIC_LIGHT1, "yellow");
      turnOnLight(TRAFFIC_LIGHT2, "red");
      interval = 2000;
      trafficState = RED1_GREEN2;
      break;
    case GREEN1_RED2:
      turnOnLight(TRAFFIC_LIGHT1, "green");
      turnOnLight(TRAFFIC_LIGHT2, "red");
      interval = 5000;
      trafficState = YELLOW1_RED2;
      break;
    case RED1_YELLOW2:
      turnOnLight(TRAFFIC_LIGHT1, "red");
      turnOnLight(TRAFFIC_LIGHT2, "yellow");
      interval = 2000;
      trafficState = GREEN1_RED2;
      break;
  }

  previousMillis = millis();
}
