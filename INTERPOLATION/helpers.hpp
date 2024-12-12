#include <vector>
#include <stdexcept> 
#include <cmath>
#include <functional>
#include <random>
#include <set>


namespace Toolbox{
    namespace intw{ 
        std::vector<double> Casual_Vec(int n, double lb, double ub) {
            // if (n > (ub - lb)) {
            //     throw std::invalid_argument("Impossibile generare abbastanza numeri unici nel range specificato.");
            // }

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> dist(lb, ub);

            std::set<double> uniqueNumbers;
            while (uniqueNumbers.size() < static_cast<size_t>(n)) {
                double num = dist(gen);
                uniqueNumbers.insert(num);
            }

            std::vector<double> result(uniqueNumbers.begin(), uniqueNumbers.end());

            return result;
        }

        std::vector<double> fill_x_equid(int n, double lb, double ub){ // vettore di n punti EQUIDISTANTI fra lb e ub
            std::vector<double> x_equid;
            x_equid.resize(n);
            for (int i = 0; i < n; ++i) 
            {
                x_equid[i] = lb + (ub - lb) * i / (n - 1);
            }
            // std::cout << "Contenuto del vettore x_equid:" << std::endl;
            //     for (double value : x_equid) 
            //     {
            //         std::cout << value << " ";
            //     }
            //     std::cout << std::endl;

            return x_equid;

        }  

        std::vector<double> fill_x_Cheby(int n, double lb, double ub){; // vettore di n punti CHEBYSHEV fra lb e ub
            std::vector<double> x_Cheby;
            x_Cheby.resize(n);
            for (int i = 0; i < n; ++i)
            {
                x_Cheby[i] = 0.5 * (ub + lb) + 0.5 * (ub - lb) * cos(M_PI * (2 * i + 1) / (2 * n));  // da aggiustare M_PI
            }

            // std::cout << "Contenuto del vettore x_Cheby:" << std::endl;
            //     for (double value : x_Cheby) 
            //     {
            //         std::cout << value << " ";
            //     }
            //     std::cout << std::endl;
            return x_Cheby;
        }  
    }
}   
