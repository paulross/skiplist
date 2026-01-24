set logscale x
set grid
set title "Computing a Rolling Median with multiprocessing SharedMemory. 4 CPUs"

set logscale y
set xlabel "Number of rows (16 columns)"
set ylabel "Time to Compute a Rolling Median (s)"
#set yrange [0:1600]

# set logscale y2
#set y2label "insert() + remove() per second"
#set y2range [800000:2400000]
#set y2tics

set pointsize 1
set datafile separator whitespace

set key left

set terminal png size 1000,700           # choose the file format
set output "images/perf_rolling_median_shared_memory.png"   # choose the output device

plot "dat/perf_rolling_median_shared_memory.dat" using 1:2 t "Single process" with lines axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory.dat" using 1:3 t "2 processes" with lines  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory.dat" using 1:4 t "4 processes" with lines  axes x1y1 lw 3, \
     "dat/perf_rolling_median_shared_memory.dat" using 1:5 t "8 processes" with lines  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory.dat" using 1:6 t "16 processes" with lines  axes x1y1 lw 2

reset
