#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#include <vector>
#include <stdexcept>
#include <cmath>
#include <functional>
#include <utility>
#include "interpolation.h"
#include "helpers.hpp"  

namespace Toolbox{
    namespace intw{ 
        template <typename T>
            class Interpolator {

                public:
                    virtual ~Interpolator() = default;

                    // Funzione principale di interpolazione
                    virtual T operator()(T t) const = 0;

                    // Function to generate y-values based on a function pointer (or lambda)
                    std::vector<T> generateY(const std::vector<T>& x, std::function<T(T)> func) {
                        std::vector<T> y;
                        for (size_t i = 0; i < x.size(); ++i) {
                            y.push_back(func(x[i]));  // Apply the function (lambda or regular function)
                        }
                        return y;
                    }

                    //template <typename f>
                    virtual double Error(std::function<double(double)> f, double a, double b) = 0;
                    virtual void build(const std::vector<T> x,const std::vector<T> y, int n, double a, double b) = 0;
                    virtual void plotError(Interpolator<T> & ip, std::function<T(T)> f, double a, double b, int k) = 0;

            };


        template <typename T>
            class LinearInterpolator : public Interpolator<T> {

                private:
                    std::vector<T> x_, y_;
                    std::vector<std::pair<int, double>> n_E;

                public:
                    LinearInterpolator() = default;

                    // Costruzione dell'interpolante lineare
                    void build(const std::vector<T> x,const std::vector<T> y, int n, double a, double b) {
                        if (x.size() != y.size()) {
                            throw std::invalid_argument("x and y vectors must have the same size.");
                        }   
                        /* TODO: check if T is numeric */
                        x_ = x;
                        y_ = y;
                    }

                    double Error(std::function<double(double)> f, double a, double b) override {

                        std::vector<double> t; 
                        t.resize(100);
                        std::vector<double> y_polinomio; 
                        y_polinomio.resize(100);
                        std::vector<double> Errore;
                        Errore.resize(100);

                        std::vector<double> Y = this -> generateY (x_ , f);

                        for (int i = 0; i < 100; ++i){

                            t[0] = a;
                            t[i+1] = (b - a)/100 + t[i] ;  

                            for (size_t j = 1; j < x_.size(); ++j){
                                if ( t[i] <= x_[j]){

                                    T x0 = x_[j - 1]; 
                                    T x1 = x_[j];
                                    T y0 = Y[j-1];
                                    T y1 = Y[j];

                                    y_polinomio[i] = y0 + (y1 - y0) * (t[i] - x0) / (x1 - x0);
                                    Errore[i]= std::abs( f(t[i]) - y_polinomio[i] );
                                    // std::cout << Errore[i] << std::endl;
                                }

                                // return y_.back();

                            } 
                            auto max_iter = std::max_element(Errore.begin(), Errore.end());
                            double max = *max_iter;
                            return max;
                        } 


                    };

                    // Override dell'operatore () per interpolare in un dato punto t
                    T operator()(T t) const override {
                        // Interpolazione lineare tra due punti vicini
                        for (size_t i = 1; i < x_.size(); ++i) {
                            if (t <= x_[i]) 
                            {
                                T x0 = x_[i - 1]; 
                                T x1 = x_[i];
                                T y0 = y_[i - 1];
                                T y1 = y_[i];
                                return y0 + (y1 - y0) * (t - x0) / (x1 - x0);
                            }
                        }
                        return y_.back();  // Se t è fuori intervallo, ritorna l'ultimo valore
                    }

                    void plotError(Interpolator<T> & A,  std::function<T(T)> f, double lb, double ub, int max_points) override {

                        for (size_t i = 1; i < max_points; i++){

                            double n = 2*i;

                            std::vector<double> x_equid = fill_x_equid(n, lb, ub);
                            std::vector<double> y = A.generateY(x_equid , f);

                            A.build(x_equid , y, y.size(), 2.0, 4.0);

                            double Errore = A.Error(f,lb,ub);
                            n_E.emplace_back(n, Errore);


                            // std::cout << n << " LINEAR EQUID --> " << Errore << std::endl;
                                // File names
                        std::string dataFile = "error_data.dat";
                        std::string outputFile = "error_plot.png";
                        std::string scriptFile = "plot_script.gnuplot";

                        // Write data to a file
                        writeToFile(n_E, dataFile);

                        // Create GNUplot script
                        createGnuplotScript(dataFile, outputFile, scriptFile);

                        // Call GNUplot to generate the plot
                        int result = std::system(("gnuplot " + scriptFile).c_str());
                        if (result != 0) {
                            std::cerr << "Error: Failed to execute GNUplot.\n";
                        } else {
                            std::cout << "Plot successfully created: " << outputFile << "\n";
                        }

                        }
                    }

            };

        template <typename T>
            class LagrangeInterpolator : public Interpolator<T> {

                private:
                    std::vector<T> x_;
                    alglib::barycentricinterpolant p_;  // Interpolante Lagrange in forma baricentrica
                    std::vector<std::pair<int, double>> n_E;


                public:
                    LagrangeInterpolator() = default;

