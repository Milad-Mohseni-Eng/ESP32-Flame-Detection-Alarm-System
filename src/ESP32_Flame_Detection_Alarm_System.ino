#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// -----------------------------
// PINS
// -----------------------------

#define FLAME_SENSOR 27

#define GREEN_LED 15
#define RED_LED 2

#define BUZZER 14

// -----------------------------
// BUZZER PWM
// -----------------------------

#define BUZZER_CHANNEL 0
#define BUZZER_FREQ 4000
#define BUZZER_RESOLUTION 8

unsigned long fireCounter = 0;

// -----------------------------
// SETUP
// -----------------------------

void setup()
{
  Serial.begin(115200);

  pinMode(FLAME_SENSOR, INPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  Wire.begin(21,22);

  if(!display.begin(
      SSD1306_SWITCHCAPVCC,
      0x3C))
  {
    while(true);
  }

  // ESP32 PWM Buzzer

  ledcAttach(BUZZER,
             BUZZER_FREQ,
             BUZZER_RESOLUTION);

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(10,20);
  display.println("FLAME");

  display.display();

  delay(2000);
}

// -----------------------------
// LOOP
// -----------------------------

void loop()
{
  int flameState =
      digitalRead(FLAME_SENSOR);

  display.clearDisplay();

  display.setTextColor(
      SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(0,0);

  display.println(
      "ESP32 FLAME MONITOR");

  if(flameState == HIGH)
  {
    fireCounter++;

    digitalWrite(
      GREEN_LED,
      LOW);

    digitalWrite(
      RED_LED,
      HIGH);

    // Loud buzzer

    ledcWriteTone(
      BUZZER,
      4000);

    Serial.println(
      "FIRE DETECTED");

    display.setTextSize(2);

    display.setCursor(0,20);

    display.println("FIRE!");

    display.setTextSize(1);

    display.setCursor(0,50);

    display.print("Count: ");

    display.print(
      fireCounter);
  }
  else
  {
    digitalWrite(
      GREEN_LED,
      HIGH);

    digitalWrite(
      RED_LED,
      LOW);

    // Stop buzzer

    ledcWriteTone(
      BUZZER,
      0);

    Serial.println(
      "SAFE");

    display.setTextSize(2);

    display.setCursor(0,20);

    display.println("SAFE");

    display.setTextSize(1);

    display.setCursor(0,50);

    display.println(
      "No Flame");
  }

  display.display();

  delay(200);
}