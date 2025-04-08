set grid
set title "Threaded insert(), at() and remove()"

# First line specification refers to major grid lines in both x and y, the second to minor grid lines in x and y.
set grid xtics mxtics ytics mytics linetype -1 linewidth 1, linetype 0 linewidth 1

set logscale x
set xlabel "Number of Threads"

set logscale y
set ylabel "Time per insert(), at() and remove() (µs)."
set yrange [0.1:100]
#set ytics
#set ymtics
#set ytics 0,2000,15000

#set y2label "Ratio compared to single thread."
#set logscale y2
#set y2range [0.1:100]
#set y2tics
#set my2tics

set pointsize 1
set datafile separator whitespace
# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "perf_height_size.dat" using 1:2 via a,b

set terminal png size 750,500           # choose the file format
set output "images/perf_py_threaded_sim_rolling_median.png"   # choose the output device

plot "dat/perf_py_threaded_sim_rolling_median.dat" using 1:($2 / 1000 / $1) t "Time/op (left)" with linespoints pt 1 lw 2

reset
