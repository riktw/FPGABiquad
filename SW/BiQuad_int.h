#ifndef BIQUAD_H
#define BIQUAD_H

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef M_LN2
#define M_LN2	   0.69314718055994530942
#endif

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

class BiQuad
{
public:
    /* whatever sample type you want */
    typedef int32_t smp_type;

    /* this holds the data required to update samples thru a filter */
    typedef struct {
        double a0, a1, a2, a3, a4;
        int32_t ai0, ai1, ai2, ai3, ai4;
        smp_type x1, x2, y1, y2;
    }
    biquad;

    /* filter types */
    enum {
        LPF, /* low pass filter */
        HPF, /* High pass filter */
        BPF, /* band pass filter */
        NOTCH, /* Notch Filter */
        PEQ, /* Peaking band EQ filter */
        LSH, /* Low shelf filter */
        HSH /* High shelf filter */
    };

    BiQuad();
    ~BiQuad();
    int32_t BiQuadCalc(smp_type sample);
    int32_t BiQuadCalcInt(smp_type sample);
    double Returnvalues(int value);
    int32_t ReturnvaluesInt(int value);
    void BiQuadClean(void);
    void BiQuad_new(int type, smp_type dbGain, /* gain of filter */
                              smp_type freq,             /* center frequency */
                              smp_type srate,            /* sampling rate */
                              smp_type bandwidth);       /* bandwidth in octaves */

private:
    biquad* bqfilter;

};

#endif // BIQUAD_H

