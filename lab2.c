// general purpose standard C lib
#include <math.h>
#include <stdio.h>
#include <stdlib.h> // stdlib includes malloc() and free()

// user-defined header files
#include "mypcm.h"

// function prototypes
void run(asignal *inputsignal);

int main() {
  asignal *inputsignal = (asignal *)malloc(sizeof(asignal));
  run(inputsignal);

  // call any other function here

  free(inputsignal);
  return 0;
}

void run(asignal *inputsignal) {
  int interval;
  int encoderbits;

  scanf("%f %f %f %d %d %d", &inputsignal->A, &inputsignal->omega,
        &inputsignal->sigma, &inputsignal->duration, &interval, &encoderbits);

  int sampleCount = (inputsignal->duration / interval) + 1;
  int quantizationLevels = 1 << encoderbits;

  float *samples = malloc(sizeof(float) * (sampleCount + 1)); // +1 for sentinel
  int *pcmpulses = malloc(sizeof(int) * (sampleCount + 1));   // +1 for sentinel
  int *dsignal = malloc(sizeof(int) * sampleCount * encoderbits);

  sampler(samples, interval, *inputsignal);
  samples[sampleCount] = INFINITY;

  quantizer(samples, pcmpulses, quantizationLevels, inputsignal->A);
  pcmpulses[sampleCount] = -1;

  encoder(pcmpulses, dsignal, encoderbits);

  for (int i = 0; i < sampleCount * encoderbits; i++) {
    printf("%d", dsignal[i]);
  }
  printf("\n");

  free(samples);
  free(pcmpulses);
  free(dsignal);
}
