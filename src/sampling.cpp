#include <Arduino.h>
#include <i2s.h>
#include <note.h>
#include <screen.h>
#include <yin.h>

// uses i2s to sample and analyze mems microphone signal
#define SAMPLE_RATE 16000
#define SAMPLES 1024 // Must be a power of 2
#define PITCH_THRESHOLD 0.2

// moving average
const int movSize = 5; // size of the moving average window
float previousFrequencies[movSize] = {0.0, 0.0, 0.0, 0.0, 0.0};
float maxError = 2; // maximum difference allowed between moving average mean
                    // and sample frequency
// moving average

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

  bool withinMovAvg =
      checkMovingAverage(frequency, movSize, previousFrequencies, maxError);

  appendFrequency(frequency, previousFrequencies, movSize);

  if (withinMovAvg && frequency > 0) {
    NoteAndError nearestNote = findNearestNote(frequency);
    Note note = nearestNote.note;
    float errorFrac = nearestNote.error / 2.0;

    Serial.println(note.note);

    display.clearDisplay();
    drawNote(note.note, "");
    drawPitch(errorFrac);
    delay(50);
  } else {
    display.clearDisplay();
    drawPitch(0);
  }
  delay(100);
}