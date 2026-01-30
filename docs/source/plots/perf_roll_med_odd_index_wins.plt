set logscale x
# set logscale y
set grid
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
