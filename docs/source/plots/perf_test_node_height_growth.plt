set title "Height of Head Node by Skiplist Length." noenhanced

# First line specification refers to major grid lines in both x and y, the second to minor grid lines in x and y.
set grid xtics mxtics ytics mytics linetype -1 linewidth 1, linetype 0 linewidth 1

set logscale x
set xlabel "Size of SkipList"
set xrange [1:1e6]

set ylabel "Height of Head Node"
# set logscale y
# set yrange [0:1000]
# set yrange [0.001:10]
# set ytics 8,35,3

# set logscale y2
# set y2label "Bytes"
# set y2range [1:1e9]
# set y2tics

set pointsize 1
set datafile separator whitespace

set key left
set key font ",9"
set boxwidth 0.2 relative

set terminal png size 1000,600           # choose the file format
set output "images/perf_test_node_height_growth.png"   # choose the output device

plot "dat/perf_test_node_height_growth.dat" using 3:($5 - $6):7:8:($5 + $6) t "Head Node Height" with candlesticks whiskerbars 0.5, \
    "dat/perf_test_double_insert_remove_value_end.dat" using 3:(1 + log($3) / log(2)) t "Height = 1 + log2(x)" with lines

reset
