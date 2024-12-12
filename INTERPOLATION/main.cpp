#include <iostream>
#include "InterpolateWrapper.hpp"


using namespace Toolbox;

int main()
{
    Toolbox::intw::LinearInterpolator<double> A;

    // Example function to interpolate (e.g., f(x) = sin(x))
    auto func = [](double x) { return std::sin(x); };

    // Define bounds and maximum points
    double lowerBound = 0.0;
    double upperBound = M_PI; // π
    int maxPoints = 10;

    // Demonstrate plotError
    A.plotError(A, func, lowerBound, upperBound, maxPoints);

    return 0;
}
