# Set terminal and output format
set terminal pngcairo size 800,600
set output 'plot.png'

# Set labels and title
set xlabel "n.of points"
set ylabel "errors"
set title "Data with Error Bars (CSV)"

# Specify the separator for CSV files
set datafile separator ','

# Plot data with error bars
plot 'errors.csv' using 1:3 with yerrorbars title "Data with Errors" pointtype 7 linewidth 2
