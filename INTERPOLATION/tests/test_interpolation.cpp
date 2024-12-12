#include <gtest/gtest.h>
#include "InterpolateWrapper.hpp"
#include <math.h>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace Toolbox;
auto f = [](double i){
        return  1 / std::atan( 1 + (i*i) );
    }; // funzione test

double t = 2; // punto di interpolazione
int n = 8; // number of point
double lb = -4; //lower bound
double ub = 4;  // upper bound
double tolerance = 0.1;

std::vector<double> x_casual = Toolbox::intw::Casual_Vec(n, lb, ub);  //{-3, -2, -1, 1, 2, 3, 4}; vettore di punti CASUALE 

// TEST GROUP for Linear Interpolator
TEST(InterpolationTests, Linear) {

Toolbox::intw::LinearInterpolator<double> A;


std::vector<double> y = A.generateY(x_casual , f);

A.build(x_casual , y, y.size(), 2.0, 4.0);

double Result = A(t);
std::cout << "Valore della funzione in " << t << ":  " << f(t) <<" --- valore interpolato in "<< t << ":  " << Result << "---- ERRORE:" << std::abs(Result - f(t))<<  std::endl;


// for (size_t i = 1; i < 5; i++){
    
//     n = 2*i;

//     std::vector<double> x_rand = Casual_Vec(n, lb, ub); 
//     y = A.generateY(x_rand , f);

//     A.build(x_rand , y);

//     double Errore = A.Error(f,lb,ub);

//     std::cout << n << " LINEAR CASUAL --> " << Errore << std::endl;

// }

// for (size_t i = 1; i < 20; i++){

//     n = 2*i;

//     std::vector<double> x_equid = fill_x_equid(n, lb, ub);
//     y = A.generateY(x_equid , f);

//     A.build(x_equid , y);

//     double Errore = A.Error(f,lb,ub);

//     std::cout << n << " LINEAR EQUID --> " << Errore << std::endl;
    

// }

EXPECT_NEAR(f(t), Result, tolerance); 

}


// TEST GROUP for Lagrange Interpolator
TEST(LagrangeInterpolatorTests, Casual) {

Toolbox::intw::LagrangeInterpolator<double> A;

std::vector<double> y = A.generateY(x_casual , f);

A.buildCasual(x_casual , y, y.size(), 2.0, 4.0);

double Result = A(t);
std::cout << "Valore della funzione in " << t << ":  " << f(t) <<" --- valore interpolato in "<< t << ":  " << Result << "---- ERRORE:" << std::abs(Result - f(t)) << std::endl;


// for (size_t i = 1; i < 20; i++){

//     n = 2*i;

//     std::vector<double> x_rand = Casual_Vec(n, lb, ub);
//     y = A.generateY(x_rand , f);

//     A.buildCasual(x_rand , y, x_rand.size());
    
//     double Errore = A.Error(f,lb,ub);

//     std::cout << n << " POLI CASUAL --> " << Errore << std::endl;

// }
   
EXPECT_NEAR(f(t), Result, tolerance);  
}

TEST(LagrangeInterpolatorTests, Equidistant) {

Toolbox::intw::LagrangeInterpolator<double> A;

std::vector<double> y = A.generateY(Toolbox::intw::fill_x_equid(n, lb, ub) , f);
A.buildEquidistant(Toolbox::intw::fill_x_equid(n, lb, ub), y, y.size(), lb, ub);

double Result = A(t);
std::cout << "Valore della funzione in " << t << ":  " << f(t) <<" --- valore interpolato in "<< t << ":  " << Result << "---- ERRORE:" << std::abs(Result - f(t)) <<  std::endl;

//  for (size_t i = 1; i < 20; i++)
// {
//     n = 2*i;

//     std::vector<double> x_rand = fill_x_equid(n, lb, ub);
//     y = A.generateY(x_rand , f);

//     A.buildEquidistant(y, y.size(), lb, ub);

//     double Errore = A.Error(f,lb,ub);

//     std::cout << n << " POLI EQUIUD --> " << Errore << std::endl;

// }

EXPECT_NEAR(f(t), Result, tolerance);

}

TEST(LagrangeInterpolatorTests, Chebyshev) {

Toolbox::intw::LagrangeInterpolator<double> A;

std::vector<double> y = A.generateY(Toolbox::intw::fill_x_Cheby(n, lb, ub) , f);
A.buildChebyshev(Toolbox::intw::fill_x_equid(n, lb, ub), y, y.size(), lb, ub);

double Result = A(t);
std::cout << "Valore della funzione in " << t << ":  " << f(t) <<" --- valore interpolato in "<< t << ":  " << Result << "---- ERRORE:" << std::abs(Result - f(t)) << std::endl;


// for (size_t i = 1; i < 20; i++){

//     n = 2*i;

//     std::vector<double> x_rand = fill_x_Cheby(n, lb, ub);
//     y = A.generateY(x_rand , f);

//     A.buildChebyshev(y, y.size(), lb, ub);

//     double Errore = A.Error(f,lb,ub);

//     std::cout << n << " POLI CHEBY --> " << Errore << std::endl;

// }

EXPECT_NEAR(f(t),Result, tolerance); 

}

/*
// TEST GROUP for Spline Interpolator
TEST(SplineInterpolatorTests, Interpolation) {
    SplineInterpolator<double> A;
    std::vector<double> y = A.generateY(x_casual , f);

    A.build(x_casual, y);
    double Result = A(t);

    // TODO 
    EXPECT_NEAR(f(t), Result, tolerance);  // This will always fail

    // Extra debugging: Triggered only if the test fails
    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";
    }
}
*/

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
};
