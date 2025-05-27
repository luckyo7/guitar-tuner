void differenceFunc(float *x, int xLen, int maxLag, float *d);

void normalizeDifference(float *d, float *dPrime, int maxLag);

float detectPitch(float *signals, int signalLen, int fs, float threshold);
