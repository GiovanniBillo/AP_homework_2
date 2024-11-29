// libraries imported from DataFrame 
#include <DataFrame/DataFrame.h>                   // Main DataFrame header
#include <DataFrame/DataFrameStatsVisitors.h>      // Statistical algorithms

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "Wrapper.hpp"

// -----------------------------------------------------------------------------

/* // DataFrame library is entirely under hmdf name-space */

/* using namespace hmdf; */

/* // A DataFrame with ulong index type */
/* // */
/* using ULDataFrame = StdDataFrame<unsigned long>; */

/* // A DataFrame with string index type */
/* // */
/* using StrDataFrame = StdDataFrame<std::string>; */

/* // A DataFrame with DateTime index type */
/* // */
/* using DTDataFrame = StdDataFrame<DateTime>; */

/* // This is just some arbitrary type to show how any type, including the DataFrame itself, could be in DataFrame */
/* // */
/* struct  MyData  { */
/*     int         i { 10 }; */
/*     double      d { 5.5 }; */
/*     std::string s { "Some Arbitrary String" }; */

/*     MyData() = default; */
/* }; */

// -----------------------------------------------------------------------------


int main(int, char *[])  {

    DataFrameWrapper dfw;
    dfw.loadAndReadFile();
    /* StrDataFrame    ibm_df; */

    /* // Also, you can load data into a DataFrame from a file, supporting a few different formats. If the file cannot be found, */
    /* // an exception will be thrown. If the DataFrame data directory is your current directory when running this, it should */
    /* // work fine. */
    /* // */
    /* ibm_df.read("superstore_updated.csv", io_format::csv2); */
    
    return (0);
}

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
