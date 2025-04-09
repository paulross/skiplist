set logscale x
set grid
set title "Computing a Rolling Median with multiprocessing SharedMemory. 4 CPUs"

#set logscale y
set xlabel "Number of rows (16 columns)"
set ylabel "Speed compared to a Single Process"
#set yrange [0:1600]

# set logscale y2
#set y2label "insert() + remove() per second"
#set y2range [800000:2400000]
#set y2tics

set pointsize 1
set datafile separator whitespace

set key left

set terminal png size 1000,700           # choose the file format
set output "images/perf_rolling_median_shared_memory_ratio.png"   # choose the output device

#plot "perf_rolling_median_shared_memory_ratio.dat" using 1:2 t "processes=1" with linespoints axes x1y1 pt 2 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:3 t "processes=2" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:4 t "processes=3" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:5 t "processes=4" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:6 t "processes=5" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:7 t "processes=6" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:8 t "processes=7" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:9 t "processes=8" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:10 t "processes=9" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:11 t "processes=10" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:12 t "processes=11" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:13 t "processes=12" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:14 t "processes=13" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:15 t "processes=14" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:16 t "processes=15" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:17 t "processes=16" with linespoints  axes x1y1 pt 1 lw 2

# plot "perf_rolling_median_shared_memory_ratio.dat" using 1:2 t "processes=1" with linespoints axes x1y1 pt 2 lw 1, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:3 t "processes=2" with linespoints  axes x1y1 pt 1 lw 1, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:5 t "processes=4" with linespoints  axes x1y1 pt 1 lw 2, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:9 t "processes=8" with linespoints  axes x1y1 pt 1 lw 1, \
     "perf_rolling_median_shared_memory_ratio.dat" using 1:17 t "processes=16" with linespoints  axes x1y1 pt 1 lw 1

plot "dat/perf_rolling_median_shared_memory_ratio.dat" using 1:2 t "Single process" with lines axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_ratio.dat" using 1:3 t "2 processes" with lines  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_ratio.dat" using 1:5 t "4 processes" with lines  axes x1y1 lw 3, \
     "dat/perf_rolling_median_shared_memory_ratio.dat" using 1:9 t "8 processes" with lines  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_ratio.dat" using 1:17 t "16 processes" with lines  axes x1y1 lw 2

reset
