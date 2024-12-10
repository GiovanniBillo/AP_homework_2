#include <gtest/gtest.h>
#include "wrapper.hpp"

std::function<T(T)> func = x*x // funzione test
double t = 4.5; // punto di interpolazione
int n = 8; // number of point
double lb = 0; //lower bound
double ub = 8  // upper bound

std::vector<double> x_casual = {0, 1, 3, 3.5, 6, 5, 4.3}; // vettore di punti CASUALE 

std::vector<double> x_equid(int n, double lb, double ub){; // vettore di n punti EQUIDISTANTI fra lb e ub
        x_equid.resize(n);
        for (int i = 0; i < n; ++i) 
        {
            x_equid[i] = lb + (ub - lb) * i / (n - 1);
        }
}  

std::vector<double> x_Cheby(int n, double lb, double ub){; // vettore di n punti CHEBYSHEV fra lb e ub
       x_Cheby.resize(n);
        for (int i = 0; i < n; ++i)
        {
            x_Cheby[i] = 0.5 * (ub + lb) + 0.5 * (ub - lb) * cos(M_PI * (2 * i + 1) / (2 * n));  // da aggiustare M_PI
        } 
}   




TEST(InterpolationTests, Linear) {

std::vector<double> y = generateY(x_casual , func);

LinearInterpolator A;
A.build(x_casual , y);
double Result1 = A(t);

std::cout << "Valore della funzione "<< func << "in" << t << ":  " << func(t) <<" --- valore interpolato in "<< t << ":  " << Result1 <<  std::endl;

//TODO 
    EXPECT_EQ(1, // just a placeholder that always fails
    2);

    // Extra debugging: Triggered only if the test fails
    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";
    }
}


// TEST GROUP for Lagrange Interpolator
TEST(LagrangeInterpolatorTests, Casual) {

std::vector<double> y = generateY(x_casual, func);

LagrangeInterpolator B;
B.buildCasual(x_casual , y);
double Result2 = B(t);

std::cout << "Valore della funzione x^2 in " << t << ":  " << func(t) <<" --- valore interpolato in "<< t << ":  " << Result2 <<  std::endl;

// TODO 
EXPECT_EQ(1, 2);  // This will always fail

// Extra debugging: Triggered only if the test fails
if (HasFailure()) {
    std::cerr << "Test failed. Debugging information:\n";
    }
}

TEST(LagrangeInterpolatorTests, Equidistant) {

std::vector<double> y = generateY(x_equid(n,lb,ub) , func) // valori della funzione x^2

LagrangeInterpolator B;
B.buildEquidistant(y,n,lb,ub);
double Result3 = B(t);

std::cout << "Valore della funzione x^2 in " << t << ":  " << func(t) <<" --- valore interpolato in "<< t << ":  " << Result3 <<  std::endl;


    // TODO 
    EXPECT_EQ(1, 2);  // This will always fail

    // Extra debugging: Triggered only if the test fails
    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";
    }
}

TEST(LagrangeInterpolatorTests, Chebyshev) {

double y = generateY(x_Cheby(n,lb,ub) , func) // valori della funzione x^2

LagrangeInterpolator B;
B.buildChebyshev(y,n,lb,ub);
double Result4 = B(t);

std::cout << "Valore della funzione x^2 in " << t << ":  " << func(t) <<" --- valore interpolato in "<< t << ":  " << Result4 <<  std::endl;


    // TODO 
    EXPECT_EQ(1, 2);  // This will always fail

    // Extra debugging: Triggered only if the test fails
    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";
    }
}




// TEST GROUP for Spline Interpolator
TEST(SplineInterpolatorTests, Interpolation) {
    // TODO 
    EXPECT_EQ(1, 2);  // This will always fail

    // Extra debugging: Triggered only if the test fails
    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



