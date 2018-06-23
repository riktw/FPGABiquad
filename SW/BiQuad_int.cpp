#include "BiQuad_int.h"
#include <stdio.h>
#include <iostream>


BiQuad::BiQuad()
{
    bqfilter = new biquad;
}

BiQuad::~BiQuad()
{
    delete bqfilter;
}

int32_t BiQuad::BiQuadCalc(smp_type sample)
{
    double result;
    double temp1, temp2, temp3, temp4, temp5;

    /* compute result */

    temp1 = bqfilter->a0 * (double)sample;
    temp2 = bqfilter->a1 * (double)bqfilter->x1;
    temp3 = bqfilter->a2 * (double)bqfilter->x2;
    temp4 = bqfilter->a3 * (double)bqfilter->y1;
    temp5 = bqfilter->a4 * (double)bqfilter->y2;

  //  std::cout << temp1 << ", " << temp2 << ", " << temp3 << ", " << temp4 << ", " << temp5 << std::endl;

    result =  temp1 + temp2 + temp3 - temp4 - temp5;

    /* shift x1 to x2, sample to x1 */
    bqfilter->x2 = bqfilter->x1;
    bqfilter->x1 = sample;

    /* shift y1 to y2, result to y1 */
    bqfilter->y2 = bqfilter->y1;
    bqfilter->y1 = result;

    return (smp_type)result;
}

int32_t BiQuad::BiQuadCalcInt(smp_type sample)
{
    int64_t result;
    int64_t temp1, temp2, temp3, temp4, temp5;

    /* compute result */

    temp1 = ((int64_t)bqfilter->ai0 * (int64_t)sample)/(1024*1024);
    temp2 = ((int64_t)bqfilter->ai1 * (int64_t)bqfilter->x1)/(1024*1024);
    temp3 = ((int64_t)bqfilter->ai2 * (int64_t)bqfilter->x2)/(1024*1024);
    temp4 = ((int64_t)bqfilter->ai3 * (int64_t)bqfilter->y1)/(1024*1024);
    temp5 = ((int64_t)bqfilter->ai4 * (int64_t)bqfilter->y2)/(1024*1024);


    result =  temp1 + temp2 + temp3 - temp4 - temp5;

    /* shift x1 to x2, sample to x1 */
    bqfilter->x2 = bqfilter->x1;
    bqfilter->x1 = sample;

    /* shift y1 to y2, result to y1 */
    bqfilter->y2 = bqfilter->y1;
    bqfilter->y1 = result;

 //   std::cout << sample << ", " << bqfilter->x1 << ", " << bqfilter->x2 << ", " << bqfilter->y1 << ", " << bqfilter->y1 << ", "  <<  result << std::endl;


    return (smp_type)result;
}

void BiQuad::BiQuadClean(void)
{
    bqfilter->x1 = bqfilter->x2 = 0;
    bqfilter->y1 = bqfilter->y2 = 0;
}

int32_t BiQuad::ReturnvaluesInt(int value)
{
    switch(value)
    {
    case 1:
        return bqfilter->ai0;
        break;
    case 2:
        return bqfilter->ai1;
        break;
    case 3:
        return bqfilter->ai2;
        break;
    case 4:
        return bqfilter->ai3;
        break;
    case 5:
        return bqfilter->ai4;
        break;
    }
}


double BiQuad::Returnvalues(int value)
{
    switch(value)
    {
    case 1:
        return bqfilter->a0;
        break;
    case 2:
        return bqfilter->a1;
        break;
    case 3:
        return bqfilter->a2;
        break;
    case 4:
        return bqfilter->a3;
        break;
    case 5:
        return bqfilter->a4;
        break;
    }
}

void BiQuad::BiQuad_new(int type, smp_type dbGain, /* gain of filter */
                          smp_type freq,             /* center frequency */
                          smp_type srate,            /* sampling rate */
                          smp_type bandwidth)       /* bandwidth in octaves */
{
    double A, omega, sn, cs, alpha, beta;
    double a0, a1, a2, b0, b1, b2;

    /* setup variables */
    A = pow(10, (double)dbGain /40);
    omega = 2 * M_PI * freq /srate;
    sn = sin(omega);
    cs = cos(omega);
    alpha = sn * sinh(M_LN2 /2 * bandwidth * omega /sn);
    beta = sqrt(A + A);

    switch (type) {
    case LPF:
        b0 = (1 - cs) /2;
        b1 = 1 - cs;
        b2 = (1 - cs) /2;
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        break;
    case HPF:
        b0 = (1 + cs) /2;
        b1 = -(1 + cs);
        b2 = (1 + cs) /2;
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        break;
    case BPF:
        b0 = alpha;
        b1 = 0;
        b2 = -alpha;
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        break;
    case NOTCH:
        b0 = 1;
        b1 = -2 * cs;
        b2 = 1;
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        break;
    case PEQ:
        b0 = 1 + (alpha * A);
        b1 = -2 * cs;
        b2 = 1 - (alpha * A);
        a0 = 1 + (alpha /A);
        a1 = -2 * cs;
        a2 = 1 - (alpha /A);
        break;
    case LSH:
        b0 = A * ((A + 1) - (A - 1) * cs + beta * sn);
        b1 = 2 * A * ((A - 1) - (A + 1) * cs);
        b2 = A * ((A + 1) - (A - 1) * cs - beta * sn);
        a0 = (A + 1) + (A - 1) * cs + beta * sn;
        a1 = -2 * ((A - 1) + (A + 1) * cs);
        a2 = (A + 1) + (A - 1) * cs - beta * sn;
        break;
    case HSH:
        b0 = A * ((A + 1) + (A - 1) * cs + beta * sn);
        b1 = -2 * A * ((A - 1) + (A + 1) * cs);
        b2 = A * ((A + 1) + (A - 1) * cs - beta * sn);
        a0 = (A + 1) - (A - 1) * cs + beta * sn;
        a1 = 2 * ((A - 1) - (A + 1) * cs);
        a2 = (A + 1) - (A - 1) * cs - beta * sn;
        break;
    }

    /* precompute the coefficients */
    bqfilter->a0 = b0 /a0;
    bqfilter->a1 = b1 /a0;
    bqfilter->a2 = b2 /a0;
    bqfilter->a3 = a1 /a0;
    bqfilter->a4 = a2 /a0;

    bqfilter->ai0 = bqfilter->a0*1024*1024;
    bqfilter->ai1 = bqfilter->a1*1024*1024;
    bqfilter->ai2 = bqfilter->a2*1024*1024;
    bqfilter->ai3 = bqfilter->a3*1024*1024;
    bqfilter->ai4 = bqfilter->a4*1024*1024;

    /* zero initial samples */
    bqfilter->x1 = bqfilter->x2 = 0;
    bqfilter->y1 = bqfilter->y2 = 0;

}

