#WBL cs.mun.ca 6 may 2004

set term postscript
set output "mg.ps"
set title "Mackey Glass benchmark"

plot "mg.dat"   w line

#pause -1

set title "Power spectrum of Mackey Glass benchmark"

set logscale y

f = 24/1201.0;

#set arrow from  f,0  to  f,1000000 nohead
#set label  "24/1201" at  f,120000 center

plot "mg.fft" u 1:4 w line

#pause -1

set xrange [0:.025]
set nologscale y

#set arrow from  f,0  to  f,1000000 nohead
set label  "24/1201" at  f,120000 center

plot "mg.fft" u 1:4 w linesp

#pause -1

