set grid
set title "Sequential vs Threaded Insert and Remove"

#set logscale x
set xlabel "Task repeat count, either sequential or threaded"

#set logscale y
set ylabel "Time for insert and remove 16384 values (ms)."
set yrange [0:15000]
set ytics 0,2000,15000

set y2label "Ratio: threaded / sequential"
set y2range [0:15]
set y2tics 0,2,15

set pointsize 1
set datafile separator "\t"
# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "perf_height_size.dat" using 1:2 via a,b

set terminal svg size 750,500           # choose the file format
set output "perf_cpp_threaded_fixed_size.svg"   # choose the output device

plot "perf_cpp_threaded_fixed_size.dat" using 1:2 t "Sequential" with linespoints pt 1 lw 2, \
    "perf_cpp_threaded_fixed_size.dat" using 1:3 t "Threaded" with linespoints pt 2 lw 2, \
    "perf_cpp_threaded_fixed_size.dat" using 1:($3/$2) axes x1y2 t "Ratio (right scale)" with linespoints pt 3 lw 2

reset
