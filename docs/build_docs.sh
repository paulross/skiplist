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
./make_plots.sh
popd

# Visualisations, requires dot (graphviz) installed
pushd source/visualisations
python dot_convert.py
popd

# Doxygen, requires doxygen installed
doxygen SkipList.dox
# Doxygen PDF
pushd doxygen/latex/
make pdf
# PDF is source/ref/doxygen/latex/refman.pdf so rename it.
mv refman.pdf SkipList_doxygen.pdf
popd

# Sphinx
rm -rf build/
make html latexpdf

echo ""
echo "Finished!"
echo "Sphinx HTML is in build/index.html"
echo "Sphinx PDF is in build/latex/SkipList.pdf"
echo "Doxygen HTML is in doxygen/doxygen_html/index.html"
echo "Doxygen PDF is in doxygen/latex/SkipList_doxygen.pdf"
