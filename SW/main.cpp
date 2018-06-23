#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "matplotlibcpp.h"
#include "BiQuad_int.h"
#include "sinegen.h"

namespace plt = matplotlibcpp;
using namespace std;

BiQuad filter;

void showsine(int type, int freq, int periods);

int main(int argc, char* argv[])
{
    // Check the number of parameters
    if (argc != 8) {
        // Tell the user how to run the program
        std::cerr << "Usage: " << argv[0] << " order(lpf/hpf/bps/notch/peak/hsh/lsh),  filter freq in Hz, order, gain, type(sine/triangle/square), test freq in Hz, test periods" << std::endl;
        /* "Usage messages" are a conventional way of telling the user
        * how to run a program if they enter the command incorrectly.
        */
        return 1;
    }

    int filtertype;
    if(strncmp("lpf", argv[1], 3) == 0)
    {
        filtertype = 0;
    }
    else if(strncmp("hpf", argv[1], 3) == 0)
    {
        filtertype = 1;
    }
    else if(strncmp("bpf", argv[1], 3) == 0)
    {
        filtertype = 2;
    }
    else if(strncmp("notch", argv[1], 5) == 0)
    {
        filtertype = 3;
    }
    else if(strncmp("peak", argv[1], 5) == 0)
    {
        filtertype = 4;
    }
    else if(strncmp("hsh", argv[1], 5) == 0)
    {
        filtertype = 5;
    }
    else if(strncmp("lsh", argv[1], 5) == 0)
    {
        filtertype = 6;
    }
    else
    {
        cout << "incorrect filter, use lpf, hpf, bpf, notch, peak, hsh or lsh" << endl;
        return 1;
    }

    int filterfreq;
    try {
        filterfreq = std::stoi(argv[2]);
    } catch (std::exception const &e) {
        cout << "filter frequency must be a number" << endl;
        return 1;
    }

    int filterorder;
    try {
        filterorder = std::stoi(argv[3]);
    } catch (std::exception const &e) {
        cout << "filter order must be a number" << endl;
        return 1;
    }

    int filtergain;
    try {
        filtergain = std::stoi(argv[4]);
    } catch (std::exception const &e) {
        cout << "filter gain must be a number" << endl;
        return 1;
    }


    int testtype;
    if(strncmp("sine", argv[5], 3) == 0)
    {
        testtype = 0;
    }
    else if(strncmp("triangle", argv[5], 3) == 0)
    {
        testtype = 1;
    }
    else if(strncmp("square", argv[5], 3) == 0)
    {
        testtype = 2;
    }
    else
    {
        cout << "incorrect type, use sine, triangle or square" << endl;
        return 1;
    }

    int testfreq;
    try {
        testfreq = std::stoi(argv[6]);
    } catch (std::exception const &e) {
        cout << "test frequency must be a number" << endl;
        return 1;
    }

    int testperiods;
    try {
        testperiods = std::stoi(argv[7]);
    } catch (std::exception const &e) {
        cout << "test periods must be a number" << endl;
        return 1;
    }

    double c1, c2, c3, c4, c5 = 0;
    int32_t ci1, ci2, ci3, ci4, ci5 = 0;
    filter.BiQuad_new(filtertype, filtergain, filterfreq, 192000, filterorder);

    c1 = filter.Returnvalues(1);
    c2 = filter.Returnvalues(2);
    c3 = filter.Returnvalues(3);
    c4 = filter.Returnvalues(4);
    c5 = filter.Returnvalues(5);
    ci1 = filter.ReturnvaluesInt(1);
    ci2 = filter.ReturnvaluesInt(2);
    ci3 = filter.ReturnvaluesInt(3);
    ci4 = filter.ReturnvaluesInt(4);
    ci5 = filter.ReturnvaluesInt(5);

    cout << std::hex << ci1 << ", " << ci2 << ", " << ci3 << ", " << ci4 << ", " << ci5 << endl;

    vector<double> x(192000+1), y(192000+1); // initialize with entries 0..100
    long double w;
    long double numerator;
    long double denominator;
    long double magnitude;
    long double SR = 192000;


    //Calculate magnitude for 0-24000Hzs
    for (int i = 0; i < 96000; ++ i)
    {
        x[i] = i;
        w = 2.0L*M_PI*i / SR;
        numerator = (c1*c1 + c2*c2 + c3*c3 + 2.0L*(c1*c2 + c2*c3)*cosl(w) + 2.0L*c1*c3*cosl(2.0L*w));
        denominator = (1.0L + c4*c4 + c5*c5 + 2.0L*(c4 + c4*c5)*cosl(w) + 2.0L*c5*cosl(2.0L*w));
        magnitude = sqrtl(numerator / denominator);
        y[i] = magnitude + 0.01;  //minimum of 0.01 for better graph
    }

    plt::figure_size(1200, 780);
    plt::subplot(2, 1, 1);
    plt::loglog(x, y);
    plt::xlim(0, 100000);
    plt::ylim(0.01, 10.0);
    plt::legend();
    plt::grid(true);
//    plt::show();


    showsine(testtype, testfreq, testperiods);
    return 0;
}


void showsine(int type, int freq, int periods)
{
    int n_point = 192000/(freq/periods);
    int p_point = n_point/periods;

    vector<double> x(n_point+1), y1(n_point+1), y2(n_point+1); // initialize with entries 0..100
    c_sinewave sinewave(100000.0, periods);

    filter.BiQuadClean();

    if(type == 0)           //Generate sine
    {
        for(int i = 0; i < n_point + 1; i++)
        {
            const double x = static_cast<double>(i) / n_point;
            y1[i] = 1*sinewave.value(x);
        }
    }
    else if(type == 1)      //Generate triangle
    {
        double y;
        for(int n = 0; n < periods; n++)
        {
            for(int i = 0; i < n_point/periods; i++)
            {

                if(i<(p_point/2))
                {
                    y = (double)i/(p_point/2);
                }
                else
                {
                    y = (double)((p_point/2)+((p_point/2)-i))/(p_point/2);
                }
                y = ((y*2)-1);
                y = y * 100000;
                y1[(n*p_point) + i] = y;
            }
        }
    }

    else if(type == 2)      //Generate square
    {
        double y;
        for(int n = 0; n < periods; n++)
        {
            for(int i = 0; i < n_point/periods; i++)
            {

                if(i<(p_point/2))
                {
                    y = -100000;
                }
                else
                {
                    y = 100000;
                }
                y1[(n*p_point) + i] = y;
            }
        }
    }
    //Apply filter over generated waveform and generate x axis.
    for(int i = 0; i < n_point + 1; i++)
    {
        x[i] = i;
        double y = 0;
        y = filter.BiQuadCalcInt(y1[i]);
        y2[i] = y;
    }

    plt::subplot(2, 1, 2);

    plt::plot(y1);
    plt::plot(y2);
    plt::xlim(0, n_point);
    plt::ylim(-120000, +120000);
    plt::legend();
    plt::grid(true);
    plt::show();
}
