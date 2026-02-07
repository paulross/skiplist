set logscale x

# First line specification refers to major grid lines in both x and y, the second to minor grid lines in x and y.
set grid xtics mxtics ytics mytics linetype -1 linewidth 1, linetype 0 linewidth 1

set title "Node height growth with skip list size. p()=0.5"
set xlabel "Index"
set ylabel "Height of Head Node"
set pointsize 1
set datafile separator "\t"
# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "perf_height_size.dat" using 1:2 via a,b

set terminal png size 1000,600           # choose the file format
set output "images/perf_height_size.png"   # choose the output device

plot "dat/perf_height_size.dat" using 2:3 t "Height" with linespoints pt 19 lw 2

reset
