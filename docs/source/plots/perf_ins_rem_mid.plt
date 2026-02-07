set logscale x

# First line specification refers to major grid lines in both x and y, the second to minor grid lines in x and y.
set grid xtics mxtics ytics mytics linetype -1 linewidth 1, linetype 0 linewidth 1

set title "Speed of calling insert(value) + remove(value) where the value is in the middle."
set xlabel "Size of skip list"
set ylabel "Total time for insert() + remove() (ns)"
set yrange [0:1600]

# set logscale y2
set y2label "insert() + remove() per second"
set y2range [800000:2400000]
set y2tics

set pointsize 1
set datafile separator "\t"
# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "perf_at_has.dat" using 1:2 via a,b

set terminal png size 1000,600           # choose the file format
set output "images/perf_ins_rem_mid.png"   # choose the output device

plot "dat/perf_ins_rem_mid.dat" using 1:2 t "insert() + remove() (ns)" with linespoints axes x1y1 pt 2 lw 2, \
     "dat/perf_ins_rem_mid.dat" using 1:3 t "insert() + remove() per second" with linespoints  axes x1y2 pt 1 lw 2
    
    
reset
