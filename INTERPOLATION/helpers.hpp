#include <vector>
#include <stdexcept> 
#include <cmath>
#include <functional>
#include <random>
#include <set>
#include <fstream> // For file I/O

namespace Toolbox{
    namespace intw{ 

    // Function to write the vector of pairs to a file
    void writeToFile(const std::vector<std::pair<int, double>>& data, const std::string& filename) {
        std::ofstream dataFile(filename);
        if (!dataFile) {
            std::cerr << "Error: Unable to create data file " << filename << ".\n";
            return;
        }
        for (const auto &pair : data) {
            dataFile << pair.first << " " << pair.second << "\n";
        }
        dataFile.close();
    }

    // Function to create a GNUplot script
    void createGnuplotScript(const std::string& dataFile, const std::string& outputFile, const std::string& scriptFile) {
        std::ofstream script(scriptFile);
        if (!script) {
            std::cerr << "Error: Unable to create GNUplot script file " << scriptFile << ".\n";
            return;
        }
        script << "set terminal pngcairo size 800,600\n";
        script << "set output '" << outputFile << "'\n";
        script << "set xlabel 'Number of Points (n)'\n";
        script << "set ylabel 'Error'\n";
        script << "set title 'Error vs Number of Points'\n";
        script << "plot '" << dataFile << "' using 1:2 with linespoints title 'Error'\n";
        script.close();
    }
        
std::vector<double> Casual_Vec(int n, double lb, double ub) { // vector of n CASUAL points between lb e ub

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
        
std::vector<double> fill_x_equid(int n, double lb, double ub){ // vector of n EQIDISTANT number between lb e ub
        
    std::vector<double> x_equid;
    x_equid.resize(n);
        for (int i = 0; i < n; ++i) 
        {
            x_equid[i] = lb + (ub - lb) * i / (n - 1);
        }

    return x_equid;

}  

std::vector<double> fill_x_Cheby(int n, double lb, double ub){; // vector of n CHEBYSHEV number between lb e ub
       
    std::vector<double> x_Cheby;
    x_Cheby.resize(n);
        for (int i = 0; i < n; ++i)
        {
            x_Cheby[i] = 0.5 * (ub + lb) + 0.5 * (ub - lb) * cos(M_PI * (2 * i + 1) / (2 * n));  
        }
                                                              
        return x_Cheby;
}   




    }
}   
