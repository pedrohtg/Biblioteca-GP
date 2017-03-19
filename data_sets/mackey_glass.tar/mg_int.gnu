#WBL cs.mun.ca 7 may 2004

#set term postscript
#set output "mg.ps"
set title "Mackey Glass benchmark"

plot "mg.dat"   w linesp, "mg_int.dat" u 1:($2/128.0) w line

pause -1

