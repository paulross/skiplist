echo "Creating plots..."
gnuplot -p biased_coin_effect.plt
gnuplot -p perf_at_has.plt
gnuplot -p perf_height_size.plt
gnuplot -p perf_ins_rem_mid.plt
gnuplot -p perf_roll_med_odd_index_wins.plt
gnuplot -p perf_size_of.plt
gnuplot -p perf_index.plt
gnuplot -p perf_cpp_threaded_fixed_size.plt
gnuplot -p perf_cpp_threaded_vari_size.plt
gnuplot -p perf_cpp_threaded_vs_single.plt
gnuplot -p perf_roll_median_shared_memory.plt
gnuplot -p perf_roll_median_shared_memory_ratio.plt
echo "All plots done"
