set grid
set title "Comparing Compilation for Single and Multi Threaded"

#set logscale x
set xlabel "Number of times the task is repeated"

#set logscale y
set ylabel "Time for insert and remove 16384 values (ms)."
set yrange [0:1500]
set ytics 0,200,1500

set y2label "Ratio: compiled threaded / compiled single"
set y2range [0.9:1.275]
set y2tics 0.9,0.05,1.275

set pointsize 1
set datafile separator "\t"
# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "perf_height_size.dat" using 1:2 via a,b

set terminal png size 1000,700           # choose the file format
set output "images/perf_cpp_threaded_vs_single.png"   # choose the output device

plot "dat/perf_cpp_threaded_vs_single.dat" using 1:2 t "Compiled Single Threaded" with linespoints pt 1 lw 2, \
    "dat/perf_cpp_threaded_vs_single.dat" using 1:3 t "Compiled Multi Threaded" with linespoints pt 2 lw 2, \
    "dat/perf_cpp_threaded_vs_single.dat" using 1:($3/$2) axes x1y2 t "Ratio (right scale)" with linespoints pt 3 lw 2

reset
