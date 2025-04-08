set logscale x
set grid
set title "Node height growth with skip list size. p()=0.5"
set xlabel "Index"
set ylabel "Height of Head Node"
set pointsize 1
set datafile separator "\t"
# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "perf_height_size.dat" using 1:2 via a,b

set terminal png size 750,500           # choose the file format
set output "images/perf_height_size.png"   # choose the output device

plot "dat/perf_height_size.dat" using 2:3 t "Height" with linespoints pt 19 lw 2

reset
