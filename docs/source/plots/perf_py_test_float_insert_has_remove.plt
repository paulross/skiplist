set title "Python Speed of insert, at, remove in 1m floats." noenhanced
set grid

set logscale x
set xlabel "Size of SkipList"

set ylabel "Time per Item (ns)"
# set logscale y
set yrange [0:2000]
# set yrange [0.001:10]
# set ytics 8,35,3

# set logscale y2
# set y2label "Bytes"
# set y2range [1:1e9]
# set y2tics

set pointsize 1
set datafile separator whitespace

# Some rate lines
latency = 0.04e-6
# 10,000 items per second
rate_10_000(x) = latency + x / 1e4
# 100,000 items per second
rate_100_000(x) = latency + x / 1e5
# 1,000,000 items per second
rate_1_000_000(x) = latency + x / 1e6
# 10,00,000 items per second
rate_10_000_000(x) = latency + x / 1e7
# 100,000,000 items per second
rate_100_000_000(x) = latency + x / 1e8
# 1,000,000,000 items per second
rate_1_000_000_000(x) = latency + x / 1e9

set key left
set key font ",9"
set boxwidth 0.2 relative

set terminal png size 1000,600           # choose the file format

set output "images/CPython_3.8.10_test_float_iar_begin_mid_end.png"   # choose the output device

plot "dat/CPython_3.8.10_test_float_iar_begin.dat" using 1:($2 * 1e9):(($3 - $4) * 1e9):(($3 + $4) * 1e9):($5 * 1e9) t "Python 3.8.10 (Begin)" with candlesticks whiskerbars 0.5, \
     "dat/CPython_3.8.10_test_float_iar_begin.dat" using 1:($2 * 1e9) t "Python 3.8.10 (Begin)" with lines, \
     "dat/CPython_3.8.10_test_float_iar_mid.dat" using 1:($2 * 1e9):(($3 - $4) * 1e9):(($3 + $4) * 1e9):($5 * 1e9) t "Python 3.8.10 (Mid-point)" with candlesticks whiskerbars 0.5, \
     "dat/CPython_3.8.10_test_float_iar_mid.dat" using 1:($2 * 1e9) t "Python 3.8.10 (Mid-point)" with lines, \
     "dat/CPython_3.8.10_test_float_iar_end.dat" using 1:($2 * 1e9):(($3 - $4) * 1e9):(($3 + $4) * 1e9):($5 * 1e9) t "Python 3.8.10 (End)" with candlesticks whiskerbars 0.5, \
     "dat/CPython_3.8.10_test_float_iar_end.dat" using 1:($2 * 1e9) t "Python 3.8.10 (End)" with lines

set output "images/CPython_3.14.2_test_float_iar_begin_mid_end.png"   # choose the output device

plot "dat/CPython_3.14.2_test_float_iar_begin.dat" using 1:($2 * 1e9):(($3 - $4) * 1e9):(($3 + $4) * 1e9):($5 * 1e9) t "Python 3.14.2 (Begin)" with candlesticks whiskerbars 0.5, \
     "dat/CPython_3.14.2_test_float_iar_begin.dat" using 1:($2 * 1e9) t "Python 3.14.2 (Begin)" with lines, \
     "dat/CPython_3.14.2_test_float_iar_mid.dat" using 1:($2 * 1e9):(($3 - $4) * 1e9):(($3 + $4) * 1e9):($5 * 1e9) t "Python 3.14.2 (Mid-point)" with candlesticks whiskerbars 0.5, \
     "dat/CPython_3.14.2_test_float_iar_mid.dat" using 1:($2 * 1e9) t "Python 3.14.2 (Mid-point)" with lines, \
     "dat/CPython_3.14.2_test_float_iar_end.dat" using 1:($2 * 1e9):(($3 - $4) * 1e9):(($3 + $4) * 1e9):($5 * 1e9) t "Python 3.14.2 (End)" with candlesticks whiskerbars 0.5, \
     "dat/CPython_3.14.2_test_float_iar_end.dat" using 1:($2 * 1e9) t "Python 3.14.2 (End)" with lines

set title "Python Speed of insert, at, remove in middle." noenhanced
set yrange [0:1400]

set output "images/CPython_test_float_iar_mid_all_pythons.png"   # choose the output device

plot "dat/CPython_3.8.10_test_float_iar_mid.dat" using 1:($2 * 1e9) t "Python 3.8.10" with linespoints, \
     "dat/CPython_3.9.13_test_float_iar_mid.dat" using 1:($2 * 1e9) t "Python 3.9.13" with linespoints, \
     "dat/CPython_3.10.11_test_float_iar_mid.dat" using 1:($2 * 1e9) t "Python 3.10.11" with linespoints, \
     "dat/CPython_3.11.9_test_float_iar_mid.dat" using 1:($2 * 1e9) t "Python 3.11.9" with linespoints, \
     "dat/CPython_3.12.7_test_float_iar_mid.dat" using 1:($2 * 1e9) t "Python 3.12.7" with linespoints, \
     "dat/CPython_3.13.0_test_float_iar_mid.dat" using 1:($2 * 1e9) t "Python 3.13.0" with linespoints, \
     "dat/CPython_3.14.2_test_float_iar_mid.dat" using 1:($2 * 1e9) t "Python 3.14.2" with linespoints, \

reset
