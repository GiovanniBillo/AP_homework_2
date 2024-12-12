#include <iostream>
#include "InterpolateWrapper.hpp"


using namespace Toolbox;

int main()
{
  /* std::vector<double> x = {0 + 1/8,1 + 1/8,2 + 1/8,3 + 1/8,4 + 1/8,5 + 1/8, 6 + 1/8, 7 + 1/8}; */
  /* std::vector<double> y = {0,0,2,7,5,3.2,6.5, 7.6}; */
  /* int lb = 0; */
  /* int ub = 8; */
  /* double t = 4.5; */

  /*  LinearInterpolator<double> A; */
  /*  A.build(x,y); */
  /*  double Result = A(t); */

  /*  std::cout << "Valore interpolato in t="<< t << ":  " << Result << std::endl; */

  /*  LagrangeInterpolator<double> B; */
   
  /*  B.buildEquidistant(y,y.size(),lb,ub); */
  /*  double Result1 = B(t); */
  /* std::cout << "Valore interpolato in t="<< t << " con Nodi Equidist:  " << Result1 << std::endl; */

  
  /*  B.buildChebyshev(y,y.size(),lb,ub); */
  /*  double Result2 = B(t); */

  /*  std::cout << "Valore interpolato in t="<< t << " con Nodi Cheby:  " << Result2 << std::endl; */

  /*  std::vector<double> y_vec = B.generateY(x, [](double x) { return x * x; }); */
  /*   // Print the vector using a normal for loop */
  /*   for (size_t i = 0; i < y_vec.size(); ++i) { */
  /*       std::cout << y_vec[i] << " ";  // Print each element followed by a space */
  /*   } */
  /*   std::cout << std::endl;  // Print a newline after the last element */

  /*   B.buildEquidistant(y_vec,y_vec.size(),lb,ub); */
  /*   double Result3 = B(t); */
  /*   std::cout << "Result: " << Result3 << "expected result: "<< t*t << std::endl; */ 
   

       
return 0;
}
