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
  const Note note;
  float error;
};

struct Mode {
  int numNotes;
  const Note availableNotes[MAX_NOTES];
  const String modeName;
  const String noteString; // string of notes to be displayed
};

// Mode currentMode; // want this to be private
const Mode &getCurrentMode();
void setCurrentMode(int modeIndex);
void incrementCurrentMode();

float getFrequency(String &note, const Mode &currentMode);
String getNote(float frequency, const Mode &currentMode);

const NoteAndError findNearestNote(float frequency);

#endif