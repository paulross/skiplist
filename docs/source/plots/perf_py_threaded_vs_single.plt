set grid
set title "Python Single and Multi Threaded insert/at/remove"

#set logscale x
set xlabel "Number of Threads (0 is not threaded)"

#set logscale y
set ylabel "Time for insert/at/ remove (µs)."
set yrange [0:]
# set ytics 0,200,1500

# set y2label "Ratio: compiled threaded / compiled single"
# set y2range [0.9:1.275]
# set y2tics 0.9,0.05,1.275

set pointsize 1
set datafile separator whitespace
# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "perf_height_size.dat" using 1:2 via a,b

set terminal png size 1000,600           # choose the file format
set output "images/perf_py_threaded_vs_single.png"   # choose the output device

plot "dat/perf_py_threaded_vs_single.dat" using 1:($2*1000) t "Execution Time" with linespoints pt 1 lw 2

reset
