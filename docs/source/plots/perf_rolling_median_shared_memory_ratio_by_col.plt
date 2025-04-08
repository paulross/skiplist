set logscale x
set grid
set title "Computing a Rolling Median with multiprocessing SharedMemory. 4 CPUs"

#set logscale y
set xlabel "Number of rows"
set ylabel "Speed compared to a Single Process"
#set yrange [0:1600]

# set logscale y2
#set y2label "insert() + remove() per second"
#set y2range [800000:2400000]
#set y2tics

set pointsize 1
set datafile separator whitespace

set key right

set terminal png size 750,500           # choose the file format
set output "images/perf_rolling_median_shared_memory_ratio_by_col.png"   # choose the output device

# 4 processes
#plot "perf_rolling_median_shared_memory_ratio_by_col.dat" using 1:14 t "65536 columns" with lines  axes x1y1 lw 2, \
     "perf_rolling_median_shared_memory_ratio_by_col.dat" using 1:9 t "1024 columns" with lines  axes x1y1 lw 2, \
     "perf_rolling_median_shared_memory_ratio_by_col.dat" using 1:4 t "16 columns" with lines axes x1y1 lw 2


# 8 processes
#plot "perf_rolling_median_shared_memory_ratio_by_col.dat" using 1:15 t "65536 columns" with lines  axes x1y1 lw 2, \
     "perf_rolling_median_shared_memory_ratio_by_col.dat" using 1:10 t "1024 columns" with lines  axes x1y1 lw 2, \
     "perf_rolling_median_shared_memory_ratio_by_col.dat" using 1:5 t "16 columns" with lines axes x1y1 lw 2


# 16 processes
plot "dat/perf_rolling_median_shared_memory_ratio_by_col.dat" using 1:16 t "65536 columns" with lines  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_ratio_by_col.dat" using 1:11 t "1024 columns" with lines  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_ratio_by_col.dat" using 1:6 t "16 columns" with lines axes x1y1 lw 2

reset
