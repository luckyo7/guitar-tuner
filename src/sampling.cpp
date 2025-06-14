#include <Arduino.h>
#include <i2s.h>
#include <note.h>
#include <screen.h>
#include <yin.h>

// uses i2s to sample and analyze mems microphone signal
#define SAMPLE_RATE 16000
#define SAMPLES 1024 // Must be a power of 2
#define PITCH_THRESHOLD 0.2

#define NOTE_DISPLAY_TIMEOUT                                                   \
  2000 // wait 2 seconds before clearing display after a result is found
unsigned long lastNoteTime = 0;
String prevNote = "-"; // only want to update note displayed if note has changed

// moving average
const int movSize = 5; // size of the moving average window
float previousFrequencies[movSize] = {0.0, 0.0, 0.0, 0.0, 0.0};
float maxError = 2; // maximum difference allowed between moving average mean
                    // and sample frequency
// moving average

void sampling_loop() {
  unsigned long currTime = millis(); // current time

  if (lastNoteTime + NOTE_DISPLAY_TIMEOUT < currTime && prevNote != "-") {
    Serial.println("Note has timed out");
    // we have timed out
    clearNote();
    clearPitch();
    drawPitch(0);
    drawNote("-", "");

    prevNote = "-";
    lastNoteTime = currTime;
  }

  float samples[SAMPLES];
  size_t bytes_read;

  for (int i = 0; i < SAMPLES; i++) {
    int32_t sample;

    i2s_read(I2S_NUM_0, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);

    samples[i] = sample;
  }

  float frequency = detectPitch(samples, SAMPLES, SAMPLE_RATE, PITCH_THRESHOLD);

  if (frequency < 0) { // got a bad result
    // unsigned long currTime = millis(); // current time

    return;
  }

  Serial.println("Detected frequency: " + String(frequency));

  bool withinMovAvg =
      checkMovingAverage(frequency, movSize, previousFrequencies, maxError);

  appendFrequency(frequency, previousFrequencies, movSize);

  const Mode &currentMode = getCurrentMode();
  if (withinMovAvg && frequency > 0) {
    NoteAndError nearestNote = findNearestNote(frequency);
    const Note &note = nearestNote.note;
    float errorFrac = nearestNote.error / 2.0;

    Serial.println(note.note);

    lastNoteTime = millis();
    clearPitch();
    drawPitch(errorFrac);
    if (note.note != prevNote) {
      prevNote = note.note;
      clearNote();
      drawNote(note.note, "");
      delay(500);
    }
  }
  // drawMode(currentMode.modeName, currentMode.noteString); // always draw mode
  delay(100);
}