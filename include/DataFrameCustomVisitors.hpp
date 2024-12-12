#ifndef CUSTOM_VISITOR_H
#define CUSTOM_VISITOR_H
#include <DataFrame/DataFrame.h>                   // Main DataFrame header
#include <DataFrame/DataFrameStatsVisitors.h>      // Statistical algorithms
                                                   //
#include <vector>
#include <string>
#include <functional>
#include <stdexcept>
#include <InterpolateWrapper.hpp>
    

template<typename IndexType, typename ValueType>
struct ClassifyVisitor {
    using index_type = IndexType;   // Index column type
    using value_type = ValueType;  // Data column type
    using result_type = std::vector<std::string>;  // The classification result is a vector of strings

private:
    std::vector<std::string> categories;  // Categories to classify into
    std::vector<std::function<bool(ValueType)>> conditions;  // Conditions for classification
    result_type result_;  // Store classification results

public:
    // Constructor: Initialize categories and conditions
    ClassifyVisitor(
        const std::vector<std::string>& categories,
        const std::vector<std::function<bool(ValueType)>>& conditions)
        : categories(categories), conditions(conditions) 
    {
        // Ensure the number of categories matches the number of conditions
        if (categories.size() != conditions.size()) {
            throw std::invalid_argument("Categories and conditions sizes must match.");
        }
    }

    // Called before processing data
    void pre() {
        result_.clear();  // Reset result for a fresh computation
    }
    
    // Overloaded operator() for single element visits (required by DataFrame::visit)
    void operator()(const index_type& index, const value_type& value) {
        bool matched = false;

        for (size_t i = 0; i < categories.size(); ++i) {
            if (conditions[i](value)) {
                result_.push_back(categories[i]);
                matched = true;
                break;
            }
        }

        if (!matched) {
            result_.push_back("Uncategorized");
        }
    }


    // Called after processing data
    void post() {
        // Finalize result processing if necessary
    }

    // Access the result (const version)
    const result_type& get_result() const {
        return result_;
    }

    // Access the result (non-const version)
    result_type& get_result() {
        return result_;
    }
};

#ifdef INTERPOLATION_MODULE
template<typename IndexType, typename ValueType>
struct InterpolationVisitor {
    using index_type = IndexType;   // Index column type
    using value_type = ValueType;  // first Data column type
    using result_type = double;  // The classification result is a double 
private:
    value_type point;
    int lb, ub;
    const char * InterpolationMethod;
    result_type result_;  // Store interpolation result

public:
    // Constructor: Initialize the number of elements in the column(s), upper bound and lower bound   
    InterpolationVisitor(double point, int lb, int ub, const char * InterpolationMethod): point(point), lb(lb), ub(ub), InterpolationMethod(InterpolationMethod){
        
    }

    // Called before processing data
    void pre() {
        result_ = 0.0;  // Reset result for a fresh computation
    }
    
    // Overloaded operator() for single element visits (required by DataFrame::visit)
    
        /* (const index_type &idx, */
        /*                     const value_type &val1, const value_type &val2) */
        /* (const index_type &idx_begin, const index_type &idx_end, */
        /*         const value_type &column_begin1, const value_type &column_end1, */
        /*         const value_type &column_begin2, const value_type &column_end2) */

    template <typename K, typename H>
    inline void operator() (const K &idx_begin, const K &idx_end,
                const H &column_begin1, const H &column_end1,
                const H &column_begin2, const H &column_end2) 
    {
        
        std::vector<value_type> x;
        std::vector<value_type> y;
        x.assign(column_begin1, column_end1);
        y.assign(column_begin2, column_end2);

        if (InterpolationMethod == "Linear"){
           LinearInterpolator<double> A;
           A.build(x, y);
           result_ = A(point);
        }
        else if (InterpolationMethod == "LagrangeCasual"){
           LagrangeInterpolator<double> A;
           A.buildCasual(x, y, y.size());
           result_ = A(point);
        }
        else if (InterpolationMethod=="LagrangeEquidistant"){
           LagrangeInterpolator<double> A;
           A.buildEquidistant(y, y.size(), lb, ub);
           result_ = A(point);
           
        }
        else if (InterpolationMethod=="LagrangeChebyshev"){
           LagrangeInterpolator<double> A;
           A.buildChebyshev(y, y.size(), lb, ub);
           result_ = A(point);
        }
        else if (InterpolationMethod == "Spline"){
           SplineInterpolator<double> A;
           A.build(x, y);
           result_ = A(point);
        }
        else{
            throw std::invalid_argument(
            "Invalid method type provided. Expected one of ['Linear', 'LagrangeCasual', 'LagrangeEquidistant', 'LagrangeChebyshev', 'Spline'], but received: '" + std::string(InterpolationMethod) + "'."
        );
        }

    }


    // Called after processing data
    void post() {
        // Finalize result processing if necessary
    }

    // Access the result (const version)
    const result_type& get_result() const {
        return result_;
    }

    // Access the result (non-const version)
    result_type& get_result() {
        return result_;
    }
};
#endif

#endif // CUSTOM_VISITOR_H
