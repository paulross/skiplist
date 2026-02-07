set logscale x
# set logscale y

# First line specification refers to major grid lines in both x and y, the second to minor grid lines in x and y.
set grid xtics mxtics ytics mytics linetype -1 linewidth 1, linetype 0 linewidth 1

set title "Speed of Rolling Median for a skip list of 1 million doubles."
set xlabel "Window Length"
set ylabel "Time (ms)"
set yrange [0:]
set pointsize 2
set datafile separator "\t"
# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "perf_roll_med_odd_index_wins.dat" using 1:2 via a,b

set terminal png size 1000,600           # choose the file format
set output "images/perf_roll_med_odd_index_wins.png"   # choose the output device

plot "dat/perf_roll_med_odd_index_wins.dat" using 1:2 t "Rolling Median" with linespoints pt 1 lw 2
reset
