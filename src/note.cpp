#include <Arduino.h>
#include <note.h>
// Take frequency of sound, return the closest note and how far off it is

const Mode allTuning = {
    25,
    {{"E", "", 82.41},   {"F", "", 87.31},   {"F", "#", 92.50},
     {"G", "", 98.00},   {"G", "#", 103.83}, {"A", "", 110.00},
     {"B", "b", 116.54}, {"B", "", 123.47},  {"C", "", 130.81},
     {"C", "#", 138.59}, {"D", "", 146.83},  {"E", "b", 155.56},
     {"E", "", 164.81},  {"F", "", 174.61},  {"F", "#", 185.00},
     {"G", "", 196.00},  {"G", "#", 207.65}, {"A", "", 220.00},
     {"B", "b", 233.08}, {"B", "", 246.94},  {"C", "", 261.63},
     {"C", "#", 277.18}, {"D", "", 293.66},  {"E", "b", 311.13},
     {"E", "", 329.63}},
    "All"};

const Mode standardTuning = {9,
                             {
                                 {"E", "", 82.41},
                                 {"A", "", 110.00},
                                 {"D", "", 146.83},
                                 {"G", "", 196.00},
                                 {"B", "", 246.94},
                                 {"E", "", 329.63},
                                 {"E", "", 164.82}, // 2nd harmonic of low E
                                 {"A", "", 220.00}, // 2nd harmonic of A
                                 {"D", "", 293.66}, // 2nd harmonic of D
                             },
                             "Standard"};

const Mode halfStepTuning = {6,
                             {
                                 {"E", "b", 77.78},  // Low E → Eb
                                 {"A", "b", 103.83}, // A  → Ab
                                 {"D", "b", 138.59}, // D  → Db
                                 {"G", "b", 185.00}, // G  → Gb
                                 {"B", "b", 233.08}, // B  → Bb
                                 {"E", "b", 311.13}, // High E → Eb
                             },
                             "Half-Step Down"};

const Mode dropDTuning = {6,
                          {
                              {"D", "", 73.42},
                              {"A", "", 110.00},
                              {"D", "", 146.83},
                              {"G", "", 196.00},
                              {"B", "", 246.94},
                              {"E", "", 329.63},
                          },
                          "Drop D"};

const Mode openDTuning = {6,
                          {
                              {"D", "", 73.42},
                              {"A", "", 110.00},
                              {"D", "", 146.83},
                              {"F", "#", 185.00},
                              {"A", "", 220.00},
                              {"D", "", 293.66},
                          },
                          "Open D"};

/*
Mode indices:
0 - all
1 - standard
2 - half
3 - drop D
4 - openD
*/
const Mode allModes[5] = {allTuning, standardTuning, halfStepTuning,
                          dropDTuning, openDTuning};

Mode currentMode = standardTuning;

Mode getCurrentMode() { return currentMode; }

void setCurrentMode(int modeIndex) { currentMode = allModes[modeIndex]; }

float getFrequency(String note) {
  int numNotes = currentMode.numNotes;
  for (int i = 0; i < numNotes; i++) {
    Note string = currentMode.availableNotes[i];
    if (note == string.note) {
      return string.frequency;
    }
  }

  return -1;
}

float freqError = 0.1; // compare floats accurately
String getNote(float frequency) {
  int numNotes = currentMode.numNotes;
  for (int i = 0; i < numNotes; i++) {
    Note string = currentMode.availableNotes[i];
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

  int numNotes = currentMode.numNotes;
  for (int i = 0; i < numNotes; i++) {
    Note string = currentMode.availableNotes[i];
    float error = frequency - string.frequency;

    if (abs(error) <= minError && abs(error) < maxError) {
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