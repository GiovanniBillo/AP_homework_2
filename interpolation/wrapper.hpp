#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#include <vector>
#include <stdexcept>
#include <cmath>
#include <functional>
#include "interpolation.h"  

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

};


template <typename T>
class LinearInterpolator : public Interpolator<T> {

private:
    std::vector<T> x_, y_;

public:
    LinearInterpolator() = default;
    
    // Costruzione dell'interpolante lineare
    void build(const std::vector<T> & x, const std::vector<T> & y) {
        if (x.size() != y.size()) {
            throw std::invalid_argument("x and y vectors must have the same size.");
        }   
        /* TODO: check if T is numeric */


        x_ = x;
        y_ = y;
    }

    // Override dell'operatore () per interpolare in un dato punto t
    T operator()(T t) const override {
        // Interpolazione lineare tra due punti vicini
        for (size_t i = 1; i < x_.size(); ++i) {
            if (t <= x_[i]) {
                T x0 = x_[i - 1], x1 = x_[i];
                T y0 = y_[i - 1], y1 = y_[i];
                return y0 + (y1 - y0) * (t - x0) / (x1 - x0);
            }
        }
        return y_.back();  // Se t è fuori intervallo, ritorna l'ultimo valore
    }
    

};



template <typename T>
class LagrangeInterpolator : public Interpolator<T> {

private:
    alglib::barycentricinterpolant p_;  // Interpolante Lagrange in forma baricentrica
    std::vector<T> x_;

public:
    LagrangeInterpolator() = default;
    
    // Metodo per costruire il modello di Lagrange con griglia casuale
    void buildCasual(const std::vector<T> x,const std::vector<T> y, int n){
        
        if (x.size() != y.size()) {
            throw std::invalid_argument("x and y vectors ust have the same size.");
        };

        alglib::real_1d_array x_ , y_;
        for (size_t i = 0; i < y.size(); ++i) {
            y_[i] = y[i];
            x_[i] = x[i];
        }
        
        // Costruzione dell'interpolante 
        alglib::polynomialbuild(x_,y_,n,p_);

}
    // Metodo per costruire il modello di Lagrange con griglia equidistante
    void buildEquidistant(const std::vector<T> y, int n, double a, double b) {
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
    void buildChebyshev(const std::vector<T>& y, int n, double a, double b) {
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

    // Override dell'operatore () per interpolare in un dato punto t
    T operator()(T t) const override {
        return alglib::barycentriccalc(p_, t);
    }

};

template <typename T>
class SplineInterpolator : public Interpolator<T> {

private:
    alglib::spline1dinterpolant s_;  // Interpolante spline cubica
    alglib::real_1d_array x_, y_;

public:
    SplineInterpolator() = default;

    // Costruzione dell'interpolante spline cubica 
    void build(const alglib::real_1d_array x, const alglib::real_1d_array y) {
        if (x.length() != y.length()) {
            throw std::invalid_argument("x and y vectors must have the same size.");
        }

        x_ = x;
        y_ = y;
        
         alglib::spline1dbuildcubic(x_, y_,s_);

    }

    // Override dell'operatore () per interpolare in un dato punto t
    double operator()(T t) const override {
        return alglib::spline1dcalc(s_, t);
    }

};


#endif
