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

const Mode allModes[5];

Mode currentMode;
Mode getCurrentMode();
void setCurrentMode(int modeIndex);

float getFrequency(String note);
String getNote(float frequency);

NoteAndError findNearestNote(float frequency);

#endif