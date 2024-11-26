/*
 * LowPassFilter.h
 *
 *  Created on: Sep 22, 2018
 *      Author: mmaximo
 */

#ifndef LOWPASSFILTER_H_
#define LOWPASSFILTER_H_

/**
 * Defines a digital first order low pass filter of the form:
 * G(s) = a / (s + a)
 * where a is the filter's bandwidth. For discrete time implementation,
 * the Tustin's method is used. Therefore, the implementation uses
 * the following finite differences equation:
 * y[k] = -b1 * y[k-1] + a0 * u[k] + a1 * u[k-1]
 * where u[k] and y[k] denotes input and output at timestep k, respectively.
 */

#define ENABLE_LOW_PASS_FILTER 1

struct LowPassFilter {
    float bandwidth; /// filter's bandwidth
    float sampleTime; /// sample time
    float b1; /// previous output coefficient is -b1
    float a0; /// current input coefficient
    float a1; /// previous input coefficient
    float up; /// previous input
    float yp; /// previous output
};

struct Elliptic {
    float b[7]; /// previous output coefficient is -b1
    float a[7]; /// current input coefficient
    int len_up;
    int len_yp;
    float up[6]; /// previous input
    float yp[6]; /// previous output
};

struct EllipticAg {
    float b[8]; /// previous output coefficient is -b1
    float a[8]; /// current input coefficient
    int len_up;
    int len_yp;
    float up[7]; /// previous input
    float yp[7]; /// previous output
};

struct MovingAverage {
    float b[10]; /// previous output coefficient is -b1
    int len_up;
    float up[9]; /// previous input
};

typedef struct MovingAverage MovingAverage;

typedef struct EllipticAg EllipticAg;

typedef struct Elliptic Elliptic;

typedef struct LowPassFilter LowPassFilter;

/**
 * Initializes a first order low pass filter.
 * @param self pointer to object
 * @param bandwidth filter's bandwidth
 * @param sampleTime sample time
 */

void Elliptic_initialize(Elliptic *self);

float Elliptic_filter(Elliptic *self, float input);

void EllipticAg_initialize(EllipticAg *self);

float EllipticAg_filter(EllipticAg *self, float input);

void MovingAverage_initialize(MovingAverage *self);

float MovingAverage_filter(MovingAverage *self, float input);

void LowPassFilter_initialize(LowPassFilter *self, float bandwidth, float sampleTime);

/**
 * Updates the low pass filter.
 * @param self pointer to object
 * @param input input to filter
 * @return output from filter
 */
float LowPassFilter_filter(LowPassFilter *self, float input);

#endif /* LOWPASSFILTER_H_ */
