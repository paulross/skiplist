set logscale x
set grid
set title "Speed of calling index(value) for a skip list of 1 million doubles."
set xlabel "Position of the value in the skip list"
set ylabel "Time (ns)"
set yrange [0:300]
set pointsize 1
set datafile separator "\t"

# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "perf_index.dat" using 1:2 via a,b

set terminal png size 750,500           # choose the file format
set output "perf_index.png"   # choose the output device

plot "perf_index.dat" using 1:2 t "index()" with linespoints pt 2 lw 2

reset
