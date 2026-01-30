set title "Speed of at(index), has(value) and index(value) in 1m doubles." noenhanced
set grid

set logscale x
set xlabel "Position in the SkipList of 1m doubles"

set ylabel "Time per Item (ns)"
# set logscale y
set yrange [0:800]
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
set output "images/perf_test_double_at_has_index.png"   # choose the output device

# NOTE: Compute actual time as $4 * $? / $9, e.g. $4 * $5 / $9

plot "dat/perf_test_double_at_1m.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) t "at(index)" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_at_1m.dat" using 3:(1e9 * $7 * $4 / $9) t "at(index) Typ. 58 + 23 * log2(x)" with lines, \
        "dat/perf_test_double_has_1m.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) t "has(value)" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_has_1m.dat" using 3:(1e9 * $7 * $4 / $9) t "has(value) Typ. 133 + 30 * log2(x)" with lines, \
        "dat/perf_test_double_index_1m.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) t "index(value)" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_index_1m.dat" using 3:(1e9 * $7 * $4 / $9) t "index(value) Typ. 183 + 28 * log2(x)" with lines
reset
