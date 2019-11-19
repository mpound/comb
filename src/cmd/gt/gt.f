	subroutine gt
	implicit undefined(a-z)
	include '../../stacks/F.h'
	include '../../scan/F.h'
	include '../../main/F.h'
	include 'gt.df.h'
	logical typcmp,islast
	integer nxtscn
	integer*2 jbknd(3)
	integer n
	automatic n
	character*64 fname
	equivalence(fname,nstk1(1))
	include 'gt.if.h'
	data jbknd/100,25,10/

c set some 'old' variables which parse doesn't handle easily.
	data snt/4/
c	sn1f = datfn(ndfn-2:ndfn)
c	sn1n = nprssc
	sn2n = nprssc
	lbknd=mbknd
c	write(6,'("Gt: ",a24,2x,a5,i6)') datfn,dfn1,nprssc

	include 'gt.ef.h'

	datfn(ndfn-2:ndfn)=sn1f
	if(fss.ne.0) then
		n=1
		if(nstk1(1).lt.0) then
			n=3
			datfn(ndfn-2:ndfn)=fname(2:4)
c This code is for VAX like machines
c			datfn(ndfn-2:ndfn-2)=fname(1:1)
c			datfn(ndfn-1:ndfn)=fname(3:4)
		endif
		sn1n=nstk1(n)
	endif
	call scnlmt(sn1n,sn2n,sn1f,sn2f,istep,snf)
c mbknd will be set to 1,2,or3 for specific backends or 100,25,10 for
c bbe,nbe,or exp.
	if(lbetyp.gt.0) then
		mbknd=jbknd(lbetyp)
	else
		mbknd=lbknd
	endif
	call opend
c If snf .ne.0 no new scan numbers were given. If lmttyp.ne.0 a limiting type
c (eg "dt:") was given. If both are true, search forward untill the required
c scan type is found. Start off by incrementing the scan number.

	if(snf.ne.0.and.lmttyp.ne.0) then
		sn1n=nxtscn()
		if(sn1n.le.0) call error("End of file")
	endif
	call move(muse,nuse,maxchn/8)
100	nprssc=sn1n
	call read(nprssc)
	if(typcmp(lmttyp)) then
		call get
		if(tsfl.ne.0) call tschng
		if(snf.eq.0.and. .not.islast()) then
			call combex("pl fhr:")
			call wait
			sn1n=nxtscn()
			goto 100
		else
			pltd=0
		endif
	else
		sn1n=nxtscn()
		if(sn1n.gt.0) goto 100
	endif
	end
c
c
	logical function typcmp(kind)
	implicit undefined (a-z)
	include '../../scan/F.h'
	integer*2 kind
c
	goto(10,20,30,40,45,50,60,70,80,85,90) kind
	goto 200
10	if(iobstp.lt.10) goto 200
	goto 100
20	if(iobstp.eq.1) goto 200
	goto 100
30	if(iobstp.eq.2) goto 200
	goto 100
40	if(iobstp.eq.4) goto 200
	goto 100
45	if(iobstp.eq.5) goto 200
	goto 100
50	if(iobstp.eq.10) goto 200
	goto 100
60	if(iobstp.eq.11) goto 200
	goto 100
70	if(iobstp.eq.12) goto 200
	goto 100
80	if(iobstp.ge.20.and.iobstp.le.29) goto 200
	goto 100
85	if(iobstp.eq.22) goto 200
	goto 100
90	call error("Internal error in typcmp")
100	typcmp=.false.
	return
200	typcmp=.true.
	end
