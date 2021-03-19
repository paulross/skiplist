set logscale x
set grid
set title "Size complexity of a skip list of double."
set xlabel "Index"

# set logscale y
set ylabel "Bytes/Node"
# set yrange [8:35]
# set ytics 8,35,3

# set logscale y2
# set y2label "Bytes"
# set y2range [1:1e9]
# set y2tics

set pointsize 1
set datafile separator "\t"
# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "perf_size_of.dat" using 1:2 via a,b

set terminal png size 750,500           # choose the file format
set output "perf_size_of.png"   # choose the output device

plot "perf_size_of.dat" using 1:3 t "Bytes/Node <double>" with linespoints axes x1y1 pt 5 lw 2#, \
    "perf_size_of.dat" using 1:5 t "Factor * sizeof(char)" with linespoints axes x1y1 pt 12 lw 2#,
    # "perf_size_of.dat" using 1:3 t "Memory usage (bytes)" with linespoints axes x1y2 pt 15 lw 2
    
reset
