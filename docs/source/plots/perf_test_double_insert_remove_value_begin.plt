set title "perf_test_double_insert_remove_value_begin()." noenhanced
set grid

set logscale x
set xlabel "Size of SkipList"

set logscale y
set ylabel "Time per Item (ns)"
# set yrange [8:35]
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

set terminal png size 700,400           # choose the file format
set output "images/perf_test_double_insert_remove_value_begin.png"   # choose the output device

# NOTE: Compute actual time as $4 * $? / $9, e.g. $4 * $5 / $9

plot "dat/perf_test_double_insert_remove_value_begin.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) \
        t "Beginning" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_insert_remove_value_mid.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) \
        t "Middle" with candlesticks whiskerbars 0.5, \
        "dat/perf_test_double_insert_remove_value_end.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) \
        t "End" with candlesticks whiskerbars 0.5#,\
    "dat/test_py_dict_to_cpp_std_unordered_map_vector_char_multiple_std_vector_char_128.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) \
        t "Python dict, bytes length 128" with candlesticks whiskerbars 0.5,\
    "dat/test_py_dict_to_cpp_std_unordered_map_vector_char_multiple_std_vector_char_1024.dat" using 3:(1e9 * ($5 - $6) * $4 / $9):(1e9 * $7 * $4 / $9):(1e9 * $8 * $4 / $9):(1e9 * ($5 + $6) * $4 / $9) \
        t "Python dict, bytes length 1024" with candlesticks whiskerbars 0.5,\
    "dat/test_py_dict_to_cpp_std_unordered_map_vector_char_multiple_std_vector_char_2.dat" using 3:(rate_1_000_000($3) * 1e9) t sprintf("Guide: %.3f µs + 1m objects/s", latency*1e9) with lines dashtype 2 lw 2, \
    "dat/test_py_dict_to_cpp_std_unordered_map_vector_char_multiple_std_vector_char_2.dat" using 3:(rate_10_000_000($3) * 1e9) t sprintf("Guide: %.3f µs + 10m objects/s", latency*1e9) with lines dashtype 2 lw 2

reset
