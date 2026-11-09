#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#include <ResponsiveAnalogRead.h>
#include <color/color.hpp>

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

constexpr uint8_t MODE_SWITCH_PIN = 26;

LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN,
                  LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

ResponsiveAnalogRead inputs[] = {
  ResponsiveAnalogRead(POT_R_PIN, true),
  ResponsiveAnalogRead(POT_G_PIN, true),
  ResponsiveAnalogRead(POT_B_PIN, true)
};

void setup() {
  pinMode(MODE_SWITCH_PIN, INPUT_PULLUP);
  analogReadResolution(12);

  for (ResponsiveAnalogRead& input : inputs) {
    input.setAnalogResolution(4096);
    input.setActivityThreshold(64);
    input.setSnapMultiplier(0.003f);
  }

  lcd.begin(16, 2);
  pixels.begin();
  pixels.setBrightness(32);
  pixels.clear();
  pixels.show();
}

void loop() {
  uint8_t values[3];
  for (uint8_t i = 0; i < 3; ++i) {
    inputs[i].update();
    values[i] = map(inputs[i].getValue(), 4095, 0, 0, 255);
  }

  const bool isHsiMode = digitalRead(MODE_SWITCH_PIN) == HIGH;
  if (isHsiMode) {
    color::rgb<float> rgb(color::hsi<uint8_t>(
        {values[0], values[1], values[2]}));

    pixels.fill(pixels.Color(
        constrain(color::get::red(rgb) * 255.0f, 0.0f, 255.0f),
        constrain(color::get::green(rgb) * 255.0f, 0.0f, 255.0f),
        constrain(color::get::blue(rgb) * 255.0f, 0.0f, 255.0f)));
  } else {
    pixels.fill(pixels.Color(values[0], values[1], values[2]));
  }

  pixels.show();
  lcd.setCursor(0, 0);
  lcd.printf("%c:%03d %c:%03d",
             isHsiMode ? 'H' : 'R',
             isHsiMode ? values[0] * 360 / 255 : values[0],
             isHsiMode ? 'S' : 'G',
             isHsiMode ? values[1] * 100 / 255 : values[1]);

  lcd.setCursor(0, 1);
  lcd.printf("%c:%03d", isHsiMode ? 'I' : 'B',
             isHsiMode ? values[2] * 100 / 255 : values[2]);

  delay(100);
}
