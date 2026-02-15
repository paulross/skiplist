# Plots the results of the very long running (six hours) benchmark for rolling median with shared memory.
#set logscale x

# First line specification refers to major grid lines in both x and y, the second to minor grid lines in x and y.
set grid xtics mxtics ytics mytics linetype -1 linewidth 1, linetype 0 linewidth 1

#set logscale y
set xlabel "Number of Processes"
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

set title "Rolling Median with multiprocessing SharedMemory, 4 CPUs"
set terminal png size 1000,600           # choose the file format

set xlabel "Number of Processes"

# Time plots.

set output "images/benchmarks/perf_rolling_median_shared_memory_benchmark_time.png"   # choose the output device

plot "dat/benchmarks/CPython_3.14.2_test_rolling_median_sh_mem_8388608_16.dat_perm" using 1:2:($3 - $4):($3 + $4):5 t "8388608 x 16" with candlesticks whiskerbars 0.5, \
     "dat/benchmarks/CPython_3.14.2_test_rolling_median_sh_mem_8388608_16.dat_perm" using 1:2 t "8388608 x 16 Minimum" with lines lw 2, \
     "dat/benchmarks/CPython_3.14.2_test_rolling_median_sh_mem_131072_1024.dat_perm" using 1:2:($3 - $4):($3 + $4):5 t "131072 x 1024" with candlesticks whiskerbars 0.5, \
     "dat/benchmarks/CPython_3.14.2_test_rolling_median_sh_mem_131072_1024.dat_perm" using 1:2 t "131072 x 1024 Minimum" with lines lw 2, \
     "dat/benchmarks/CPython_3.14.2_test_rolling_median_sh_mem_2048_65536.dat_perm" using 1:2:($3 - $4):($3 + $4):5 t "2048 x 65536" with candlesticks whiskerbars 0.5, \
     "dat/benchmarks/CPython_3.14.2_test_rolling_median_sh_mem_2048_65536.dat_perm" using 1:2 t "2048 x 65536 Minimum" with lines lw 2

#plot "dat/perf_rolling_median_shared_memory_cols_16.dat" using ($1 * 16 / 1e6):2 t "16 Columns, 1 process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using ($1 * 1024 / 1e6):2 t "1024 Columns, 1 process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using ($1 * 65536 / 1e6):2 t "65536 Columns, 1 process" with linespoints axes x1y1 lw 2

#set output "images/perf_rolling_median_shared_memory_cols_all_four_processes.png"   # choose the output device

#plot "dat/perf_rolling_median_shared_memory_cols_16.dat" using ($1 * 16 / 1e6):4 t "16 Columns, 4 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using ($1 * 1024 / 1e6):4 t "1024 Columns, 4 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using ($1 * 65536 / 1e6):4 t "65536 Columns, 4 processes" with linespoints  axes x1y1 lw 2

#set output "images/perf_rolling_median_shared_memory_cols_all.png"   # choose the output device

#plot "dat/perf_rolling_median_shared_memory_cols_16.dat" using ($1 * 16 / 1e6):2 t "16 Columns, 1 process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_16.dat" using ($1 * 16 / 1e6):4 t "16 Columns, 4 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using ($1 * 1024 / 1e6):2 t "1024 Columns, 1 process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using ($1 * 1024 / 1e6):4 t "1024 Columns, 4 processes" with linespoints  axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using ($1 * 65536 / 1e6):2 t "65536 Columns, 1 process" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using ($1 * 65536 / 1e6):4 t "65536 Columns, 4 processes" with linespoints  axes x1y1 lw 2

# Ratio plots

set key left
set ylabel "Ratio Compared to a Single process"

unset logscale y

#set xrange [0.01:]
#set output "images/perf_rolling_median_shared_memory_cols_all_ratio.png"   # choose the output device

#plot "dat/perf_rolling_median_shared_memory_cols_16.dat" using ($1 * 16 / 1e6):($2 / $4) t "16 Columns" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_1024.dat" using ($1 * 1024 / 1e6):($2 / $4) t "1024 Columns" with linespoints axes x1y1 lw 2, \
     "dat/perf_rolling_median_shared_memory_cols_65536.dat" using ($1 * 65536 / 1e6):($2 / $4) t "65536 Columns" with linespoints axes x1y1 lw 2

reset
