	subroutine li
	implicit undefined (a-z)
	include '../../stacks/F.h'
	include '../../main/F.h'
	include "li.df.h"
	integer modisp
	automatic modisp
c functions
	real*8 tran
	integer stfull

	include 'li.if.h'

	if(nordr.gt.1) nordr=1
	include "li.ef.h"

	if(stfull(1).eq.0) call error("No data in stack 1")
	modisp=iacton
	if(widfl.eq.0) exwid1=tran(htype,1,wid)-tran(htype,1,0.d0)

	call bsln(nordr,modisp)
	pltd=0

	end

      subroutine bsln(nslope,modisp)

c     2-23-81 modified for modisp=2 - aas
c
c  baseline removal by least squares - uses points i if
c     i.gt.0  and  i.le.numpt1  and  ibit(i,nuse).ne.0
c  kbsln=0,1,2 will additionally cause no points,points within
c  nwid/2 of sig,within nwid/2 of sig or ref to be eliminated
c  baseline removed for all points between 1 and numpt1
c
c curfi declarations:
c
c  m = polynomial  order
c modisp = 1 evaluates b and replaces y by best fit curve
c modisp = 0 evaluates b and replaces y by residuals
c modisp = 2 same as modisp=0 and replaces stack 2 by residuals
c
c  current dimensions are for polynomial order 15
c  a(max,max+1),b(2*max-2),mm(max) where max = nslope+1

	implicit undefined (a-z)
	include '../../stacks/F.h'
	include '../../main/F.h'
	logical ifdif
c functions and arguments
	integer irond,ibit,indxsn,modisp,nslope
	integer*4 mm(16),nsig1,nsig2,nref1,nref2
     1  ,i,j,ipj,m,mp1,m2,ms,no2,nbeg,nen
	real*8 aa(16,17),bb(30),a,b,a2,bb2,c,ci,cwrat
     1  ,offset,scale,xc,xp,xx,xxc,xy,y,ycomp
	automatic mm(16),nsig1,nsig2,nref1,nref2
     1  ,i,j,ipj,m,mp1,m2,ms,no2,nbeg,nen
	automatic aa(16,17),bb(30),a,b,a2,bb2,c,ci,cwrat
     1  ,offset,scale,xc,xp,xx,xxc,xy,y,ycomp
c
	if(modisp.eq.2) then
		if(ibsln1.ne.ibsln2.or.ifdif(rmval1,rmval2,1e-4)
     1		.or.indx1.ne.indx2.or.factt1.ne.factt2
     2		.or.nprc1.ne.nprc2
     3		) goto 20
		do 10 i=1,indx1
			j=indxsn(i)
			if(nstk1(j).ne.nstk2(j)) goto 20
10		continue
	endif
	goto 30
20	call warn("Stacks 1&2 not compatable for baseline removal")
30	continue

      if(nslope.lt.0)return
	if(nslope.gt.15) call error("Baseline order too high")

c Restrict baseline operations to the plotted range if a plot exists.
	call plchns(nbeg,nen)

c Set the new ibsln parameters.
	ibsln1=max0(ibsln1,nslope)
c If stack 2 is to be modified also set the chan width ratio.
	if(modisp.eq.2)then
		ibsln2=ibsln1
		if(fwid1.lt.1e-6) call error("Bad stack1 in bsln")
		cwrat=fwid2/fwid1
	endif

c use c as a convenient temporary while calculating the excluded chans.
	if(exwid1.gt.0.9) then
		c=exwid1/2.
	else
		c=-1000.
	endif
	nsig1=irond(expch1-c)
	nsig2=irond(expch1+c)
	nref1=irond(refch1-c)
	nref2=irond(refch1+c)

c If the order is higher than 1, use the more complicated routine.
	if(nslope.gt.1) goto 300

      c=0.
      xc=0.
      xxc=0.
      y=0.
      xy=0.
      do 100 i=nbeg,nen
      if(ibit(nuse,i).eq.0)go to 80
      if((i.ge.nsig1).and.(i.le.nsig2))go to 80
      if((i.ge.nref1).and.(i.le.nref2))go to 80
      ci=i
      c=c+1.
      xc=xc+ci
      xxc=xxc+ci*ci
      y=y+stak1(i)
      xy=xy+i*stak1(i)
   80 continue
  100 continue
      if(c.le.nslope)call error("bsln-too few points")
