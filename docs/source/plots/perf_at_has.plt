set logscale x
set grid
set title "Speed of calling at(index) and has(value) for a skip list of 1 million doubles."
set xlabel "Index"
set ylabel "Time (ns)"
set yrange [0:300]
set pointsize 1
set datafile separator "\t"
# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "perf_at_has.dat" using 1:2 via a,b

set terminal png size 750,500           # choose the file format
set output "perf_at_has.png"   # choose the output device

plot "perf_at_has.dat" using 1:3 t "has()" with linespoints pt 2 lw 2, \
    "perf_at_has.dat" using 1:2 t "at()" with linespoints pt 1 lw 2
    
    
reset
