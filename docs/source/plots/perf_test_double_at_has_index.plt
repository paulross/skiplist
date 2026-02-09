set title "Speed of at(index), has(value) and index(value) in 1m doubles." noenhanced

# First line specification refers to major grid lines in both x and y, the second to minor grid lines in x and y.
set grid xtics mxtics ytics mytics linetype -1 linewidth 1, linetype 0 linewidth 1

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

# NOTE: Compute actual time as $4 * $? / $9, e.g. $4 * $5 / $9

# Combined plot, probably too much data.
set title "Speed of at(index), has(value) and index(value) in 1m doubles." noenhanced
set output "images/perf_test_double_at_has_index.png"   # choose the output device

plot "dat/perf_test_double_at_1m.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) t "at(index)" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_at_1m.dat" using 3:(1e9 * $7 * $4 / $9) t "at(index) minimum" with lines lw 2, \
        "dat/perf_test_double_at_1m.dat" using 3:(30 + 10 * log($3) / log(2)) t "at(index) Typ. 30 + 10 * log2(x)" with lines lw 2, \
        "dat/perf_test_double_has_1m.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) t "has(value)" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_has_1m.dat" using 3:(1e9 * $7 * $4 / $9) t "has(value) minimum" with lines lw 2, \
        "dat/perf_test_double_has_1m.dat" using 3:(100 + 10 * log($3) / log(2)) t "has(value) Typ. 100 + 10 * log2(x)" with lines lw 2, \
        "dat/perf_test_double_index_1m.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) t "index(value)" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_index_1m.dat" using 3:(1e9 * $7 * $4 / $9) t "index(value) minimum" with lines lw 2, \
        "dat/perf_test_double_index_1m.dat" using 3:(160 + 15 * log($3) / log(2)) t "index(value) Typ. 160 + 15 * log2(x)" with lines lw 2

# Separate plots.

set title "Speed of at(index) in 1m doubles." noenhanced
set output "images/perf_test_double_at.png"   # choose the output device

plot "dat/perf_test_double_at_1m.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) t "at(index)" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_at_1m.dat" using 3:(1e9 * $7 * $4 / $9) t "at(index) minimum" with lines lw 2, \
        "dat/perf_test_double_at_1m.dat" using 3:(30 + 15 * log($3) / log(2)) t "at(index) Typ. 30 + 15 * log2(x)" with lines lw 2

set title "Speed of has(value) in 1m doubles." noenhanced
set output "images/perf_test_double_has.png"   # choose the output device

plot "dat/perf_test_double_has_1m.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) t "has(value)" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_has_1m.dat" using 3:(1e9 * $7 * $4 / $9) t "has(value) minimum" with lines lw 2, \
        "dat/perf_test_double_has_1m.dat" using 3:(100 + 15 * log($3) / log(2)) t "has(value) Typ. 100 + 15 * log2(x)" with lines lw 2

set title "Speed of index(value) in 1m doubles." noenhanced
set output "images/perf_test_double_index.png"   # choose the output device

plot "dat/perf_test_double_index_1m.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) t "index(value)" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_index_1m.dat" using 3:(1e9 * $7 * $4 / $9) t "index(value) minimum" with lines lw 2, \
        "dat/perf_test_double_index_1m.dat" using 3:(160 + 15 * log($3) / log(2)) t "index(value) Typ. 160 + 15 * log2(x)" with lines lw 2

reset
