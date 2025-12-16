#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

// sensor pins
#define TRIG_PIN 3
#define ECHO_PIN 2

// buzzer pin
#define BUZZER_PIN 7

// led pins
int segPins[7] = {4, 5, 6, 8, 9, 10, 11};

// Shared distance variable (volatile to prevent compiler optimization)
volatile int distance_cm = -1;

// Variable to store echo pulse duration
float duration_us;

// led displayed digits combinations
byte digits[11][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}, // 9
    {1, 0, 0, 1, 1, 1, 1}  // E
};

// function to display the giving digit
void displayDigit(int num)
{
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(segPins[i], digits[num][i]);
  }
}

// task 01 : using the ultrasonic for distance measurement + update the distance variable
void ultrasonicTask(void *pvParameters)
{
  while (1)
  {
    // Send a 10 µs HIGH pulse to trigger pin
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Read echo duration in microseconds
    duration_us = pulseIn(ECHO_PIN, HIGH);

    // convert time to distance
    distance_cm = 0.017 * duration_us;

    // displaying the distance in the serial monitor
    Serial.print("distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");

    // block the task for this time
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// task 02 : displaying the distance in the led
void displayTask(void *pvParameters)
{
  while (1)
  {
    int d = distance_cm;

    if (d >= 0 && d <= 450)
    {
      // if in the range then display it
      displayDigit(d / 50);
    }
    else
    {
      // else display e for error (out of range)
      displayDigit(10);
    }

    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// task 03 :  Controls buzzer frequency based on distance (closer = faster beep)

void buzzerTask(void *pvParameters)
{
  while (1)
  {
    int d = distance_cm;

    if (d > 0 && d <= 450)
    {
      // Map distance to delay: closer objects = shorter delay (faster beep)
      int beepDelay = map(d, 450, 20, 500, 50);
      digitalWrite(BUZZER_PIN, HIGH);
      vTaskDelay(100 / portTICK_PERIOD_MS);

      digitalWrite(BUZZER_PIN, LOW);
      vTaskDelay(beepDelay / portTICK_PERIOD_MS);
    }
    else
    {

      // Turn buzzer OFF if out of range
      digitalWrite(BUZZER_PIN, LOW);
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  for (int i = 0; i < 7; i++)
  {
    pinMode(segPins[i], OUTPUT);
  }

  // creation of each task : ultrasonic, display, buzzer
  xTaskCreate(ultrasonicTask, "Ultrasonic", 128, NULL, 1, NULL);
  xTaskCreate(displayTask, "Display", 128, NULL, 1, NULL);
  xTaskCreate(buzzerTask, "Buzzer", 128, NULL, 1, NULL);
}

void loop()
{
  // FreeRTOS handles tasks
}
