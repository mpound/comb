	subroutine gf
c   gf=Gaussian Fit performs a chi**2 fit of the data in stack 1 to
c   		gauss = a(1)*exp(-.5*((n-a(2))/a(3))**2)
c
	implicit undefined(a-z)
	include '../../stacks/F.h'
	include '../../main/F.h'
	common /gfcom/a(3),sigsq
	include 'gf.df.h'
	real*4 widftr
	parameter(widftr=2.35482)
	integer mt
	parameter(mt=3)

c functions
	real gauss
	real*8 chisq,tran,rtran,wt
	integer stfull

c local variables
	character*4 title(3)
	integer csigsv
	integer i,j,k,nfrom,nto,nchan,nfree,
     *		nterms,npass,nsn1,m(mt)
	automatic i,j,k,nfrom,nto,nchan,nfree,
     *		nterms,npass,nsn1,m

	real*4 a(mt),alpha(mt,mt),beta(mt),deriv(mt),aa(mt),sigma(mt),
     *		anorm(mt,mt),
     *		flamda,gfun,sigsq,z,z2,
     *		hfact,fwhm,fwerr,fpeak,fperr,area
	automatic alpha,beta,deriv,aa,sigma,anorm,
     *		flamda,gfun,z,z2,
     *		hfact,fwhm,fwerr,fpeak,fperr,area

      double precision array(mt,mt),chisqr,chiold
     *	,rfrom,rto,rtemp
      automatic array,chisqr,chiold
     *	,rfrom,rto,rtemp

	data title/" ch ","Mhz ","Km/s"/
	include 'gf.if.h'

c This Gaussian fit program is taken from Bevington's CURFIT (prog 11-5).
c  array definitions: a(j)=Gaussian parameter (amplitude, mean, & dispersion)
c		     aa(j)=copy of a(j) used in error analysis
c		  sigma(j)=1 sigma error in a(j)
c		array(j,k)=modified curvature matrix and its inverse.
c		  deriv(j)=partial derivatives of current Gaussian at chan i
c

	include 'gf.ef.h'
	if(stfull(1).eq.0) call error("No data in stack 1")
	call pltoch(h1,h2,nfrom,nto)
	
c  set reasonable starting values
	i=(nfrom+nto)/2
	a(1)=stak1(i)
	a(2)=i
      do 5 i=nfrom,nto
      if(a(1).ge.stak1(i)) go to 5
      a(1) = stak1(i)
      a(2) = i
    5 continue

c Widfl will =0 if a width was given. In that case the width should not be
c varied.
	if(widfl.ne.0) then
		a(3)=(nto-nfrom+1)/(2.*widftr)
		nterms=3
	else
		a(3)=(tran(htype,1,width)-tran(htype,1,0.d0))/widftr
		sigma(3)=0.
		nterms=2
	endif
      nchan=nto-nfrom+1
      nfree=nchan-nterms
	if(nfree.le.0) call error("not enough channels")
	if(rms1.ne.0.) then
		sigsq=rms1**2
	else
		sigsq=4./(fwid1*wt(int(a(2)),1))
c This gives the expected rms**2.
	endif
	npass=1
	flamda=.001


c get chisq for the initial guess.
	chisqr=chisq(nfrom,nto)

c	write(6,100)npass,flamda,a,chisqr

c This is the beginning of the main loop for following the gradient of chisq.
c save current chisquare and clear the alpha and beta matrices.
20	chiold=chisqr
	call clear(beta,mt*4)
	call clear(alpha,mt*mt*4)
	call move(a,aa,mt*4)

c evaluate the alpha and beta matrices.
	do 50 i=nfrom,nto
c compute the current gaussian function and its derivatives.
		z=(i-a(2))/a(3)
		z2=z*z
		if(z2.gt.16) then
			call clear(deriv,mt*4)
			gfun=0.
		else
			deriv(1)=exp(-0.5*z2)
			gfun=a(1)*deriv(1)
			deriv(1)=deriv(1)
			deriv(2)=gfun*z/a(3)
			deriv(3)=deriv(2)*z
		endif
c add to beta and alpha matrices
		do 46 j=1,nterms
			beta(j)=beta(j)+(stak1(i)-gfun)*deriv(j)
			do 45 k=1,j
				alpha(j,k)=alpha(j,k)+deriv(j)*deriv(k)
45			continue
46		continue
50	continue

c construct and invert modified curvature matrix.
71	do 74 j=1,nterms
		do 73 k=1,j-1
			anorm(j,k)=sqrt(alpha(j,j)*alpha(k,k))
			if(anorm(j,k).eq.0) then
				anorm(j,k)=1e-20
				if(alpha(j,j).eq.0) alpha(j,j)=1e-10
				if(alpha(k,k).eq.0) alpha(k,k)=1e-10
			endif
			anorm(k,j)=anorm(j,k)
			array(j,k)=alpha(j,k)/anorm(j,k)
			array(k,j)=array(j,k)
73		continue
		array(j,j)=1.+flamda
		anorm(j,j)=alpha(j,j)
74	continue

c	write(6,101)array
c101	format(3(3g15.4/)/)

	call matinv(array,nterms)

c Update the a coeff by adding alpha*beta
	do 84 j=1,nterms
		a(j)=aa(j)
		do 83 k=1,nterms
			a(j)=a(j)+beta(k)*array(j,k)/anorm(j,k)
