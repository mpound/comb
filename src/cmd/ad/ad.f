	subroutine ad
c
c  ncare=0 means add will add all scans regardless of baseline
c      or clouds or pointing problems
c  ncare=1,2,3 means that add is increasingly severe about using
c      dubious scans
c  ncare=1 is default in cmi
	implicit undefined(a-z)
	include '../../stacks/F.h'
	include '../../main/F.h'
	include '../../scan/F.h'
	include 'ad.df.h'

	integer isav,init,kount,itest,nscan
	automatic isav,init,kount,itest,nscan
	integer cmdsav
	integer nxtscn
	integer*2 jbknd(3)
	include 'ad.if.h'
	data cmdsav/-1/
	data jbknd/100,25,10/

c set up 'old' values which parse doesn't easily deal with
	data snt/4/
c	sn1f= dfn1(1:3)
c	sn1n=nprssc
	sn2n=nprssc
	lbknd=mbknd
c call parse
	include 'ad.ef.h'
c Put current file name end in place
	datfn(ndfn-2:ndfn)=sn1f
c set up limits, always declaring that limits were set
	call scnlmt(sn1n,sn2n,sn1f,sn2f,istep,0)

c set up backend
	if(lbetyp.gt.0) then
		mbknd=jbknd(lbetyp)
	else
		mbknd=lbknd
	endif
c open data file and initialize a few things
	call opend
	call move(muse,nuse,maxchn/8)
	if(cmdsig.ne.cmdsav) then
		cmdsav=cmdsig
		init=1
	else
		init=0
	endif
      kount=0
	nscan = sn1n

100	nprssc=nscan
	call read(nprssc)
	if(nscan.eq.sn1n) then
		isav=ndat(13)
	else if(ndat(13).ne.isav) then
		kount=kount+1
		go to 150
	endif
c this is the place to check icare stuff
	if(ndat(34)+ncare.gt.2) then
		kount=kount+1
		go to 150
	endif
	call get
c do we need to check for matching header?
	if(mhflg.ne.0.and.nlist2.ne.0.and.nlist1.ne.0) then
		call shift(2,itest)
		if(itest.ne.0) then
			kount=kount+1
			go to 150
		endif
	endif
	call cmbn(init)
	init=0
150	nscan = nxtscn()
	if(nscan.gt.0) goto 100
	if(kount.ne.0) write(6,'(i5" Scans not added")')kount
	call move(numpt2,numpt1,stklen)
	pltd=0
	end
