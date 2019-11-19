 	subroutine pf
c	pf = parabola fit; performs a chisquared fit of the data in stack1    
c	to parab=a(3)*(1.0-(nch - a(1))**2/a(2)**2)
c
	implicit undefined (a-z)
	include '../../stacks/F.h'
	include '../../main/F.h'
	include 'pf.df.h'
	integer mt
	parameter(mt=3)                  
	common /pfcom/a
	real*4 a(mt)
c
c	functions
c
	real*4 parab
	real*8 tran,rtran
	integer stfull
c
c	local variables
c
	character*4 title(3)
	integer i,nfrom,nto,nchan,nfree,npass,nsn1,m(mt)
	automatic i,nfrom,nto,nchan,nfree,npass,nsn1,m
	real*8 chisqr
	real*4 sigmaa(mt),deltaa(mt),sig(maxchn),diff
	real*4 chtest,fpeak,hfact,fwhm,fwerr,fperr,area 
	real*4 csigsv,rfrom,rto,rtemp
	automatic chisqr
	automatic sigmaa(mt),deltaa(mt),sig(maxchn),diff
	automatic chtest,fpeak,hfact,fwhm,fwerr,fperr,area 
	automatic csigsv,rfrom,rto,rtemp
	data title/ "ch", "Mhz", "km/s"/
	include 'pf.if.h'
c
c	This parabola fit program is from Bevington's GRIDLS (p212)
c	array definitions: a(j) = parabola parameters
c 	                   sigmaa(j)=one sigma errors in a(j)
c                          deltaa(j)=increments for a in chi-squared
c                          grid search
c	                   sig(i)=one sigma noise in data, for weighting
c
	include 'pf.ef.h'
c
	if(stfull(1).eq.0) call error("No data in stack 1")
	do 500 i=1,numpt1                       
500	sig(i)=rms1                              
	call pltoch(h1,h2,nfrom,nto)
c
c     Set reasonable starting values and increments
c
	i=(nfrom+nto)/2
	a(3)=stak1(i)
	a(1)=i
	i=(nto-nfrom)/2
	a(2)=i
	deltaa(3)=a(3)/5.0
	deltaa(1)=a(2)/5.0
	deltaa(2)=deltaa(1)
	nchan=nto-nfrom+1
	nfree=nchan-mt
	if(nfree.le.0) call error("not enough channels")
	npass=1
	chtest=rms1*nchan
c
c     Begin loop for following gradient of chisquared.
c
200	call gridls(stak1,sig,nfrom,nto,mt,1,deltaa,sigmaa,chisqr)
	npass=npass+1
	diff=(chtest-chisqr)/chisqr
	diff=abs(diff)
	if(diff.lt.0.001) go to 100
	chtest=chisqr
	go to 200
100	continue
c
c Carry out requested action on stack 1
c
	go to (280,260,270)iacton+1
c
c  Put parabolic function in stack 1 and overlay it on data plot.
c
260	continue
	if(pltd.eq.0) call combex("pl fhr:")
	do 265 i=1,numpt1
	stak1(i)=parab(i)
265	continue
	call combex("pl ov:oh:ovl:")
	go to 280
c
c  Subtract the parabolic function from the data in stack 1
c
270	do 275 i=1,numpt1
	stak1(i)=stak1(i)-parab(i)
275 	continue
	pltd=0
280	continue
c
c  Calculate stack header information
c
	hfact=rtran(1,3)
	fwhm1=a(2)*2.*hfact
	fwerr1=sigmaa(2)*2.*hfact
	fpeak1=tran(1,3,dble(a(1)))
	fperr1=sigmaa(1)*hfact
	ftmax1=a(3)
	fterr1=sigmaa(3)
	area1=2./3.*a(3)*fwhm1
	if(dpfl.ne.0) return
c
c  Calculate information for printing
c
	hfact=rtran(1,htype)
	fwhm=a(2)*2.*hfact
	fwerr=sigmaa(2)*2.*hfact
	fpeak=tran(1,htype,dble(a(1)))
	fperr=sigmaa(1)*hfact
	area=2./3.*a(3)*fwhm
c
c  Do we need a header?
c
	if(cmdsig.ne.csigsv) then
	csigsv=cmdsig
	rfrom=tran(1,htype,dble(nfrom))
	rto=tran(1,htype,dble(nto))
	if(rto.lt.rfrom) then
	rtemp=rfrom
	rfrom=rto
	rto=rtemp
	endif
	write(6,321)rfrom,rto,(title(htype),i=1,4)
	endif
321 	format("Parabolic fit from",f11.2," to",f11.2,a6/"scan # tpeak(k)"
     1  ,"err(k) center(",a4,") err fwzp(",a4,") err area(K",a4,
     2  ") chisq nfre")
	if(nstk1(1).lt.-32000) then
	nsn1=nstk1(3)
	else
	nsn1=nstk1(1)
	endif
	write(6,331)nsn1,ftmax1,fterr1,fpeak,fperr,fwhm,fwerr,area
     1  ,chisqr,nfree
