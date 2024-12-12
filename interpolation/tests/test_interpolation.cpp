
#include <gtest/gtest.h>
#include "wrapper.hpp"
#include <math.h>
#include <random>
#include <algorithm>
#include <iomanip>


auto f = [](double i){
        return  1 / std::atan( 1 + (i*i) );
    }; // funzione test

double t = 2; // punto di interpolazione
int n = 8; // number of point
double lb = -4; //lower bound
double ub = 4;  // upper bound
double tolerance = 0.1;


// std::vector<double> Casual_Vec(int n, double lb, double ub) {
//     // if (n > (ub - lb)) {
//     //     throw std::invalid_argument("Impossibile generare abbastanza numeri unici nel range specificato.");
//     // }

//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_real_distribution<double> dist(lb, ub);

//     std::set<double> uniqueNumbers;
//     while (uniqueNumbers.size() < static_cast<size_t>(n)) {
//         double num = dist(gen);
//         uniqueNumbers.insert(num);
//     }

//     std::vector<double> result(uniqueNumbers.begin(), uniqueNumbers.end());

//     return result;
// }


// std::vector<double> fill_x_equid(int n, double lb, double ub){ // vettore di n punti EQUIDISTANTI fra lb e ub
//         std::vector<double> x_equid;
//         x_equid.resize(n);
//         for (int i = 0; i < n; ++i) 
//         {
//             x_equid[i] = lb + (ub - lb) * i / (n - 1);
//         }
// // std::cout << "Contenuto del vettore x_equid:" << std::endl;
// //     for (double value : x_equid) 
// //     {
// //         std::cout << value << " ";
// //     }
// //     std::cout << std::endl;

//         return x_equid;

// }  

// std::vector<double> fill_x_Cheby(int n, double lb, double ub){; // vettore di n punti CHEBYSHEV fra lb e ub
//        std::vector<double> x_Cheby;
//         x_Cheby.resize(n);
//         for (int i = 0; i < n; ++i)
//         {
//             x_Cheby[i] = 0.5 * (ub + lb) + 0.5 * (ub - lb) * cos(M_PI * (2 * i + 1) / (2 * n));  // da aggiustare M_PI
//         }
        
// // std::cout << "Contenuto del vettore x_Cheby:" << std::endl;
// //     for (double value : x_Cheby) 
// //     {
// //         std::cout << value << " ";
// //     }
// //     std::cout << std::endl;
//         return x_Cheby;
// }   

std::vector<double> x_casual = Casual_Vec(n, lb, ub);  //{-3, -2, -1, 1, 2, 3, 4}; vettore di punti CASUALE 

// TEST GROUP for Linear Interpolator
TEST(InterpolationTests, Linear) {

LinearInterpolator<double> A;


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

LagrangeInterpolator<double> A;

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

LagrangeInterpolator<double> A;

std::vector<double> y = A.generateY(fill_x_equid(n, lb, ub) , f);
A.buildEquidistant(fill_x_equid(n, lb, ub), y, y.size(), lb, ub);

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

LagrangeInterpolator<double> A;

std::vector<double> y = A.generateY(fill_x_Cheby(n, lb, ub) , f);
A.buildChebyshev(fill_x_equid(n, lb, ub), y, y.size(), lb, ub);

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




























