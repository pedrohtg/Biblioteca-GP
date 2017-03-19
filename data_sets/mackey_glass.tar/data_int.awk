#nawk script to make it easy to use mg.dat with GPengine
#
# WBL 7 May 2004 $Revision: 1.00 $
#

BEGIN { 
}

(NF>=2) {
  x = int($2*128+0.5);
  printf("%4d %3d\n",$1,x);
}

END{
}
