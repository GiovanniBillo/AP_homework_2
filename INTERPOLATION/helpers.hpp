 #include <vector>
#include <stdexcept> 
#include <cmath>
#include <functional>
#include <random>
#include <set>
#include <fstream> 
#include <iostream> // Needed for std::cerr

namespace Toolbox {
    namespace intw {

    // Function to write a vector of pairs (n, Error) to a file
    void writeToFile(const std::vector<std::pair<int, double>>& data, const std::string& filename) {
        std::ofstream dataFile(filename);
        if (!dataFile) {
            std::cerr << "Error: Unable to create data file " << filename << ".\n";
            return;
        }
        for (const auto& pair : data) {
            dataFile << pair.first << " " << pair.second << "\n";
        }
        dataFile.close();
    }

    // Function to create a GNUplot script for visualizing Error vs Number of Points
    void createGnuplotScript(const std::string& dataFile, const std::string& outputFile, 
                             const std::string& scriptFile, const std::string& methodName) {
        std::ofstream script(scriptFile);
        if (!script) {
            std::cerr << "Error: Unable to create GNUplot script file " << scriptFile << ".\n";
            return;
        }
        script << "set terminal pngcairo size 800,600\n";
        script << "set output '" << outputFile << "'\n";
        script << "set xlabel 'Number of Points (n)'\n";
        script << "set ylabel 'Error'\n";
        script << "set title 'Error vs Number of Points (" << methodName << ")'\n";
        script << "plot '" << dataFile << "' using 1:2 with linespoints title 'Error'\n";
        script.close();
    }

    // Function to generate a vector of `n` random (casual) points between `lb` and `ub`
    std::vector<double> Casual_Vec(int n, double lb, double ub) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(lb, ub);

        std::set<double> uniqueNumbers;
        while (uniqueNumbers.size() < static_cast<size_t>(n)) {
            double num = dist(gen);
            uniqueNumbers.insert(num); // Ensure unique points
        }

        std::vector<double> result(uniqueNumbers.begin(), uniqueNumbers.end());
        return result;
    }

    // Function to generate a vector of `n` equidistant points between `lb` and `ub`
    std::vector<double> fill_x_equid(int n, double lb, double ub) {
        std::vector<double> x_equid(n);
        for (int i = 0; i < n; ++i) {
            x_equid[i] = lb + (ub - lb) * i / (n - 1); // Compute equidistant point
        }
        return x_equid;
    }

    // Function to generate a vector of `n` Chebyshev points between `lb` and `ub`
    std::vector<double> fill_x_Cheby(int n, double lb, double ub) {
        std::vector<double> x_Cheby(n);
        for (int i = 0; i < n; ++i) {
            x_Cheby[i] = 0.5 * (ub + lb) + 0.5 * (ub - lb) * cos(M_PI * (2 * i + 1) / (2 * n));
        }
        return x_Cheby;
    }

    } // namespace intw
} // namespace Toolbox
  
