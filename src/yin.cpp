#include <Arduino.h>
// implements YIN algorithm for pitch detection

void differenceFunc(float *x, int xLen, int maxLag, float *d) {
  // return the differences for lag (tau) in [1, maxLag]
  // lag is an integer index shift

  for (int lag = 1; lag <= maxLag; lag++) {
    // d(tau) = sum((samples(j) - samples(j+tau))^2)
    for (int j = 0; j < xLen - maxLag; j++) {
      float diff = x[j] - x[j + lag];
      d[lag] += diff * diff;
    }
  }
};

// cumulative mean normalized difference function
void normalizeDifference(float *d, float *dPrime, int maxLag) {
  for (int lag = 1; lag <= maxLag; lag++) {
    float dSum = 0;
    for (int j = 1; j <= lag; j++) {
      dSum += d[j];
    }
    dPrime[lag] = lag * d[lag] / dSum;
  }
}

// TODO: add interpolation
float detectPitch(float *signals, int signalLen, int fs, float threshold) {
  int maxLag = fs / 50; // 50 Hz lower bound

  float d[maxLag] = {0};
  differenceFunc(signals, signalLen, maxLag, d);

  float dPrime[maxLag] = {0};
  normalizeDifference(d, dPrime, maxLag);

  float pitch = -1;
  float firstPitch = -1;
  float lastPitch = -1;
  bool flag = false;
  float prevError = 0;
  float outLag; // final lag
  for (int lag = 2; lag < maxLag; lag++) {
    if (dPrime[lag] < threshold && dPrime[lag] < prevError) {
      if (!flag) {
        firstPitch = fs / lag;
        outLag = lag;
        flag = true;
      }
      lastPitch = fs / lag;
      // break;
    } else if (flag) {
      break;
    }

    prevError = dPrime[lag];
  }
  // pitch = 0.5 * (firstPitch + lastPitch);
  pitch = fs / interpolateLag(dPrime, maxLag, outLag);

  return pitch;
}

float interpolateLag(float *dPrime, int dLen, int lag) {
  // lag is the index where CNMDF (dPrime) is minimized
  float a = dPrime[lag - 1];
  float b = dPrime[lag];
  float c = dPrime[lag + 1];
  float denominator = (2 * (a - 2 * b + c));

  if (abs(denominator) < 0.1) {
    return lag;
  }

  return lag + (a - c) / denominator;
}