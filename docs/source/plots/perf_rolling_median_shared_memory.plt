set logscale x

# First line specification refers to major grid lines in both x and y, the second to minor grid lines in x and y.
set grid xtics mxtics ytics mytics linetype -1 linewidth 1, linetype 0 linewidth 1

set logscale y
set xlabel "Number of Rows"
set ylabel "Time to Compute a Rolling Median (s)"
#set yrange [0:1600]

# set logscale y2
#set y2label "insert() + remove() per second"
#set y2range [800000:2400000]
#set y2tics

set pointsize 1
set datafile separator whitespace

set key left

# set title "Big Title\n{/*0.5 subtitle}"

set title "Rolling Median with multiprocessing SharedMemory, 4 CPUs\nColumns: 16"
set terminal png size 1000,600           # choose the file format
set output "images/perf_rolling_median_shared_memory_cols_16.png"   # choose the output device

plot "dat/perf_rolling_median_shared_memory_cols_16.dat" using 1:2 t "Single process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_16.dat" using 1:3 t "2 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_16.dat" using 1:4 t "4 processes" with linespoints  axes x1y1 lw 3, \
     "dat/perf_rolling_median_shared_memory_cols_16.dat" using 1:5 t "8 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_16.dat" using 1:6 t "16 processes" with linespoints  axes x1y1 lw 2

set title "Rolling Median with multiprocessing SharedMemory, 4 CPUs\nColumns: 1024"
set terminal png size 1000,600           # choose the file format
set output "images/perf_rolling_median_shared_memory_cols_1024.png"   # choose the output device

plot "dat/perf_rolling_median_shared_memory_cols_1024.dat" using 1:2 t "Single process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using 1:3 t "2 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using 1:4 t "4 processes" with linespoints  axes x1y1 lw 3, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using 1:5 t "8 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using 1:6 t "16 processes" with linespoints  axes x1y1 lw 2

set title "Rolling Median with multiprocessing SharedMemory, 4 CPUs\nColumns: 65536"
set terminal png size 1000,600           # choose the file format
set output "images/perf_rolling_median_shared_memory_cols_65536.png"   # choose the output device

plot "dat/perf_rolling_median_shared_memory_cols_65536.dat" using 1:2 t "Single process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using 1:3 t "2 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using 1:4 t "4 processes" with linespoints  axes x1y1 lw 3, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using 1:5 t "8 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using 1:6 t "16 processes" with linespoints  axes x1y1 lw 2

# Comparison plot

set title "Rolling Median with multiprocessing SharedMemory"
set terminal png size 1000,600           # choose the file format

set xlabel "Number of Data Points (m)"

set output "images/perf_rolling_median_shared_memory_cols_single_process.png"   # choose the output device

plot "dat/perf_rolling_median_shared_memory_cols_16.dat" using ($1 * 16 / 1e6):2 t "16 Columns, 1 process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using ($1 * 1024 / 1e6):2 t "1024 Columns, 1 process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using ($1 * 65536 / 1e6):2 t "65536 Columns, 1 process" with linespoints axes x1y1 lw 2

set output "images/perf_rolling_median_shared_memory_cols_all_four_processes.png"   # choose the output device

plot "dat/perf_rolling_median_shared_memory_cols_16.dat" using ($1 * 16 / 1e6):4 t "16 Columns, 4 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using ($1 * 1024 / 1e6):4 t "1024 Columns, 4 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using ($1 * 65536 / 1e6):4 t "65536 Columns, 4 processes" with linespoints  axes x1y1 lw 2

set output "images/perf_rolling_median_shared_memory_cols_all.png"   # choose the output device

plot "dat/perf_rolling_median_shared_memory_cols_16.dat" using ($1 * 16 / 1e6):2 t "16 Columns, 1 process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_16.dat" using ($1 * 16 / 1e6):4 t "16 Columns, 4 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using ($1 * 1024 / 1e6):2 t "1024 Columns, 1 process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using ($1 * 1024 / 1e6):4 t "1024 Columns, 4 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using ($1 * 65536 / 1e6):2 t "65536 Columns, 1 process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using ($1 * 65536 / 1e6):4 t "65536 Columns, 4 processes" with linespoints  axes x1y1 lw 2

# Rate plots

set key right
set ylabel "Time per Value (ns)"
set xrange [0.01:]
set output "images/perf_rolling_median_shared_memory_cols_all_rate.png"   # choose the output device

plot "dat/perf_rolling_median_shared_memory_cols_16.dat" using ($1 * 16 / 1e6):(1e9 * $2 / ($1 * 16)) t "16 Columns, 1 process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_16.dat" using ($1 * 16 / 1e6):(1e9 * $4 / ($1 * 16)) t "16 Columns, 4 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using ($1 * 1024 / 1e6):(1e9 * $2 / ($1 * 1024)) t "1024 Columns, 1 process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using ($1 * 1024 / 1e6):(1e9 * $4 / ($1 * 1024)) t "1024 Columns, 4 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using ($1 * 65536 / 1e6):(1e9 * $2 / ($1 * 65536)) t "65536 Columns, 1 process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using ($1 * 65536 / 1e6):(1e9 * $4 / ($1 * 65536)) t "65536 Columns, 4 processes" with linespoints  axes x1y1 lw 2

reset
