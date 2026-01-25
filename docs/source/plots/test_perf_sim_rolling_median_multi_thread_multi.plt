set title "insert() + at() + remove() by Number of Threads." noenhanced
set grid

set logscale x
set xlabel "Number of Threads"
set xrange [0.4:256]

set ylabel "Time per Item (us)"
# set logscale y
set yrange [0:25]
# set yrange [0.001:10]
# set ytics 8,35,3

# set logscale y2
set y2label "Ration Multi/Single"
set y2range [0:5]
# set y2range [1:1e9]
set y2tics

set pointsize 1
set datafile separator whitespace

set key left
set key font ",9"
set boxwidth 0.2 relative

set terminal png size 1000,600           # choose the file format
set output "images/test_perf_sim_rolling_median_multi_thread_multi.png"   # choose the output device

# NOTE: Compute actual time as $4 * $? / $9, e.g. $4 * $5 / $9

plot "dat/test_perf_sim_rolling_median_multi_thread_single.dat" using 3:(1e6 * ($5 - $6) * $4 / $9):(1e6 * $7 * $4 / $9):(1e6 * $8 * $4 / $9):(1e6 * ($5 + $6) * $4 / $9) t "Single-threaded" with candlesticks whiskerbars 0.5, \
    "dat/test_perf_sim_rolling_median_multi_thread_multi.dat" using 3:(1e6 * ($5 - $6) * $4 / $9):(1e6 * $7 * $4 / $9):(1e6 * $8 * $4 / $9):(1e6 * ($5 + $6) * $4 / $9) t "Multi-threaded" with candlesticks whiskerbars 0.5, \
    "dat/test_perf_sim_rolling_median_multi_thread_multi.dat" using 3:($7 * $4 / $9 / (10 * 0.000020417 / 200)) axes x1y2 t "Ratio Multi/Single [right]" with lines


reset
