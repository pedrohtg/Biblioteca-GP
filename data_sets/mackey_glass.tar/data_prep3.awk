#nawk script to make it easy to use mg.dat with GProc-1.8b
#
# WBL 7 May 2004 $Revision: 1.00 $
# WBL  5 Jul 2004 For GProc on sonic
# WBL 10 May 2004 collect history back 128, 64, 32,...1
#

BEGIN { 
	for(j=128;j>0.5;j=j/2) {
	    if(j<1) j=0;
	    printf("%3s",sprintf("D%d",j));
	    if(j!=16) printf(" ");
	}
	printf("Out\n");
}

(NF>=2) {
    data[$1] = $2;
    last = $1;
}

END{
    for(i=0;i<=last;i++) {
#	printf("%4d ",i);
	for(j=128;j>=0.5;j=j/2) {
	    if(j<1) j=0;
	    x = i-j;
	    if(x<0) printf("  0 ");
	    else    printf("%3s ",data[x]);
	}
	printf("\n");
    }
}
