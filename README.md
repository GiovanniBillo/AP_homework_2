# ADVANCED PROGRAMMING HOMEWORK 2: implementation of a scientific computing toolbox
All files and folders for the second assignment of the Advanced Programming course at UniTS, A.2024/45
Group members:
- Giovanni Billo billogiova@gmail.com
- Tommaso Piscitelli s309639@ds.units.it
Individual contributions
Giovanni Billo:
- Implementation of the entire statistics module
- Implementation of custom Visitors(https://htmlpreview.github.io/?https://github.com/hosseinmoein/DataFrame/blob/master/docs/HTML/DataFrame.html#5) for classification and interpolation 
- Tests for both modules
- CMakeFile.txt configurations and code organization for both modules

Tommaso Piscitelli:
- Implementation of the entire statistics module
- Tests for module
- CMakeFile.txt configurations and code organization


# STATISTICS MODULE (Giovanni Billo)
The module consists of a wrapper that eases the usage of Hossein Moein's DataFrame library [https://github.com/hosseinmoein/DataFrame].
This is a highly performant data analysis library that already implements many tools.
It provides a generalized DataFrameWrapper class that hides the library's technicalities from the user: DataFrame expects the csv files input to it to have a header formatted in this way:
'''
"ColumnName":n_rows:<ColumnType>
'''
with the first column named "INDEX".
The wrapper takes care of parsing each csv to the appropriate format via  the CSVparser header.
Still, **the type of the index column should be passed as a template argument to the DataFrameWrapper class when initializing it**, as well as that of other columns when performing operations on them.

while still maintaining its core funcionalities and adding some useful features.
DataFrame realies on the Visitor class for basically each operation that's performed on data. This makes some of these kind of counterintuitive 
for someone new that's not already well-acquainted with the library.
DataFrameWrapper implements mean, median, standard deviation, correlation, frequency count, classification and interpolation wrappers in a more beginner-friendly Python like syntax.
Some other functionalities were extended, 
For the last 2 operations in this list, classification and interpolation, not present in the original library, custom visitors were implemented following Moein's guidelines

# INTERPOLATION MODULE (Tommaso Piscitelli)
The module consists of a wrapper that implements some methods within the interpolation package of the ALGLIB library, which is a library rich of functions useful for numerical analysis. The module defines an abstract class Interpolator from which the classes LinearInterpolator, LagrangeInterpolator, and SplineInterpolator are derived.

As the names suggest, the LinearInterpolator class, through the build method, allows the interpolation of a data set (vector y) generally interpreted as values corresponding to another data set (vector x) or associated with a real-variable function x, using a piecewise linear polynomial, the LagrangeInterpolator class provides multiple methods for creating an interpolating polynomial depending on the type of vector x (random nodes, equidistant nodes, or Chebyshev nodes). Lastly, the SplineInterpolator class includes another build method that creates a cubic spline interpolant based on two input vectors, x and y.

In each class, the operator () is overloaded, allowing immediate access to a specific element of the interpolant, which is useful for evaluating the pointwise error at a precise point (t). 
An approximate evaluation of the maximum error between the interpolant and a function is provided by the Error method. 
Finally, it is possible to visualize the trend of the error in relation to the number of points used to generate the different types of interpolants using the PlotError function.

## Convergence of interpolation methods 

### Linear Interpolation
![Linear Interpolation](images/error_plot_linear.png)

### Lagrange Interpolation (Chebyshev Nodes)
![Lagrange Interpolation](images/error_plot_chebyshev.png)

### Spline Interpolation
![Spline Interpolation](images/error_plot_spline.png)

using the PlotError function, we plot the graphs to examine the behaviour of our interpolants as the number of points increases.
In this case, just a Linear, Lagrange and Spline interpolant were considered. It can be clearly seen that while with Lagrange and Spline methods an increase in the number of points quickly translates to a drastic error reduction, this is not the case for a linear one, where on the contrary we observe the error increasing.

# BUILD INSTRUCTIONS
A CMakeLists.txt file handling the whole build process is provided for both working examples (in the main project folder) and test suites (in the tests folder).
For each case it will be sufficient to execute the usual commands.
'''
cmake -B build
cd build
make
'''
in each relevant folder.
Some options can be selected at the build stage:
- CMAKE_BUILD_TYPE: build either the "Debug" (default) version of Wrappers (with warnings and debug flags, not optimized) or "Release" (optimized and faster, without debugging flags) 
- DATA_DIR : choose a custom data directory to use in your analysis (if not specified, it will default to the /data folder present in the project)

Usage example:
'''
cmake -B build -CMAKE_BUILD_TYPE=Release -DDATA_DIR=/home/giovanni/example_data_dir # build Release version referring to some data custom data directory
cd build
make // any files compiled with this will have a variable DATA_DIR containing the specified custom data directory, which can then be used in the code 
'''
### Compiler compatibility
The code was tested on g++ compilers and needs version 14+ to utilize some modern C++ features. 

