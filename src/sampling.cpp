#include <Arduino.h>
#include <i2s.h>
#include <yin.h>

// uses i2s to sample and analyze mems microphone signal
#define SAMPLE_RATE 16000
#define SAMPLES 1024 // Must be a power of 2
#define PITCH_THRESHOLD 0.2

void sampling_loop() {
  float samples[SAMPLES];
  size_t bytes_read;

  for (int i = 0; i < SAMPLES; i++) {
    int32_t sample;

    i2s_read(I2S_NUM_0, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);

    samples[i] = sample;
  }

  float frequency = detectPitch(samples, SAMPLES, SAMPLE_RATE, PITCH_THRESHOLD);

  Serial.println("Detected frequency: " + String(frequency));
}