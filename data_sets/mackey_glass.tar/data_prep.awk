#nawk script to make it easy to use mg.dat with GPengine
#
# WBL 7 May 2004 $Revision: 1.13 $
#

BEGIN { 
}

(NF>=2) {
    data[$1] = $2;
}

END{
    for(i=0;i<=$1;i++) {
	printf("%4d ",i);
	for(j=14;j>=0;j--) {
	    x = i-j;
	    if(x<0) printf("       0 ");
	    else    printf("%8s ",data[x]);
	}
	printf("\n");
    }
}
