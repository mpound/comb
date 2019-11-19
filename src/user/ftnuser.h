
	integer ,stklen,stkuse,maxchn,notfs
c stklen = 512 (header) + maxchn * sizeof(real) + sizeof(nstkx)
	parameter(stklen=8960)
	parameter(stkuse=512)
	parameter(maxchn=2048)
	parameter(notfs=-9999)
	common /stk/ numpt1,nver1,nprc1,nplc1,onstk1,nwts1,locwt1(15),
     1  ibsln1,kuse1(32),nstk1(32),nlist1,indx1,isdum1(38),numst1,
     2  fwid1,expch1,refch1,vlsr1,exwid1,wght1(15),factt1,time1,
     3  pwrav1,pwrer1,rms1,tmax1,arerr1,area1,vcent1,fwhm1,fwerr1,
     4  fpeak1,fperr1,ftmax1,fterr1,rmval1,ra1,dec1,l1,b1,dra1,ddec1,
     5  dl1,db1,dx1,dy1,odra1,oddec1,epoch1,vpeak1,rsdm1(12),freq1,
     6  stak1(maxchn),nstkx1(128)
	integer*2 numpt1,nver1,nprc1,nplc1,onstk1,nwts1,locwt1,
     1  ibsln1,kuse1,nstk1,nlist1,indx1,isdum1,nstkx1
	integer*4 numst1
	real*4 fwid1,expch1,refch1,vlsr1,exwid1,wght1,factt1,time1,
     1  pwrav1,pwrer1,rms1,tmax1,arerr1,area1,vcent1,fwhm1,fwerr1,
     2  fpeak1,fperr1,ftmax1,fterr1,rmval1,ra1,dec1,l1,b1,dra1,ddec1,
     3  dl1,db1,dx1,dy1,odra1,oddec1,epoch1,vpeak1,rsdm1,stak1
	real*8 freq1
	save /stk/
