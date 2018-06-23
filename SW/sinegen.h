#include <iostream>
#include <iomanip>
#include <limits>
#include <complex>

class c_sinewave
{
public:

  static const double two_pi;

private:

  const double amplitude;
  const unsigned int frequency;
public:

  c_sinewave(const double a = 1.0, const unsigned int f = 1) : amplitude(a), frequency(f)
  {
  }

  const double value(const double x)
  {
    if(x < 0 || x > 1)
    {
      return 0;
    }
    else
    {
      return amplitude * ::sin((two_pi * frequency) * x);
    }
  }

};

const double c_sinewave::two_pi = ::atan(1) * 8.0;

