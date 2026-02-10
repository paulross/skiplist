set grid
set title "Python Single and Multi Threaded insert/has/remove"

#set logscale x
set xlabel "Number of Threads (0 is not threaded)"

#set logscale y
set ylabel "Time for insert/has/remove (ms)."
set yrange [5:15]
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

set terminal png size 1000,600           # choose the file format
set output "images/perf_py_threaded_vs_single_py.png"   # choose the output device

plot "dat/benchmarks/CPython_3.8.10_test_ihr_float_threaded.dat" using 1:($2 * 1e3):(($3 - $4) * 1e3):(($3 + $4) * 1e3):($5 * 1e3) t "Python 3.8.10" with candlesticks whiskerbars 0.5, \
     "dat/benchmarks/CPython_3.8.10_test_ihr_float_threaded.dat" using 1:($2 * 1e3) t "Python 3.8.10" with lines, \
     "dat/benchmarks/CPython_3.14.2_test_ihr_float_threaded.dat" using 1:($2 * 1e3):(($3 - $4) * 1e3):(($3 + $4) * 1e3):($5 * 1e3) t "Python 3.14.2" with candlesticks whiskerbars 0.5, \
     "dat/benchmarks/CPython_3.14.2_test_ihr_float_threaded.dat" using 1:($2 * 1e3) t "Python 3.14.2" with lines

set output "images/perf_py_threaded_vs_single_py_min_all_pythons.png"   # choose the output device

plot "dat/benchmarks/CPython_3.8.10_test_ihr_float_threaded.dat" using 1:($2 * 1e3) t "Python 3.8.10" with linespoints, \
     "dat/benchmarks/CPython_3.9.13_test_ihr_float_threaded.dat" using 1:($2 * 1e3) t "Python 3.9.13" with linespoints, \
     "dat/benchmarks/CPython_3.10.11_test_ihr_float_threaded.dat" using 1:($2 * 1e3) t "Python 3.10.11" with linespoints, \
     "dat/benchmarks/CPython_3.11.9_test_ihr_float_threaded.dat" using 1:($2 * 1e3) t "Python 3.11.9" with linespoints, \
     "dat/benchmarks/CPython_3.12.7_test_ihr_float_threaded.dat" using 1:($2 * 1e3) t "Python 3.12.7" with linespoints, \
     "dat/benchmarks/CPython_3.13.0_test_ihr_float_threaded.dat" using 1:($2 * 1e3) t "Python 3.13.0" with linespoints, \
     "dat/benchmarks/CPython_3.14.2_test_ihr_float_threaded.dat" using 1:($2 * 1e3) t "Python 3.14.2" with linespoints

reset
