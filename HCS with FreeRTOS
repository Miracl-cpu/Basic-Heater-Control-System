#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22

#define HEATER_PIN 5
#define RED_PIN 12
#define GREEN_PIN 13
#define BLUE_PIN 14

// Thresholds
float targetTemp = 50.0;
float hysteresis = 2.0;
float overheatTemp = 70.0;

DHT dht(DHTPIN, DHTTYPE);

// Queue to share temperature
QueueHandle_t tempQueue;

// Heater states
enum HeaterState {
  IDLE,
  HEATING,
  STABILIZING,
  TARGET_REACHED,
  OVERHEAT
};

HeaterState currentState = IDLE;

// Forward declarations
void sensorTask(void *pvParameters);
void controlTask(void *pvParameters);
void ledTask(void *pvParameters);
void setRGB(int r, int g, int b);

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(HEATER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Create queue for temperature
  tempQueue = xQueueCreate(1, sizeof(float));

  // Create FreeRTOS tasks
  xTaskCreatePinnedToCore(sensorTask, "Sensor Task", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(controlTask, "Control Task", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(ledTask, "LED Task", 2048, NULL, 1, NULL, 1);

  Serial.println("ESP32 FreeRTOS Heater Control Started");
}

void loop() {
  // Nothing here — using FreeRTOS tasks
}

// ------------------- TASKS ------------------------

void sensorTask(void *pvParameters) {
  float temp;

  while (1) {
    temp = dht.readTemperature();

    if (!isnan(temp)) {
      xQueueOverwrite(tempQueue, &temp);
    } else {
      Serial.println("Failed to read from DHT sensor");
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void controlTask(void *pvParameters) {
  float temp;

  while (1) {
    if (xQueuePeek(tempQueue, &temp, portMAX_DELAY)) {
      // State machine
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

      // Heater control
      if (currentState == HEATING || currentState == STABILIZING) {
        digitalWrite(HEATER_PIN, HIGH);
      } else {
        digitalWrite(HEATER_PIN, LOW);
      }

      // Always log temp and state
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

    vTaskDelay(pdMS_TO_TICKS(1000));  // Log every second
  }
}

void ledTask(void *pvParameters) {
  while (1) {
    switch (currentState) {
      case IDLE:            setRGB(255, 255, 255); break; // OFF
      case HEATING:         setRGB(0, 90, 255);     break; // Orange
      case STABILIZING:     setRGB(255, 0, 255);    break; // Green
      case TARGET_REACHED:  setRGB(255, 255, 0);    break; // Blue
      case OVERHEAT:        setRGB(0, 255, 255);    break; // Red
    }

    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

// COMMON ANODE RGB LED: 0 = ON, 255 = OFF
void setRGB(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}
