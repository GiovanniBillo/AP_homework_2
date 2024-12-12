#include <iostream>
#include "InterpolateWrapper.hpp"


using namespace Toolbox;

int main()
{
    // Producing some graphs for the behaviour and convergence of various interpolators
    Toolbox::intw::LinearInterpolator<double> A;

    // Example function to interpolate (e.g., f(x) = sin(x))
    auto func = [](double x) { return std::sin(x); };

    // Define bounds and maximum points
    double lowerBound = 0.0;
    double upperBound = M_PI; // π
    int maxPoints = 10;
    // Method name for the interpolation
    std::string methodName = "linearinterpolation";

    // Demonstrate plotError with the required methodName argument
    A.plotError(func, lowerBound, upperBound, maxPoints, Toolbox::intw::fill_x_equid, methodName);

    Toolbox::intw::LagrangeInterpolator<double> B;

    std::string methodName2 = "Lagrange";

    B.plotError(func, lowerBound, upperBound, maxPoints, Toolbox::intw::fill_x_Cheby, methodName2);

    Toolbox::intw::SplineInterpolator<double> C;

    std::string methodName3 = "Cubicspline";

    C.plotError(func, lowerBound, upperBound, maxPoints, Toolbox::intw::fill_x_equid, methodName3);




    return 0;
}
