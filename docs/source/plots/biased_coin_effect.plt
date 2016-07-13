set grid
set title "Effect of varying p() on various operations, normalised to p(0.5)."

set xlabel "p() of 'heads'"
set xrange [0:1]
set xtics 0,0.1,1

set ylabel "Relative to p(0.5)"
set yrange [0:6]
set pointsize 1
set datafile separator "\t"
# Curve fit
#cost(x) = a + (b / (x/1024))
#fit cost(x) "biased_coin_effect.dat" using 1:2 via a,b

set terminal svg size 750,500           # choose the file format
set output "biased_coin_effect.svg"   # choose the output device

# pt is pointtype
# lt is linetype
plot "biased_coin_effect.dat" using 1:7  t "at()" with linespoints pt 1 lt 1 lw 2, \
    "biased_coin_effect.dat" using 1:8 t "has()" with linespoints pt 2 lt 2 lw 2, \
    "biased_coin_effect.dat" using 1:9 t "insert()+at()+remove()" with linespoints pt 3 lt 3 lw 2, \
    "biased_coin_effect.dat" using 1:10 t "rolling median" with linespoints pt 7 lt 7 lw 2

#reset

set title "Effect of varying p() on size_of() the skip list <double>."
set ylabel "Size in bytes / node"
set yrange [0:250]

set terminal svg size 750,500           # choose the file format
set output "biased_coin_effect_size_of.svg"   # choose the output device
plot "biased_coin_effect.dat" using 1:($6 * 8.0) t "size_of()" with linespoints pt 12 lt 12 lw 2

reset
