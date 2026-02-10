set grid
set title "Python Single and Multi Threaded insert/has/remove"

#set logscale x
set xlabel "Number of Threads (0 is not threaded)"

#set logscale y
set ylabel "Time for insert/has/remove (ns)."
set yrange [0:2000]
# set ytics 0,200,1500

# set y2label "Ratio: compiled threaded / compiled single"
# set y2range [0.9:1.275]
# set y2tics 0.9,0.05,1.275

set pointsize 1
set datafile separator whitespace
# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "perf_height_size.dat" using 1:2 via a,b

set key left

# See: tests.benchmarks.test_benchmark_SkipList_threaded.test_ihr_float_threaded()
FACTOR_NS = 1e9 / (1024 * 8)

set terminal png size 1000,600           # choose the file format
set output "images/benchmarks/perf_py_threaded_vs_single_py.png"   # choose the output device

set boxwidth 4

plot "dat/benchmarks/CPython_3.14.2_test_ihr_float_threaded.dat" using 1:($2 * FACTOR_NS):(($3 - $4) * FACTOR_NS):(($3 + $4) * FACTOR_NS):($5 * FACTOR_NS) t "Python 3.14.2" with candlesticks whiskerbars 0.5, \
     "dat/benchmarks/CPython_3.14.2_test_ihr_float_threaded.dat" using 1:($2 * FACTOR_NS) t "Python 3.14.2 Minimum" with lines lw 2

set output "images/benchmarks/perf_py_threaded_vs_single_py_min_all_pythons.png"   # choose the output device

plot "dat/benchmarks/CPython_3.10.11_test_ihr_float_threaded.dat" using 1:($2 * FACTOR_NS) t "Python 3.10.11" with linespoints lw 2, \
     "dat/benchmarks/CPython_3.11.9_test_ihr_float_threaded.dat" using 1:($2 * FACTOR_NS) t "Python 3.11.9" with linespoints lw 2, \
     "dat/benchmarks/CPython_3.12.7_test_ihr_float_threaded.dat" using 1:($2 * FACTOR_NS) t "Python 3.12.7" with linespoints lw 2, \
     "dat/benchmarks/CPython_3.13.0_test_ihr_float_threaded.dat" using 1:($2 * FACTOR_NS) t "Python 3.13.0" with linespoints lw 2, \
     "dat/benchmarks/CPython_3.14.2_test_ihr_float_threaded.dat" using 1:($2 * FACTOR_NS) t "Python 3.14.2" with linespoints lw 2

reset
