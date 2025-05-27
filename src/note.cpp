#include <Arduino.h>
#include <note.h>
// Take frequency of sound, return the closest note and how far off it is

// const int numNotes = 25;
// Note notes[numNotes] = {
//     {"E", "", 82.41},   {"F", "", 87.31},   {"F", "#", 92.50},
//     {"G", "", 98.00},   {"G", "#", 103.83}, {"A", "", 110.00},
//     {"B", "b", 116.54}, {"B", "", 123.47},  {"C", "", 130.81},
//     {"C", "#", 138.59}, {"D", "", 146.83},  {"E", "b", 155.56},
//     {"E", "", 164.81},  {"F", "", 174.61},  {"F", "#", 185.00},
//     {"G", "", 196.00},  {"G", "#", 207.65}, {"A", "", 220.00},
//     {"B", "b", 233.08}, {"B", "", 246.94},  {"C", "", 261.63},
//     {"C", "#", 277.18}, {"D", "", 293.66},  {"E", "b", 311.13},
//     {"E", "", 329.63}};

const int numNotes = 9;
Note notes[numNotes] =
    {
        {"E", "", 82.41},  {"A", "", 110.00}, {"D", "", 146.83},
        {"G", "", 196.00}, {"B", "", 246.94}, {"E", "", 329.63},
        {"E", "", 164.82}, // 2nd harmonic of low E
        {"A", "", 220.00}, // 2nd harmonic of A
        {"D", "", 293.66}, // 2nd harmonic of D
};

float getFrequency(String note) {
  for (int i = 0; i < numNotes; i++) {
    Note string = notes[i];
    if (note == string.note) {
      return string.frequency;
    }
  }

  return -1;
}

float freqError = 0.1; // compare floats accurately
String getNote(float frequency) {
  for (int i = 0; i < numNotes; i++) {
    Note string = notes[i];
    if (abs(frequency - string.frequency) <= freqError) {
      return string.note;
    }
  }

  return "";
}

/*
find the nearest guitar string from a given frequency
loop through all guitar strings, find the nearest frequency
need to also prolly check some harmonics too
*/

NoteAndError findNearestNote(float frequency) {
  Note nearestString = {"", "", 0};
  float minError = 1000.0; // should be big enough
  float maxError = 10.0;   // we want to ignore notes that are very far off

  for (int i = 0; i < numNotes; i++) {
    Note string = notes[i];
    float error = abs(frequency - string.frequency);

    if (error <= minError && error < maxError) {
      minError = error;
      nearestString = string;
    }
  }

  // if (minError > minHarmonicError) {
  //   // error is too high, start checking harmonics
  //   for (int harmonic = minHarmonic; harmonic <= maxHarmonic; harmonic++) {
  //     // ewww nested loops
  //     for (int i = 0; i < numNotes; i++) {
  //       Note string = notes[i];
  //       if (abs(frequency - string.frequency) <= minError) {
  //         minError = abs(frequency - string.frequency);
  //         nearestString = string;
  //       }
  //     }
  //   }
  // }

  return {nearestString, minError};
}