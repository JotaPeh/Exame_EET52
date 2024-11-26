/*
 * LowPassFilter.c
 *
 *  Created on: Sep 22, 2018
 *      Author: mmaximo
 */

#include "LowPassFilter.h"

void LowPassFilter_initialize(LowPassFilter *self, float bandwidth, float sampleTime) {
    self->bandwidth = bandwidth;
    self->sampleTime = sampleTime;
    self->b1 = (bandwidth * sampleTime - 2.0f) / (bandwidth * sampleTime + 2.0f);
    self->a0 = bandwidth * sampleTime / (bandwidth * sampleTime + 2.0f);
    self->a1 = bandwidth * sampleTime / (bandwidth * sampleTime + 2.0f);
    self->up = 0.0f;
    self->yp = 0.0f;
}

float LowPassFilter_filter(LowPassFilter *self, float input) {
    static float y;
    y = -self->b1 * self->yp + self->a0 * input + self->a1 * self->up;
    self->up = input;
    self->yp = y;
    return y;
}

float Elliptic_filter(Elliptic *self, float input) {
    static float y;

    y = 0;
    for(int i=1; i <=  self->len_yp; i++){
        y -= self->a[i] * self->yp[i-1];
    }

    y += self->b[0] * input;
    for(int j=1; j <=  self->len_up; j++){
        y += self->b[j] * self->up[j-1];
    }

    for(int j = self->len_up-1; j > 0; j--){
        self->up[j] = self->up[j-1];
    }
    self->up[0] = input;

    for(int i = self->len_yp-1; i > 0; i--){
       self->yp[i] = self->yp[i-1];
    }
    self->yp[0] = y;
    return y;
}

void Elliptic_initialize(Elliptic *self) {
    self->len_up = 6;
    self->len_yp = 6;
    self->b[0] = 0.0524;
    self->b[1] = -0.2680; /// previous output coefficient is -a1
    self->b[2] = 0.6099;
    self->b[3] = -0.7881;
    self->b[4] = 0.6099;
    self->b[5] = -0.2680;
    self->b[6] = 0.0524;
    self->a[0] = 1.0000; /// current input coefficient
    self->a[1] = -5.3037;
    self->a[2] = 11.9131;
    self->a[3] = -14.4834;
    self->a[4] = 10.0426;
    self->a[5] = -3.7634;
    self->a[6] = 0.5954;

    for(int i=0; i< self->len_up; i++){
        self->up[i] = 0.0f; /// previous input
    }
    for(int i=0; i< self->len_yp; i++){
        self->yp[i] = 0.0f; /// previous output
    }
}

float EllipticAg_filter(EllipticAg *self, float input) {
    static float y;
    
    y = 0;
    for(int i=1; i<=  self->len_yp; i++){
        y -= self->a[i] * self->yp[i-1];
    }

    y += self->b[0] * input;
    for(int j=1; j<=  self->len_up; j++){
        y += self->b[j] * self->up[j-1];
    }

    for(int j= self->len_up-1; j > 0; j--){
        self->up[j] = self->up[j-1];
    }
    self->up[0] = input;

    for(int i= self->len_yp-1; i > 0; i--){
       self->yp[i] = self->yp[i-1];
    }
    self->yp[0] = y;
    return y;
}

void EllipticAg_initialize(EllipticAg *self) {
    self->len_up = 7;
    self->len_yp = 7;
    self->b[0] = 0.0856;
    self->b[1] = -0.1237; /// previous output coefficient is -a1
    self->b[2] = 0.2072;
    self->b[3] = -0.0381;
    self->b[4] = -0.0381;
    self->b[5] = 0.2072;
    self->b[6] = -0.1237;
    self->b[7] = 0.0856;
    self->a[0] = 1.0000; /// current input coefficient
    self->a[1] = -3.4902;
    self->a[2] = 6.8197;
    self->a[3] = -8.2474;
    self->a[4] = 6.8023;
    self->a[5] = -3.6701;
    self->a[6] = 1.2386;
    self->a[7] = -0.1908;
    
    for(int i=0; i< self->len_up; i++){
        self->up[i] = 0.0f; /// previous input
    }
    for(int i=0; i< self->len_yp; i++){
        self->yp[i] = 0.0f; /// previous output
    }
}

float MovingAverage_filter(MovingAverage *self, float input) {
    static float y;
    
    y = 0;
    // for(int i=1; i<  self->len_up; i++){
    //     y -= self->a[i] * self->yp[i-1];
    // }

    y += self->b[0] * input;
    for(int j=1; j<=  self->len_up; j++){
        y += self->b[j] * self->up[j-1];
    }

    for(int j= self->len_up-1; j > 0; j--){
        self->up[j] = self->up[j-1];
    }
    self->up[0] = input;

    return y;
}

void MovingAverage_initialize(MovingAverage *self) {
    self->len_up = 9;

    for(int i=0; i<= self->len_up; i++){
        self->b[i] = 1/(self->len_up+1);
    }
    
    for(int i=0; i< self->len_up; i++){
        self->up[i] = 0.0f; /// previous input
    }
}