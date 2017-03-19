#nawk script to make it easy to use mg.dat with GPengine
#
# WBL 7 May 2004 $Revision: 1.00 $
# WBL 10 May 2004 collect history back 128, 64, 32,...1
#

BEGIN { 
}

(NF>=2) {
    data[$1] = $2;
}

END{
    for(i=0;i<=$1;i++) {
	printf("%4d ",i);
	for(j=128;j>=0.5;j=j/2) {
	    if(j<1) j=0;
	    x = i-j;
	    if(x<0) printf("  0 ");
	    else    printf("%3s ",data[x]);
	}
	printf("\n");
    }
}
