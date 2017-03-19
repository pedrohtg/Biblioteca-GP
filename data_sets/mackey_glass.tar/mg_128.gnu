#WBL cs.mun.ca 10 may 2004

set term postscript
set output "mg_128.ps"
set title "First 200 points of Mackey Glass benchmark"

set xrange [0:200]
set nokey

plot \
"mg_int_128.dat" u 1:2 w line,\
"mg_int_128.dat" u 1:3 w line,\
"mg_int_128.dat" u 1:4 w line,\
"mg_int_128.dat" u 1:5 w line,\
"mg_int_128.dat" u 1:6 w line,\
"mg_int_128.dat" u 1:7 w line,\
"mg_int_128.dat" u 1:8 w line,\
"mg_int_128.dat" u 1:9 w line,\
"mg_int_128.dat" u 1:10 w linesp

#pause -1

