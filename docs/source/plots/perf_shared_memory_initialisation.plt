
set logscale x
set xlabel "Number of numpy Floating Pont Values (million)"
set xrange [1:1000]

# First line specification refers to major grid lines in both x and y, the second to minor grid lines in x and y.
set grid xtics mxtics ytics mytics linetype -1 linewidth 1, linetype 0 linewidth 1

set logscale y
set ylabel "Time (ms)"
#set yrange [0:1600]

# set logscale y2
#set y2label "insert() + remove() per second"
#set y2range [800000:2400000]
#set y2tics

set pointsize 1
set datafile separator whitespace

set key left

set terminal png size 1000,600           # choose the file format

set title "Time to Initialise Shared Memory"
set output "images/benchmarks/CPython_3.14.2_perf_shared_memory_initialisation.png"   # choose the output device

# CPython_3.14.2_test_sh_mem_create_copy.dat

plot "dat/benchmarks/CPython_3.14.2_test_sh_mem_create_copy.dat" using ($1 / 1e6):($2 * 1e3):(($3 - $4) * 1e3):(($3 + $4) * 1e3):($5 * 1e3) t "Python 3.14.2 with copy" with candlesticks whiskerbars 0.5, \
     "dat/benchmarks/CPython_3.14.2_test_sh_mem_create_copy.dat" using ($1 / 1e6):($2 * 1e3) t "Python 3.14.2 with copy, minimum" with linespoints lw 2, \
     "dat/benchmarks/CPython_3.14.2_test_sh_mem_create_no_copy.dat" using ($1 / 1e6):($2 * 1e3):(($3 - $4) * 1e3):(($3 + $4) * 1e3):($5 * 1e3) t "Python 3.14.2 no copy" with candlesticks whiskerbars 0.5, \
     "dat/benchmarks/CPython_3.14.2_test_sh_mem_create_no_copy.dat" using ($1 / 1e6):($2 * 1e3) t "Python 3.14.2 no copy, minimum" with linespoints lw 2


set title "Time to Create numpy Array from Shared Memory"
# CPython_3.14.2_test_copy_shared_memory_into_new_numpy_array.dat
set output "images/benchmarks/CPython_3.14.2_perf_shared_memory_into_new_numpy_array.png"   # choose the output device

plot "dat/benchmarks/CPython_3.14.2_test_copy_shared_memory_into_new_numpy_array.dat" using ($1 / 1e6):($2 * 1e3):(($3 - $4) * 1e3):(($3 + $4) * 1e3):($5 * 1e3) t "Python 3.14.2" with candlesticks whiskerbars 0.5, \
     "dat/benchmarks/CPython_3.14.2_test_copy_shared_memory_into_new_numpy_array.dat" using ($1 / 1e6):($2 * 1e3) t "Python 3.14.2, minimum" with linespoints lw 2

reset
