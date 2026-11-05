#include <cstdio>

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#include <ResponsiveAnalogRead.h>

constexpr uint8_t LCD_RS_PIN = 23;
constexpr uint8_t LCD_ENABLE_PIN = 22;
constexpr uint8_t LCD_D4_PIN = 21;
constexpr uint8_t LCD_D5_PIN = 19;
constexpr uint8_t LCD_D6_PIN = 18;
constexpr uint8_t LCD_D7_PIN = 5;

constexpr uint8_t LED_PIN = 25;
constexpr uint16_t LED_COUNT = 25;

constexpr uint8_t POT_R_PIN = 32;
constexpr uint8_t POT_G_PIN = 33;
constexpr uint8_t POT_B_PIN = 34;

LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN,
                  LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

ResponsiveAnalogRead inputs[] = {
  ResponsiveAnalogRead(POT_R_PIN, true),
  ResponsiveAnalogRead(POT_G_PIN, true),
  ResponsiveAnalogRead(POT_B_PIN, true)
};

void setup() {
  analogReadResolution(12);
  for (ResponsiveAnalogRead& input : inputs) {
    input.setAnalogResolution(4096);
    input.setActivityThreshold(64);
    input.setSnapMultiplier(0.003f);
  }

  lcd.begin(16, 2);
  pixels.begin();
  pixels.setBrightness(64);
  pixels.clear();
  pixels.show();
}

void loop() {
  int color[3];
  for (uint8_t i = 0; i < 3; ++i) {
    inputs[i].update();
    color[i] = map(inputs[i].getValue(), 4095, 0, 0, 255);
  }

  pixels.fill(pixels.Color(color[0], color[1], color[2]));
  pixels.show();

  char displayText[12];
  std::snprintf(displayText, sizeof(displayText), "%03d %03d %03d", color[0],
                color[1], color[2]);

  lcd.setCursor(0, 0);
  lcd.print(displayText);

  delay(100);
}
