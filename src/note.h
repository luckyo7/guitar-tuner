#include <Arduino.h>

#ifndef NOTE_H
#define NOTE_H

constexpr int MAX_NOTES = 25;

struct Note {
  String note;
  String accidental;
  float frequency;
};

struct NoteAndError {
  Note note;
  float error;
};

struct Mode {
  int numNotes;
  Note availableNotes[MAX_NOTES];
  String modeName;
};

// Mode currentMode; // want this to be private
const Mode &getCurrentMode();
void setCurrentMode(int modeIndex);
void incrementCurrentMode();

float getFrequency(String note, Mode currentMode);
String getNote(float frequency, Mode currentMode);

NoteAndError findNearestNote(float frequency);

#endif