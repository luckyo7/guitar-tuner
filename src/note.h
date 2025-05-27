#include <Arduino.h>

#ifndef NOTE_H
#define NOTE_H

struct Note {
  String note;
  String accidental;
  float frequency;
};

struct NoteAndError {
  Note note;
  float error;
};

float getFrequency(String note);
String getNote(float frequency);

NoteAndError findNearestNote(float frequency);

#endif