#include <Arduino.h>
#include <LiquidCrystal.h>
#include <ResponsiveAnalogRead.h>

LiquidCrystal lcd(23, 22, 21, 19, 18, 5);

constexpr uint8_t POT_R_PIN = 32;
constexpr uint8_t POT_G_PIN = 33;
constexpr uint8_t POT_B_PIN = 34;

ResponsiveAnalogRead redInput(POT_R_PIN, true);
ResponsiveAnalogRead greenInput(POT_G_PIN, true);
ResponsiveAnalogRead blueInput(POT_B_PIN, true);

void setup() {
  analogReadResolution(12);
  redInput.setAnalogResolution(4096);
  greenInput.setAnalogResolution(4096);
  blueInput.setAnalogResolution(4096);
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {
  redInput.update();
  greenInput.update();
  blueInput.update();

  const int red = map(redInput.getValue(), 4095, 0, 0, 255);
  const int green = map(greenInput.getValue(), 4095, 0, 0, 255);
  const int blue = map(blueInput.getValue(), 4095, 0, 0, 255);

  lcd.setCursor(0, 0);
  if (red < 100) lcd.print('0');
  if (red < 10) lcd.print('0');
  lcd.print(red);
  lcd.print(' ');
  if (green < 100) lcd.print('0');
  if (green < 10) lcd.print('0');
  lcd.print(green);
  lcd.print(' ');
  if (blue < 100) lcd.print('0');
  if (blue < 10) lcd.print('0');
  lcd.print(blue);

  delay(100);
}
