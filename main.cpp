
#include <iostream>
#include "interpolation.h"
#include "wrapper.hpp"

using namespace alglib;

int main()
{
  DataVector x = {0,1,2,3,5,7};
  DataVector y = {0,0,2,7,5,3.2};
  double t = 4.5;

   LinearInterpolator A;
   A.build(x,y);
   double Result = A(t);

   std::cout << "Valore interpolato in t="<< t << ":  " << Result << std::endl;

   LagrangeInterpolator B;
   
   B.buildEquidistant(y,6,0,8);
   double Result1 = B(t);
  std::cout << "Valore interpolato in t="<< t << " con Nodi Equidist:  " << Result1 << std::endl;

  /*
   B.buildChebyshev(y,y.size(),0,8);
   double Result2 = B(t);

   std::cout << "Valore interpolato in t="<< t << " con Nodi Equidist:  " << Result1 << std::endl;
   std::cout << "Valore interpolato in t="<< t << " con Nodi Cheby:  " << Result2 << std::endl;

 /*      
       
        // Here we demonstrate polynomial interpolation of y=x^2-x sampled at [0,1,2].
        // Barycentric representation of polynomial is used.
        
        

double a = 0;  // limite inferiore dell'intervallo
double b = 10;  // limite superiore dell'intervallo
ae_int_t n = 6;  // numero di punti

        barycentricinterpolant p_casuale;
        barycentricinterpolant p_eqdist;
        barycentricinterpolant p_cheby;

        // barycentric model is created
        polynomialbuild(x, y, p_casuale);
        polynomialbuildeqdist(a,b,y,3,p_eqdist);
        polynomialbuildcheb1(a,b,y,3,p_cheby);

        barycentriccalc(p_casuale,t);
        barycentriccalc(p_eqdist,t);
        barycentriccalc(p_cheby,t);





    double t = 5;  // Punto in cui vogliamo interpolare

    // Chiamata alla funzione di interpolazione
    double interpolatedValue_eqdist = polynomialcalceqdist(a, b, y, n, t, xparams());
    // Chiamata alla funzione di interpolazione
    double interpolatedValue_cheb = polynomialcalccheb1(a, b, y, n, t, xparams());

    // Visualizza il risultato
    std::cout << "Valore interpolato in t = " << t << ": " << interpolatedValue_eqdist << std::endl;
    std::cout << "Valore interpolato in t = " << t << ": " << interpolatedValue_cheb << std::endl;


  
  /* _1D::LinearInterpolator<double> interp;
    interp.setData(x_vals, y_vals);
    std::cout << "Linear interpolation at 1.5: " << interp(1.5) << std::endl;*/

return 0;
}