331	format(i6,2f8.3,f11.3,f7.3,f8.3,f7.3,f10.4,f10.1,i4)
	end
c
c
	subroutine gridls(y,sigmay,nfrom,nto,nterms,mode,deltaa,
     &	sigmaa,chisqr)
c       Deltaa values are modified by the program.
c
	include '../../stacks/F.h'
	common /pfcom/a(3)
	real*4 a
c functions
	real*4 parab
	real*8 fchisq
c arguments
	integer nfrom,nto,nterms,mode
	real*4 y(1),sigmay(1),deltaa(1),sigmaa(1)
	real*8 chisqr
c local variables
	integer nchan,nfree,i,j
	real*4 free,fn,delta,save,yfit(maxchn)
	real*8 chisq1,chisq2,chisq3
	automatic nchan,nfree,i,j
	automatic free,fn,delta,save,yfit(maxchn)
	automatic chisq1,chisq2,chisq3
c
	nchan=nto-nfrom+1
11	nfree=nchan-nterms
	free=nfree
	if(free.le.0.) go to 100
20	do 90 j=1,nterms
c
c  Evaluate chi square at first two search points
c
21	do 22 i=nfrom,nto
22	yfit(i)=parab(i)    
23	chisq1=fchisq(stak1,sigmay,nfrom,nto,nfree,mode,yfit)
	fn=0.
	delta=deltaa(j)
41	a(j)=a(j)+delta
	do 43 i=nfrom,nto
43	yfit(i)=parab(i)
44	chisq2=fchisq(y,sigmay,nfrom,nto,nfree,mode,yfit)
45	if(chisq1-chisq2) 51,41,61
c
c   reverse direction if chisquared is increasing
c
51	delta=-delta
	a(j)=a(j)+delta
	do 54 i=nfrom,nto
54	yfit(i)=parab(i)    
	save=chisq1
	chisq1=chisq2
57	chisq2=save
c
c  Increment a(j) until chisq increases
c
61	fn=fn+1.
	a(j)=a(j)+delta
	do 64 i=nfrom,nto
64	yfit(i)=parab(i)    
	chisq3=fchisq(stak1,sigmay,nfrom,nto,nfree,mode,yfit)
66	if(chisq3.ge.chisq2) go to 81
71	chisq1=chisq2
	chisq2=chisq3
	go to 61
c
c  Find minimum of parabola defined by last three points
81	delta=delta*(1./(1.+(chisq1-chisq2)/(chisq3-chisq2))+0.5)
82	a(j)=a(j)-delta
83	sigmaa(j)=deltaa(j)*sqrt(2./(free*(chisq3-2.*chisq2+chisq1)))
84	deltaa(j)=deltaa(j)*fn/3.
90	continue
c
c  Evaluate fit and chisq for final parameters
91	do 92 i=1,nfrom,nto
92	yfit(i)=parab(i)    
93	chisqr=fchisq(stak1,sigmay,nfrom,nto,nfree,mode,yfit)
2000	format(2x,i3,f8.2)
100	return
	end
c
c  Function to calculate the current parabola for the given channel
c
	function parab(nx)    
	implicit undefined(a-z)
	real*4 parab
	integer nx
c
	common /pfcom/a(3)
	real*4 a
c local variables
	real*4 xi,q
	automatic xi,q
c
	xi=nx-a(1)
	q=(xi/a(2))**2
	if(q.ge.1.) then
		 parab=0.
	else
		parab=a(3)*(1.-q)                
	endif
	return
	end
        real*8 function fchisq(stak1,sigmay,nfrom,nto,nfree,mode,yfit)
c  Bevington p194
c
	implicit undefined(a-z)
c arguments
	integer nfrom,nto,nfree,mode
	real*4 stak1(1),sigmay(1),yfit(1)
c
        common /pfcom/a(3)
	real*4 a
c local variables
	integer i
	real*4 free
	real*8 chisq,weight
	automatic i,free,chisq,weight
c
11	chisq=0.d0
12	if(nfree.gt.0) go to 20
13	fchisq=0.
	go to 40
c
c  Accumulate chi squared
c
20	do 30 i=nfrom,nto
21	if(mode) 22,27,29
22	if(stak1(i)) 25,27,23
23	weight=1./stak1(i)
	go to 30
25	weight=1./(-stak1(i))
	go to 30
27	weight=1.
	go to 30
29	weight=1./sigmay(i)**2
30	chisq=chisq+weight*(stak1(i)-yfit(i))**2
c
c   Divide by number of degrees of freedom
c
31	free=nfree
32	fchisq=chisq/free
40	return
	end
