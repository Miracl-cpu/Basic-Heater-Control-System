#include <DHT.h>

// Pin configuration
const int dhtPin = 15;       // DHT22 data pin
const int heaterPin = 5;     // Heater (LED)
const int redPin = 12;       // RGB Red
const int greenPin = 13;     // RGB Green
const int bluePin = 14;      // RGB Blue

// Sensor setup
DHT dht(dhtPin, DHT22);

// Temperature thresholds
float targetTemp = 50.0;
float hysteresis = 2.0;
float overheatTemp = 70.0;

// Heater states
enum HeaterState {
  IDLE,
  HEATING,
  STABILIZING,
  TARGET_REACHED,
  OVERHEAT
};

HeaterState currentState = IDLE;

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(heaterPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.println("Heater Control System Started");
}

void loop() {
  float temp = dht.readTemperature();

  if (isnan(temp)) {
    Serial.println("Failed to read from DHT22 sensor");
    delay(1000);
    return;
  }

  updateState(temp);
  controlHeater();
  updateLED();

  printStatus(temp);
  delay(1000);
}

void updateState(float temp) {
  switch (currentState) {
    case IDLE:
      if (temp < targetTemp - hysteresis) currentState = HEATING;
      break;
    case HEATING:
      if (temp >= targetTemp - 1) currentState = STABILIZING;
      break;
    case STABILIZING:
      if (temp >= targetTemp) currentState = TARGET_REACHED;
      break;
    case TARGET_REACHED:
      if (temp > overheatTemp) currentState = OVERHEAT;
      else if (temp < targetTemp - hysteresis) currentState = HEATING;
      break;
    case OVERHEAT:
      if (temp < targetTemp) currentState = IDLE;
      break;
  }
}

void controlHeater() {
  if (currentState == HEATING || currentState == STABILIZING) {
    digitalWrite(heaterPin, HIGH);
  } else {
    digitalWrite(heaterPin, LOW);
  }
}

void updateLED() {
  switch (currentState) {
    case IDLE:            setRGB(255, 255, 255); break; // OFF (all HIGH)
    case HEATING:         setRGB(0, 90, 255);     break; // Orange (R+G low)
    case STABILIZING:     setRGB(255, 0, 255);    break; // Green
    case TARGET_REACHED:  setRGB(255, 255, 0);    break; // Blue
    case OVERHEAT:        setRGB(0, 255, 255);    break; // Red
  }
}

// For COMMON ANODE: 0 = ON, 255 = OFF
void setRGB(int r, int g, int b) {
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}

void printStatus(float temp) {
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" °C | State: ");
  switch (currentState) {
    case IDLE: Serial.println("IDLE"); break;
    case HEATING: Serial.println("HEATING"); break;
    case STABILIZING: Serial.println("STABILIZING"); break;
    case TARGET_REACHED: Serial.println("TARGET_REACHED"); break;
    case OVERHEAT: Serial.println("OVERHEAT"); break;
  }
}
