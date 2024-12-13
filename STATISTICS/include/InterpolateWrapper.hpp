#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#ifdef INTERPOLATION_MODULE
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
                private:
                    std::vector<std::pair<int, double>> n_E;
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
                    /* virtual void plotError(Interpolator<T> & ip, std::function<T(T)> f, double a, double b, int k) = 0; */
                    
 

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


            };

        template <typename T>
            class LagrangeInterpolator : public Interpolator<T> {

                private:
                    std::vector<T> x_;
                    alglib::barycentricinterpolant p_;  // Interpolante Lagrange in forma baricentrica
                    std::vector<std::pair<int, double>> n_E;


                public:
                    LagrangeInterpolator() = default;


                    void build(const std::vector<T> x,const std::vector<T> y, int n, double a, double b){

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
                        alglib::polynomialbuild(x_, y_, n, p_);
                    }
                    void buildEquidistant(const std::vector<T> x,const std::vector<T> y, int n, double a, double b) {
                        if (y.size() != static_cast<size_t>(n)) {
                            throw std::invalid_argument("y vector size must be equal to n.");
                        }

                        alglib::real_1d_array y_;
                        const double * idx; 
                        idx = y.data(); 
                        y_.setcontent(n, idx);
    
                        alglib::polynomialbuildeqdist(a, b, y_, n, p_);
                    }
                    void buildChebyshev(const std::vector<T> x,const std::vector<T> y, int n, double a, double b) {
                        if (y.size() != static_cast<size_t>(n)) {
                            throw std::invalid_argument("f vector size must be equal to n.");
                        }

                                                                         

                        alglib::real_1d_array y_;
                        const double * idx; 
                        idx = y.data(); 
                        y_.setcontent(n, idx);
                        

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
                            t[i+1] = (b - a)/100 + t[i] ;  
                            Errore[i]= std::abs( f(t[i]) - alglib::barycentriccalc(p_, t[i]) );

                        } 
                        auto max_iter = std::max_element(Errore.begin(), Errore.end());
                        double max = *max_iter;
                        return max;
                    } 

                    T operator()(T t) const override {
                        return alglib::barycentriccalc(p_, t);
                    }
            };

        template <typename T>
            class SplineInterpolator : public Interpolator<T> {

                private:
                    alglib::real_1d_array x_, y_;
                    alglib::spline1dinterpolant s_;  // cubic spline interpolant 
                    std::vector<std::pair<int, double>> n_E;

                public:
                    SplineInterpolator() = default;

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
                      double Error(std::function<double(double)> f, double a, double b) override {
                        std::vector<double> t(101);  // 101 points to include both endpoints
                        std::vector<double> Errore(101);

                        // Compute the interpolation points
                        double step = (b - a) / 100;
                        for (int i = 0; i <= 100; ++i) {
                            t[i] = a + i * step;
                        }

                        // Compute the error at each point
                        for (int i = 0; i <= 100; ++i) {
                            double interpolatedValue = alglib::spline1dcalc(s_, t[i]);  // Use the spline interpolant
                            Errore[i] = std::abs(f(t[i]) - interpolatedValue);
                        }

                        // Find the maximum error
                        auto max_iter = std::max_element(Errore.begin(), Errore.end());
                        double max = *max_iter;

                        return max;
                    }


                    double operator()(T t) const override {
                        return alglib::spline1dcalc(s_, t);
                    }


            };

    }
}
#endif

#endif
