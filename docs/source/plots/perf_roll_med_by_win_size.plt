set title "Rolling Median of 1m doubles." noenhanced
set grid

set logscale x
set xlabel "Size of Rolling Median Window"

set ylabel "Time to Compute the Rolling Median (s)"
# set logscale y
#set yrange [200:800]
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

set terminal png size 1000,700           # choose the file format
set output "images/perf_roll_med_by_win_size.png"   # choose the output device

# NOTE: Compute actual time as $4 * $? / $9, e.g. $4 * $5 / $9

plot "dat/perf_roll_med_by_win_size.dat" using 3:(1e0 * ($5 - $6) * $4 / $9):(1e0 * $7 * $4 / $9):(1e0 * $8 * $4 / $9):(1e0 * ($5 + $6) * $4 / $9) t "Rolling Median" with candlesticks whiskerbars 0.5, \
        "dat/perf_roll_med_by_win_size.dat" using 3:(0.3 + sqrt($3) / 300) t "Rolling Median Typ. 0.3 + sqrt(window length) / 300" with lines
reset
