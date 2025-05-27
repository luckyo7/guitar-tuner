#include <Arduino.h>
#include <arduinoFFT.h>
#include <driver/i2s.h>
#include <i2s.h>
#include <note.h>
#include <screen.h>

#define I2S_WS 41
#define I2S_SD 42
#define I2S_SCK 40

#define SAMPLE_RATE 16000
#define SAMPLES 1024 // Must be a power of 2

double vReal[SAMPLES];
double vImag[SAMPLES];

// moving average
const int movSize = 5; // size of the moving average window
float previousFrequencies[movSize] = {0.0, 0.0, 0.0, 0.0, 0.0};
float maxError = 2; // maximum difference allowed between moving average mean
                    // and sample frequency
// moving average

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLE_RATE);

void i2s_install() {
  // Set up I2S Processor configuration
  const i2s_config_t i2s_config = {
      .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = SAMPLE_RATE,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
      .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
      .intr_alloc_flags = 0,
      .dma_buf_count = 8,
      .dma_buf_len = SAMPLES,
      .use_apll = true,
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

bool checkMovingAverage(float currentFrequency, int movSize,
                        float previousFrequencies[], float maxError) {

  float mean = 0.0;
  for (int i = 0; i < movSize; i++) {
    mean += previousFrequencies[i];
  }

  mean /= movSize;

  return abs(currentFrequency - mean) <= maxError;
}

void appendFrequency(float currentFrequency, float previousFrequencies[],
                     int movSize) {
  for (int i = movSize - 1; i > 0; i--) {
    previousFrequencies[i - 1] = previousFrequencies[i];
  }
  previousFrequencies[movSize - 1] = currentFrequency;
}

const float peakMeanRatio = 4000;
void i2s_loop() {
  int32_t samples_read;
  size_t bytes_read;

  float mean = 0;
  for (int i = 0; i < SAMPLES; i++) {
    int32_t sample = 0;
    i2s_read(I2S_NUM_0, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);
    // sample >>= 14; // Scale down 32-bit to 18-bit or so
    vReal[i] = (double)sample;
    vImag[i] = 0.0;

    // mean += vReal[i];
  }

  // mean /= SAMPLES;

  // FFT magic
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();

  double peak = 0;
  int peakIndex = 0;
  for (int i = 1; i < (SAMPLES); i++) {
    if (vReal[i] > peak) {
      peak = vReal[i];
      peakIndex = i;
      // Serial.println(i);
    }
  }

  // Serial.println(peak / mean);
  // if (abs(peak * peakMeanRatio) <= abs(mean)) {
  //   // the peak signal isn't strong enough
  //   Serial.println("weak signal");
  //   return;
  // }

  // parabolic interpolation
  int k = peakIndex;
  double delta = 0.5 * (vReal[k - 1] - vReal[k + 1]) /
                 (vReal[k - 1] - 2 * vReal[k] + vReal[k + 1]);

  double interpolated_bin = k + delta;
  double frequency = (interpolated_bin * SAMPLE_RATE) / SAMPLES;

  // double minFundamental = frequency;

  // double subharmonicThreshold = 0.3;
  // for (int divisor = 2; divisor <= 3; divisor++) {
  //   int subharmonicIndex = round(peakIndex / divisor);

  //   double subDelta =
  //       0.5 * (vReal[subharmonicIndex - 1] - vReal[subharmonicIndex + 1]) /
  //       (vReal[subharmonicIndex - 1] - 2 * vReal[subharmonicIndex] +
  //        vReal[subharmonicIndex + 1]);

  //   if (subharmonicIndex >= 1 &&
  //       vReal[subharmonicIndex] > subharmonicThreshold * vReal[peakIndex]) {
  //     minFundamental = ((subharmonicIndex + subDelta) * SAMPLE_RATE) /
  //     SAMPLES;
  //   }
  // }

  // double frequency = (peakIndex * 1.0 * SAMPLE_RATE) / SAMPLES;
  Serial.print("Peak frequency: ");
  Serial.print(frequency);
  Serial.println(" Hz");

  bool withinMovAvg =
      checkMovingAverage(frequency, movSize, previousFrequencies, maxError);

  appendFrequency(frequency, previousFrequencies, movSize);

  if (withinMovAvg) {
    NoteAndError nearestNote = findNearestNote(frequency);
    Note note = nearestNote.note;
    float errorFrac = nearestNote.error / 2.0;

    Serial.println(note.note);

    display.clearDisplay();
    drawNote(note.note, "");
    drawPitch(errorFrac);
    delay(500);
  } else {
    display.clearDisplay();
    drawPitch(0.0);
  }
  delay(100);
}

void i2s_setpin() {
  // Set I2S pin configuration
  const i2s_pin_config_t pin_config = {.bck_io_num = I2S_SCK,
                                       .ws_io_num = I2S_WS,
                                       .data_out_num = -1,
                                       .data_in_num = I2S_SD};

  i2s_set_pin(I2S_PORT, &pin_config);
}

/*
main.cpp
setup:
call i2s_install(), i2s_setpin(), i2s_start(I2S_PORT)
*/