83		continue
84	continue

c if chisquare increased, increase flamda and try again.
	chisqr=chisq(nfrom,nto)

c	write(6,100)npass,flamda,a,chisqr
100	format(i3,e10.2,3f9.4,f12.4)

	if(chisqr.gt.chiold)then
		if(flamda.gt.1000.)goto 120
		flamda=10.*flamda
		goto 71
	endif
		
c end of main loop.
	if(a(2).lt.nfrom.or.a(2).gt.nto.or.a(3).gt.nto-nfrom) goto 120
	if(chiold.gt.chisqr+.001.or.npass.lt.3) then
		npass=npass+1
		flamda=flamda/10.
		goto 20
	endif
	goto 130

120	write(0,'("Gauss fit not converging")')


c evaluate the uncertainties.
130	do 133 j=1,nterms
		sigma(j)=sqrt(sigsq*array(j,j)/alpha(j,j))
133	continue

c cary out any requested action on stack 1.
	goto(280,260,270)iacton+1

c put Gaussian function in stack 1 and overlay it on the data plot.
260	continue
	if(pltd.eq.0) call combex("pl fhr:")
	do 265 i=1,numpt1
		stak1(i)=gauss(i)
265	continue
	call combex("pl ov:oh:ovl:")
	goto 280

c Subtract the Gaussian function from the data in stack 1.
270	do 275 i=1,numpt1
		stak1(i)=stak1(i)-gauss(i)
275	continue
	pltd=0

280	continue
c  calculate stack header information
c  get factor to translate chans into velocity.
	hfact=rtran(1,3)
      fwhm1=widftr*a(3)*hfact
      fwerr1=widftr*sigma(3)*hfact
      fpeak1=tran(1,3,dble(a(2)))
      fperr1=sigma(2)*hfact
      ftmax1=a(1)
      fterr1=sigma(1)
      area1=1.064467*a(1)*fwhm1

	if(dpfl.ne.0) return

c  calculate information for printing.
	hfact=rtran(1,htype)
	fwhm=widftr*a(3)*hfact
	fwerr=widftr*sigma(3)*hfact
	fpeak=tran(1,htype,dble(a(2)))
	fperr=sigma(2)*hfact
	area=1.064467*a(1)*fwhm

c Do we need a header?
	if(cmdsig.ne.csigsv) then
		csigsv = cmdsig
		rfrom=tran(1,htype,dble(nfrom))
		rto=tran(1,htype,dble(nto))
		if(rto.lt.rfrom) then
			rtemp=rfrom
			rfrom=rto
			rto=rtemp
		endif
		write(6,321)rfrom,rto,(title(htype),i=1,4)
	endif
321	format("Gaussian fit from",f11.2,"  to",f11.2,a6/"scan # tpeak(k)"
     1	," err(k) center(",a4,") err  fwhm(",a4,") err  area(K",a4,
     2	") chisq nfre")

	if(nstk1(1).lt.-32000) then
		nsn1=nstk1(3)
	else
		nsn1=nstk1(1)
	endif
	write(6,331)nsn1,ftmax1,fterr1,fpeak,fperr,fwhm,fwerr,area
     1	,chisqr,nfree
331	format(i6,2f8.3,f11.3,f7.3,f8.3,f7.3,f10.4,f10.1,i4)
	end

c Function to calculate the current Gaussian function for the given channel.
	function gauss(nx)
	implicit undefined(a-z)
	real*4 gauss,arg
	automatic arg
	integer nx

	common /gfcom/a(3),sigsq
	real*4 a(3),sigsq

	arg=-0.5*((nx-a(2))/a(3))**2
	if(arg.lt.-50) then
		gauss=0.
		return
	else
		gauss=a(1)*exp(arg)
	endif
	end

c Function to calculate chi square over the region to be fit.
	real*8 function chisq(nfrom,nto)
	implicit undefined(a-z)
	integer nfrom,nto

	include '../../stacks/F.h'
	common /gfcom/a(3),sigsq
	real*4 a(3),sigsq

	real*4 gauss

	real*8 chi2
	integer i
	automatic chi2,i

	chi2=0.d0
	do 10 i=nfrom,nto
		chi2=chi2+(stak1(i)-gauss(i))**2
10	continue
	chisq=chi2/sigsq
	end

c Matrix inversion subroutine.
	subroutine matinv(array,norder)
	implicit undefined(a-z)

	integer mt
	parameter(mt=3)
	real*8 array(mt,mt)
	integer norder

	real*8 amax
c amax is Bevington's term, I am using it to hold the diagonal element.
	integer i,j,k
	automatic amax,i,j,k

	do 100 k=1,norder
		amax=array(k,k)
		if(abs(amax).lt.1.d-7)
     *			call error("matinv can't invert matrix")
		do 70 i=1,norder
			if(i.ne.k) array(i,k)=-array(i,k)/amax
70		continue
		do 80 i=1,norder
		    do 79 j=1,norder
			if(i.ne.k.and.j.ne.k)
     *			    array(i,j)=array(i,j)+array(i,k)*array(k,j)
79		    continue
80		continue
		do 90 j=1,norder
			if(j.ne.k) array(k,j)=array(k,j)/amax
90		continue
		array(k,k)=1./amax
100	continue
	end
