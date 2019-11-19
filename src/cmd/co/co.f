	subroutine co

	include '../../stacks/F.h'
	include '../../main/F.h'
	include 'co.df.h'
c
	integer init
	automatic init
	integer cmdsav
	data cmdsav/-1/
c
	include 'co.if.h'
c
	include 'co.ef.h'
c
	call move(muse,nuse,maxchn/8)
c  call gstack(locad,locsm)
	if(locad.ne.1.or.locsm.ne.2)call gstak(locad,locsm)
	if(cmdsig.ne.cmdsav) then
		cmdsav=cmdsig
		init=1
	else
		init=0
	endif
	if(addf.ne.0) init=init+8
	if(acf.ne.0) init=init + 32
	if(dcf.ne.0) init=init + 4
	call cmbn(init)
	call move(numpt2,numpt1,stklen)
	if(kusfl.ne.0) call move(kuse2,nuse,stkuse/8)
	pltd=0
	end
