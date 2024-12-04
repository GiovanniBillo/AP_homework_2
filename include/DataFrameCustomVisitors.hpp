#ifndef CUSTOM_VISITOR_H
#define CUSTOM_VISITOR_H
#include <DataFrame/DataFrame.h>                   // Main DataFrame header
#include <DataFrame/DataFrameStatsVisitors.h>      // Statistical algorithms
                                                   //
#include <vector>
#include <string>
#include <functional>
#include <stdexcept>

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

#endif // CUSTOM_VISITOR_H

/* #ifndef CUSTOM_VISITOR_H */
/* #define CUSTOM_VISITOR_H */

/* #include "DataFrameStatsVisitors.h" */

/* template<typename IndexType, typename ValueType> */
/* struct ClassifyVisitor { */
/*     using index_type = IndexType;   // Index column type */
/*     using value_type = ValueType;  // Data column type */
/*     using result_type = std::vector<std::string>;  // The classification result is a vector of strings (one for each data point) */ 

/* private: */
/*     result_type result_;  // Store results here */

/* public: */
/*     // Constructor: Define any parameters your visitor needs */
/*     ClassifyVisitor( */
/*         const std::vector<std::string>& categories, */
/*         const std::vector<std::function<bool(CT)>>& conditions): */
/*         categories(categories), conditions(conditions) */ 
/*     { */
/*         // Ensure the number of categories matches the number of conditions */
/*         if (categories.size() != conditions.size()) { */
/*             throw std::invalid_argument("Categories and conditions sizes must match."); */
/*         } */

/*         std::vector<std::string> classifications; */
/*         /1* const std::vector<ValueType>& data = df. template get_column<ValueType>(columnName); *1/ */
/*     } */ 

/*     // Called before processing data */
/*     void pre() { */
/*         result_.clear();  // Reset result for a fresh computation */
/*     } */

/*     // The main algorithm: Functor implementation */
/*     void operator()( */
/*         typename std::vector<index_type>::const_iterator index_begin, */
/*         typename std::vector<index_type>::const_iterator index_end, */
/*         typename std::vector<value_type>::const_iterator data_begin, */
/*         typename std::vector<value_type>::const_iterator data_end */
/*         /1* Add more begin/end pairs if needed *1/ */
/*     ) { */
/*         auto index_it = index_begin; */
/*         auto data_it = data_begin; */

/*         while (index_it != index_end && data_it != data_end) { */
/*             /1* // Example: Store the value if it meets some condition *1/ */
/*             /1* if (*data_it > 0) {  // Replace with your condition *1/ */
/*             /1*     result_.push_back(static_cast<double>(*data_it));  // Modify logic as needed *1/ */
/*             /1* } *1/ */
/*             bool matched = false; */
/*             for (size_t i = 0; i < categories.size(); ++i) { */
/*                 if (conditions[i](value)) { */
/*                     classifications.push_back(categories[i]); */
/*                     matched = true; */
/*                     break; */
/*                 } */
/*             } */
/*             // Default classification if no condition matches */
/*             if (!matched) { */
/*                 classifications.push_back("Uncategorized"); */
/*             } */

/*             ++index_it; */
/*             ++data_it; */
/*         } */
/*         result_ = classifications; */
/*     } */

/*     // Called after processing data */
/*     void post() { */
/*         // Finalize result processing if necessary */
/*     } */

/*     // Access the result (const version) */
/*     const result_type& get_result() const { */
/*         return result_; */
/*     } */

/*     // Access the result (non-const version) */
/*     result_type& get_result() { */
/*         return result_; */
/*     } */
/* }; */

/* #endif // CUSTOM_VISITOR_H */