                    void build(const std::vector<T> x,const std::vector<T> y, int n, double a, double b) override {;}

                    // Metodo per costruire il modello di Lagrange con griglia casuale
                    void buildCasual(const std::vector<T> x,const std::vector<T> y, int n, double a, double b){

                        if (x.size() != y.size()) {
                            throw std::invalid_argument("x and y vectors ust have the same size.");
                        };

                        const double * idx_x;
                        idx_x = x.data(); 
                        const double * idx_y;
                        idx_y = y.data(); 


                        alglib::real_1d_array x_ , y_;
                        x_.setcontent(n, idx_x);
                        y_.setcontent(n, idx_y);
                        /* for (size_t i = 0; i < y.size(); ++i) { */
                        /*     y_[i] = y[i]; */
                        /*     x_[i] = x[i]; */
                        /* } */

                        // Costruzione dell'interpolante 
                        alglib::polynomialbuild(x_, y_, n, p_);
                    }
                    // Metodo per costruire il modello di Lagrange con griglia equidistante
                    void buildEquidistant(const std::vector<T> x,const std::vector<T> y, int n, double a, double b) {
                        if (y.size() != static_cast<size_t>(n)) {
                            throw std::invalid_argument("y vector size must be equal to n.");
                        }

                        /*    // Generazione dei punti x equidistanti
                              x_.resize(n);
                              for (int i = 0; i < n; ++i) {
                              x_[i] = a + (b - a) * i / (n - 1);
                              }                                            */

                        alglib::real_1d_array y_;
                        const double * idx; 
                        idx = y.data(); 
                        y_.setcontent(n, idx);
                        /* for (size_t i = 0; i < y.size(); ++i) { */
                        /*     y_[i] = y[i]; */
                        /* } */
                        // Costruzione dell'interpolante 
                        alglib::polynomialbuildeqdist(a, b, y_, n, p_);
                    }
                    // Metodo per costruire il modello di Lagrange con griglia di Chebyshev
                    void buildChebyshev(const std::vector<T> x,const std::vector<T> y, int n, double a, double b) {
                        if (y.size() != static_cast<size_t>(n)) {
                            throw std::invalid_argument("f vector size must be equal to n.");
                        }

                        /*   // Generazione dei punti x (Chebyshev)
                             x_.resize(n);
                             for (int i = 0; i < n; ++i) {
                             x_[i] = 0.5 * (b + a) + 0.5 * (b - a) * cos(M_PI * (2 * i + 1) / (2 * n));
                             }                                                                               */

                        alglib::real_1d_array y_;
                        const double * idx; 
                        idx = y.data(); 
                        y_.setcontent(n, idx);

                        /* for (size_t i = 0; i < y.size(); ++i) { */
                        /*     y_[i] = y[i]; */
                        /* } */

                        // Costruzione dell'interpolante
                        alglib::polynomialbuildcheb1(a, b, y_, n, p_);
                    }

                    double Error(std::function<double(double)> f, double a, double b) override {
                        std::vector<double> t; 
                        t.resize(100);
                        std::vector<double> Errore;
                        Errore.resize(100);

                        for (int i = 0; i < 100; ++i)
                        {
                            t[0] = a;
                            t[i+1] = (b - a)/100 + t[i] ;  // da aggiustare M_PI
                            Errore[i]= std::abs( f(t[i]) - alglib::barycentriccalc(p_, t[i]) );
                            //std::cout << Errore[i] << std::endl;

                        } 
                        auto max_iter = std::max_element(Errore.begin(), Errore.end());
                        double max = *max_iter;
                        return max;
                    } 

                    void plotError(Interpolator<T> & ip, std::function<T(T)> f, double a, double b, int k) override {};
                    // Override dell'operatore () per interpolare in un dato punto t
                    T operator()(T t) const override {
                        return alglib::barycentriccalc(p_, t);
                    }
                    void plotError(Interpolator<T> & ip, double a, double b, int k){;}

            };

        template <typename T>
            class SplineInterpolator : public Interpolator<T> {

                private:
                    alglib::real_1d_array x_, y_;
                    alglib::spline1dinterpolant s_;  // Interpolante spline cubic
                    std::vector<std::pair<int, double>> n_E;

                public:
                    SplineInterpolator() = default;

                    // Costruzione dell'interpolante spline cubica 
                    void build(const std::vector<T> x,const std::vector<T> y, int n, double a, double b) {
                        if (x.size() != y.size()) {
                            throw std::invalid_argument("x and y vectors must have the same size.");
                        }
                        const double * idx_x;
                        idx_x = x.data(); 
                        const double * idx_y;
                        idx_y = y.data(); 

                        x_.setcontent(x.size(), idx_x);
                        y_.setcontent(y.size(), idx_y);

                        alglib::spline1dbuildcubic(x_, y_,s_);

                    }

                    // Override dell'operatore () per interpolare in un dato punto t
                    double operator()(T t) const override {
                        return alglib::spline1dcalc(s_, t);
                    }
                    void plotError(Interpolator<T> & ip, double a, double b, int k) override {;}


            };

    }
}
#endif
