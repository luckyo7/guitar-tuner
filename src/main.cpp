#include <Arduino.h>
#include <buttons.h>
#include <i2s.h>
#include <note.h>
#include <sampling.h>
#include <screen.h>

void loop() {
  button_loop();
  sampling_loop();
}

void setup() {
  Serial.begin(115200);

  screenSetup();
  display.clearDisplay();

  drawNote("-", "");
  drawPitch(0.0);

  // i2s setup
  i2s_install();
  i2s_setpin();
  i2s_start(I2S_PORT);

  button_setup();

  delay(500);
}