#!/bin/bash
# Build all the documentation from scratch.
#
# Other references:
# https://kvz.io/bash-best-practices.html
# https://bertvv.github.io/cheat-sheets/Bash.html

set -o errexit  # abort on nonzero exitstatus
set -o nounset  # abort on unbound variable
set -o pipefail # don't hide errors within pipes

# Plots, requires gnuplot installed
pushd source/plots
echo "---- START Creating gnuplot plots"
./make_plots.sh
echo "---- DONE  Creating gnuplot plots"
popd

# Visualisations, requires dot (graphviz) installed
pushd source/visualisations
echo "---- START Creating DOT plots"
# Very noisy so /dev/null
python dot_convert.py > /dev/null
echo "---- DONE  Creating DOT plots"
popd

# Doxygen, requires doxygen installed, very noisy so /dev/null
echo "---- START Creating Doxygen output"
rm -rf doxygen/
doxygen SkipList.dox > /dev/null
echo "---- DONE  Creating Doxygen output"
# Doxygen PDF
pushd doxygen/latex/
# Very noisy so /dev/null
echo "---- START Creating Doxygen PDF output"
make pdf > /dev/null
echo "---- DONE  Creating Doxygen PDF output"
# PDF is source/ref/doxygen/latex/refman.pdf so rename it.
mv refman.pdf SkipList_doxygen.pdf
popd

# Sphinx
echo "---- START Sphinx output"
rm -rf build/
make html
echo "---- DONE  Sphinx html"
# Very noisy so /dev/null
make latexpdf > /dev/null
echo "---- DONE  Sphinx PDF"

echo ""
echo "Finished!"
echo "Sphinx HTML is in build/index.html"
echo "Sphinx PDF is in build/latex/SkipList.pdf"
echo "Doxygen HTML is in doxygen/doxygen_html/index.html"
echo "Doxygen PDF is in doxygen/latex/SkipList_doxygen.pdf"
