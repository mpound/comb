	subroutine pd
	implicit undefined(a-z)
	include '../../scan/F.h'
	include '../../stacks/F.h'
	include '../../main/F.h'
	include 'pd.df.h'
	logical isscan
	integer i,j,k,kk,i4(6)
	integer indxsn
	automatic i,j,k,kk
	real*4 rhd(50),r4(3)
	integer*2 nhdr(128),i2(10)
	equivalence (numpt1,nhdr(1)),(fwid1,rhd(1))
	equivalence (i2(1),nszsc),(i4(1),lnfil),(r4(1),tsky)
	data isscan/.false./
	include 'pd.if.h'

	include 'pd.ef.h'
	if(flag1.ne.0) isscan=.false.
	if(flag2.ne.0) isscan=.true.
	if(flag3.ne.0) then
		write(6,'("numbch=",i6," nbadch=",8i6/"nuse=",(10i9))')
     1			numbch,nbadch,nuse
		write(6,'("scan common"/10i6/6i8/3g10.4)')i2,i4,r4
	else if(flag4.ne.0.or.(flag5.eq.0.and.flag6.eq.0.and.flag7.eq.0))then
  		if(.not.isscan)then
			write(6,191)nhdr
			write(6,192)rhd
			write(6,'(f13.4)')freq1
		else
c			write(6,191)(ndat(i),i=1,128)
			call pscanhdr
		endif
	endif
	if(flag5.ne.0) then
  		if(isscan)then
			write(6,191)(ndat(i),i=129,lnfil)
		else
			write(6,192)(stak1(i),i=1,numpt1)
		endif
	endif
191	format(10i7)
192	format(5g14.6)
	if(flag6.ne.0) then
		call prtsl
c		k=1
c200		if(nstk1( indxsn(k) ).lt.0) then
c			write(6,201)(nstk1( indxsn(i) ),i=k,k+1)
c201			format(/a1,a2)
c			k=k+2
c		else
c			kk=min(k+9,indx1)
c			do 210 j=k,kk
c				if(nstk1( indxsn(j) ).lt.0) go to 220
c210			continue
c220			write(6,221) (nstk1( indxsn(i) ),i=k,j-1)
c221			format(10i6)
c			k=j
c		endif
c		if(k.le.indx1) go to 200
	endif
c	if(flag7.ne.0) call prdate(ndat(5),ndat(16))
	if(flag7.ne.0) call prdate(iobssec)
	end
