set title "Insert+Remove at Beginning, Middle and End." noenhanced

# First line specification refers to major grid lines in both x and y, the second to minor grid lines in x and y.
set grid xtics mxtics ytics mytics linetype -1 linewidth 1, linetype 0 linewidth 1

set logscale x
set xlabel "Size of SkipList"

set ylabel "Time per Item (ns)"
# set logscale y
set yrange [200:800]
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
set output "images/perf_test_double_insert_remove_value.png"   # choose the output device

# NOTE: Compute actual time as $4 * $? / $9, e.g. $4 * $5 / $9

# Use the same initial value, 275, for consistency.
# Then take minimum value at last point: 1048576

plot "dat/perf_test_double_insert_remove_value_begin.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) t "Beginning" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_insert_remove_value_begin.dat" using 3:(275 + (404.6 - 275) / 20 * log($3) / log(2)) t "Beginning : 275 + 6.48 * log2(x)" with lines lw 2, \
        "dat/perf_test_double_insert_remove_value_mid.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) t "Middle" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_insert_remove_value_mid.dat" using 3:(275 + (517.93 - 275) / 20 * log($3) / log(2)) t "Middle: 275 + 12.1 * log2(x)" with lines lw 2, \
        "dat/perf_test_double_insert_remove_value_end.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) t "End" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_insert_remove_value_end.dat" using 3:(275 + (447.5 - 275) / 20 * log($3) / log(2)) t "End: 275 + 8.63 * log2(x)" with lines lw 2
reset
