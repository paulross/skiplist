echo "Creating plots..."
gnuplot -p biased_coin_effect.plt
gnuplot -p perf_at_has.plt
gnuplot -p perf_height_size.plt
gnuplot -p perf_ins_rem_mid.plt
gnuplot -p perf_roll_med_odd_index_wins.plt
gnuplot -p perf_size_of.plt
echo "All plots done"
