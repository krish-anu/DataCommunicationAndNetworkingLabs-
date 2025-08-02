// general purpose standard C lib
#include <stdio.h>
#include <stdlib.h> // includes malloc(),free()
//Add any necessary headers here
#include <math.h>
#include <float.h>

// user-defined header files
#include "mypcm.h" // do not modify this file

// put your function prototypes for additional helper functions below:


// implementation
float analog_signal_generator(asignal signal, int t)
{
	//TODO
	return signal.A*sin(signal.omega*t+signal.sigma);
	
	
}
 void sampler(float *samples, int interval, asignal signal) {
    int idx = 0;
    for (int t = 0; t <= signal.duration; t += interval) {
        samples[idx++] = analog_signal_generator(signal, t);
    }
    samples[idx] = INFINITY;  // sentinel
}

// void quantizer1(float *samples, int *pcmpulses, int levels)
// {
// 	//TODO
// 	float min = FLT_MAX, max = -FLT_MAX;
//     int count = 0;

//     while (1) {
//         float val = samples[count];
//         if (val == FLT_MAX) break;  // use FLT_MAX as terminator if needed
//         if (val < min) min = val;
//         if (val > max) max = val;
//         count++;
//     }

//     float range = max - min;

//     for (int i = 0; i < count; i++) {
//         float normalized = (samples[i] - min) / range;
//         int level = (int)(normalized * levels);
//         if (level == levels) level = levels - 1;  // Cap the maximum
//         pcmpulses[i] = level;
//     }

// }
void quantizer(float *samples, int *pcmpulses, int levels, float A) {
    float Amin = -A;
    float Amax = A;
    float range = Amax - Amin;
    int i = 0;

    while (samples[i] != INFINITY) {
        float normalized = (samples[i] - Amin) / range;
        int level = (int)(normalized * levels);
        if (level >= levels) level = levels - 1;
        pcmpulses[i] = level;
        i++;
    }
    pcmpulses[i] = -1;  // sentinel
}

void encoder(int *pcmpulses, int *dsignal, int encoderbits) {
    int i = 0;
    while (pcmpulses[i] != -1) {
        int val = pcmpulses[i];
        for (int j = encoderbits - 1; j >= 0; j--) {
            dsignal[i * encoderbits + (encoderbits - 1 - j)] = (val >> j) & 1;
        }
        i++;
    }
}