c  a,b are usual coefficients for y=ax+b
      a=(y*xc-xy*c)/(xc*xc-c*xxc)
      b=(xc*xy-xxc*y)/(xc*xc-c*xxc)
      offset=y/c
      if (nslope.eq.1) offset=a*expch1+b
      do 200 i=1,numpt1
      if(modisp.ne.1)go to 199
	if(nslope.eq.1) then
		stak1(i)=a*i+b
	else
      		stak1(i)=offset
	endif
      go to 200
  199 if(nslope.eq.1) then
		stak1(i)=stak1(i)-(a*i+b)
	else
		stak1(i)=stak1(i)-offset
	endif
  200 continue
      if (modisp.eq.1) go to 290
	rmval1=rmval1+offset
      if (modisp.ne.2) go to 290
      a2=a*cwrat
      bb2=b+a*expch1-a2*expch2
      do 280 i=1,numpt2
	if (nslope.eq.1) then
		stak2(i)=stak2(i)-(a2*i+bb2)
	else
		stak2(i)=stak2(i)-offset
	endif
  280 continue
	rmval2=rmval2+offset
  290 if(nslope.le.1) return
c
c     subroutine curfi(modisp)
c
300	no2=(nen+nbeg)/2
      scale=sqrt((nen-nbeg)/2.)
      m=nslope+1
      mp1=m+1
      m2=2*m-2
      do 1 i = 1,m
      do 1 j = 1,mp1
    1 aa(i,j) = 0.0
      do 3 i = 1,m2
    3 bb(i) = 0.0
      do 6 i = nbeg,nen
      if(ibit(nuse,i).eq.0)go to 6
      if((i.ge.nsig1).and.(i.le.nsig2))go to 6
      if((i.le.nref2).and.(i.ge.nref1))go to 6
      xp=float(i-no2)/scale
      xx=xp
      bb(1) = bb(1) + xp
      aa(1,mp1) = aa(1,mp1) + stak1(i)
      aa(1,1) = aa(1,1) + 1.
      do 2 j = 2,m
      aa(j,mp1) = aa(j,mp1) + stak1(i)*xp
      xp = xp*xx
    2 bb(j) = bb(j) + xp
      do 61 j= mp1,m2
      xp = xp*xx
      bb(j) = bb(j) + xp
   61 continue
    6 continue
	if(aa(1,1).le.nslope)call error("Bsln-too few pnts")
      ms = 2
      do 4 i=1,m
      do 5 j = ms,m
      ipj = i + j - 2
    5 aa(i,j) = bb(ipj)
    4 ms = 1
      call eqsol(m,aa,mm,bb)
      do 8 i = 1,numpt1
      xp=float(i-no2)/scale
      call eqval(m,bb,xp,ycomp)
      if(modisp .ne. 1) go to 9
      stak1(i) = ycomp
      go to 8
    9 stak1(i)=stak1(i)-ycomp
    8 continue
      if (modisp.eq.1) return
      xp=(expch1-no2)/scale
      call eqval(m,bb,xp,offset)
      rmval1=rmval1+offset
      if (modisp.ne.2) return
c evaulate fit for stak 2
      do 108 i=1,numpt2
      xp=((cwrat*(i-expch2)+expch1)-no2)/scale
      call eqval(m,bb,xp,ycomp)
      stak2(i)=stak2(i)-ycomp
  108 continue
      rmval2=rmval2+offset
c
c end of curfi
c
	end
      subroutine eqsol(n,a,m,x)
c
	integer m(16),i,j,ip,n,nn,mmm
	real*8 a(16,17),x(30),amax,zmult
	automatic i,j,ip,nn,mmm,amax,zmult
      nn=n+1
      do 5 i = 1,n
      m(i) = 1
      amax = a(i,1)
      do 2 j = 2,n
      if (abs(a(i,j)) - abs(amax))2,2,100
  100 amax = a(i,j)
      m(i) = j
    2 continue
      do 4 j = 1,nn
    4 a(i,j) = a(i,j)/amax
      do 5 ip = 1,n
      if (ip-i)50,5,50
   50 mmm = m(i)
      zmult = a(ip,mmm)
      do 6 j = 1,nn
      if (j-mmm)9,8,9
    8 a(ip,j) = 0.
      go to 6
    9 a(ip,j) = a(ip,j) - zmult*a(i,j)
    6 continue
    5 continue
      do 7 i = 1,n
      mmm = m(i)
    7 x(mmm) = a(i,nn)
      return
	end
      subroutine eqval(m,bb,xp,ycomp)
c
	implicit undefined (a-z)
	integer m,j
	real*8 xp,ycomp,bb(37)
	automatic j
	ycomp=bb(m)
	do 7 j=m-1,1,-1
		ycomp=ycomp*xp+bb(j)
7	continue
      end

