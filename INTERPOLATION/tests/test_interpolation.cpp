
#include <gtest/gtest.h>
#include "InterpolateWrapper.hpp"
#include <math.h>
#include <random>
#include <algorithm>
#include <iomanip>


auto f = [](double i){
        return  1 / std::atan( 1 + (i*i) );
    }; // funzione test

double t = 2; // interpolation point
int n = 8; // number of point
double lb = -4; // lower bound
double ub = 4;  // upper bound
double tolerance = 0.5;

std::vector<double> x_casual = Toolbox::intw::Casual_Vec(n, lb, ub);  // Generate random vector with n elements 
std::vector<double> x_equid = Toolbox::intw::fill_x_equid(n, lb, ub); // Generate random vector of n equidistant value
std::vector<double> x_Cheby = Toolbox::intw::fill_x_Cheby(n, lb, ub); // Generate random vector with n Chebyshev value

// TEST GROUP for Linear Interpolator
TEST(InterpolationTests, Linear) {

Toolbox::intw::LinearInterpolator<double> A;

std::vector<double> y = A.generateY(x_casual , f); // Vector of f( x_casual[i] )

A.build(x_casual , y, y.size(), lb, ub); 

double Result = A(t); // Linear polynomial on t 
std::cout << "Function value on " << t << ": " << f(t) <<" --- Interpolated value on "<< t << ": " << Result << " ---- ERROR ON "<< t << ": "<< std::abs(Result - f(t))<<  std::endl;

double Max_error = A.Error(f, lb, ub); // Approximate evaluation of the maximum error between the test function and the linear interpolating polynomial.
std::cout <<"Max Error: " << Max_error << std::endl;

EXPECT_NEAR(f(t), Result, tolerance); 

}


// TEST GROUP for Lagrange Interpolator
TEST(LagrangeInterpolatorTests, Casual) {

Toolbox::intw::LagrangeInterpolator<double> A;

std::vector<double> y = A.generateY(x_casual , f); // Vector of f( x_casual[i] )

A.buildCasual(x_casual , y, y.size(), lb, ub); // Generate interpolating polynomial with vector x of n casual number 

double Result = A(t); // Interpolating polynomial on t 
std::cout << "Function value on " << t << ":  " << f(t) <<" --- interpolated value on "<< t << ":  " << Result << " ---- ERROR ON: "<< t << ": " << std::abs(Result - f(t)) << std::endl;

double Max_error = A.Error(f, lb, ub); // Approximate evaluation of the maximum error between the test function and the interpolating polynomial.
std::cout <<"Max Error: " << Max_error << std::endl;


EXPECT_NEAR(f(t), Result, tolerance);  
}


TEST(LagrangeInterpolatorTests, Equidistant) {

    Toolbox::intw::LagrangeInterpolator<double> A;

    std::vector<double> y = A.generateY( x_equid , f );

    A.buildEquidistant(x_equid, y, y.size(), lb, ub); // Genrate interpolating polynomial with vector x of n equidistant points 

    double Result = A(t); // Interpolating polynomial on t 
    std::cout << "Function Vaule on " << t << ":  " << f(t) <<" ---- Interpolated value on "<< t << ":  " << Result << "---- ERRORE ON " << t << ": " << std::abs(Result - f(t)) <<  std::endl;

    double Max_error = A.Error(f, lb, ub); // Approximate evaluation of the maximum error between the test function and the interpolating polynomial.
    std::cout <<"Max Error: " << Max_error << std::endl;

    EXPECT_NEAR(f(t), Result, tolerance);

}

TEST(LagrangeInterpolatorTests, Chebyshev) {

    Toolbox::intw::LagrangeInterpolator<double> A;

    std::vector<double> y = A.generateY( x_Cheby , f);

    A.buildChebyshev(Toolbox::intw::fill_x_equid(n, lb, ub), y, y.size(), lb, ub); // Genrate interpolating polynomial with vector x of n Chebyshev points 

    double Result = A(t); // Interpolating polynomial on t 
    std::cout << "Function Vaule on " << t << ":  " << f(t) <<" ---- Interpolated value on "<< t << ":  " << Result << "---- ERRORE ON " << t << ": " << std::abs(Result - f(t)) <<  std::endl;

    double Max_error = A.Error(f, lb, ub); // Approximate evaluation of the maximum error between the test function and the interpolating polynomial.
    std::cout <<"Max Error: " << Max_error << std::endl;

}


// TEST GROUP for Spline Interpolator
TEST(SplineInterpolatorTests, Interpolation) {
    
    Toolbox::intw::SplineInterpolator<double> A;

    std::vector<double> y = A.generateY( x_equid , f);

    A.build(x_casual, y, n, lb, ub); //Generate cubic spline
    
    double Result = A(t); // Spine valueted on t 
    std::cout << "Function value on " << t << ":  " << f(t) <<" ---- Interpolated value on "<< t << ":  " << Result << "---- ERRORE ON " << t << ": " << std::abs(Result - f(t)) <<  std::endl;

    double Max_error = A.Error(f, lb, ub); // Approximate evaluation of the maximum error between the test function and the interpolating spline.
    std::cout <<"Max Error: " << Max_error << std::endl;
     
    EXPECT_NEAR(f(t), Result, tolerance);  // This will always fail

}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
};



