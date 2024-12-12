# ADVANCED PROGRAMMING HOMEWORK 2: implementation of a scientific computing toolbox
All files and folders for the second assignment of the Advanced Programming course at UniTS, A.2024/45
Group members:
- Giovanni Billo billogiova@gmail.com
- Tommaso Piscitelli
Individual contributions
Giovanni Billo:
- Implementation of the entire statistics module
- Implementation of custom Visitors(https://htmlpreview.github.io/?https://github.com/hosseinmoein/DataFrame/blob/master/docs/HTML/DataFrame.html#5) for classification and interpolation 
- Tests for both modules
- CMakeFile.txt configurations and code organization for both modules

Tommaso Piscitelli:

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


# BUILD INSTRUCTIONS
A CMakeLists.txt file handling the whole build process is provided for both working examples (in the main project folder) and test suites (in the tests folder).
For each case it will be sufficient to execute the usual commands
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

