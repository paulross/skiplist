SkipList Documentation
======================

There are a number of generated files here that need to be created before running Sphinx.

GNUPlot Files
-------------

These can be created by:

$ cd <skiplist>/docs/source/plots
$ ./make_plots.sh 
Creating plots...
All plots done

DOT Visualisations
------------------

$ cd <skiplist>/docs/source/visualisations
$ python dot_convert.py 
Converting: doc_insert.dot
Converting: doc_insert_12-5_percent.dot
Converting: doc_insert_25_percent.dot
Converting: doc_insert_75_percent.dot
Converting: doc_insert_remove.dot
Converting: doc_insert_remove_12-5_percent.dot
Converting: doc_insert_remove_25_percent.dot
Converting: doc_insert_remove_75_percent.dot
Converting: doc_insert_remove_repeat.dot
Converting: doc_simple.dot

Running Sphinx
--------------

$ cd <skiplist>/docs/
$ $ make html
sphinx-build -b html -d build/doctrees   source build/html
Running Sphinx v1.2.2
loading pickled environment... done
building [html]: targets for 1 source files that are out of date
updating environment: 0 added, 1 changed, 0 removed
reading sources... [100%] performance                                                                                                                                     
looking for now-outdated files... none found
pickling environment... done
checking consistency... done
preparing documents... done
writing output... [100%] performance                                                                                                                                      
writing additional files... genindex search
copying images... [100%] plots/perf_index.svg                                                                                                                             
copying static files... done
copying extra files... done
dumping search index... done
dumping object inventory... done
build succeeded.
