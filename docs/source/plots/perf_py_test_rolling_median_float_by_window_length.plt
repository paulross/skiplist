set title "Python 3.14 Rolling Median of 1m floats." noenhanced

# First line specification refers to major grid lines in both x and y, the second to minor grid lines in x and y.
set grid xtics mxtics ytics mytics linetype -1 linewidth 1, linetype 0 linewidth 1

set logscale x
set xlabel "Size of Rolling Median Window"

set ylabel "Time per Operation (ns)"
set logscale y
# set yrange [0:2000]
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

# perf_py_test_rolling_median_float_by_window_length
# CPython_3.14.2_test_rolling_median_float_by_window_length.dat

# Length is 1e6
# Number of operations is 1e6 - $1
# To get ns/value multiplier is 1e9
# So to get ns/operation (1e9 / (1e6 - $1))
#
# Example minimum in .dat file ($7) is 1.807299542 with window size of 65536
# Number of operations = 1e6 - 65536 = 934464
# Time per operation is: 1.807299542 * 1e9 / (1e6 - 65536)
# 1934.0494037223477

set terminal png size 1000,600           # choose the file format

set output "images/benchmarks/CPython_3.14.2_test_rolling_median_float_by_window_length.png"   # choose the output device

plot "dat/benchmarks/CPython_3.14.2_test_rolling_median_float_by_window_length.dat" using 1:($2 * 1e9 / (1e6 - $1)):(($3 - $4) * 1e9 / (1e6 - $1)):(($3 + $4) * 1e9 / (1e6 - $1)):($5 * 1e9 / (1e6 - $1)) t "Python 3.14.2 Rolling Median" with candlesticks whiskerbars 1.0, \
     "dat/benchmarks/CPython_3.14.2_test_rolling_median_float_by_window_length.dat" using 1:($2 * 1e9 / (1e6 - $1)) t "Minimum" with lines linewidth 2.0

reset
