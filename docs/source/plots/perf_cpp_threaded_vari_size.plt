set logscale x
set grid
set title "Threaded Insert and Remove"
set xlabel "Threads"
set ylabel "Time the same number of values values (ms)."
set pointsize 1
set datafile separator "\t"
# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "perf_height_size.dat" using 1:2 via a,b

set terminal png size 1000,600           # choose the file format
set output "images/perf_cpp_threaded_vari_size.png"   # choose the output device

plot "dat/perf_cpp_threaded_vari_size.dat" using 1:3 t "Threaded" with linespoints pt 1 lw 2

reset
