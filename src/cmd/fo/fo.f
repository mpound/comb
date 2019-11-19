	subroutine fo
	implicit undefined (a-z)
	include '../../stacks/F.h'
	include '../../main/F.h'
	include 'fo.df.h'
	real*4 tempwt(15),totwt,sigwt,refwt
	integer nsigof,nrefof,iabfld,nfld,ichan,irond
	integer*2 itmplc(15),nwts
	automatic nsigof,nrefof,iabfld,nfld,itmplc,nwts,ichan
     1	,tempwt,totwt,sigwt,refwt
c functions
	integer stfull
	real*8 wt
	include 'fo.if.h'

c call parse to advance the ntty pointer.
	include 'fo.ef.h'

	if(stfull(1).eq.0) call error("No data in stack 1")
	if(and(nprc1,4).ne.0)call error("Stack already folded")
	if(refch1.eq.notfs) call error("NO FOLD - not freq switched")
	nfld=irond(expch1-refch1)
	iabfld=iabs(nfld)
	if(numpt1-iabfld.lt.5)call error("NO FOLD - too few chans left")
	if(nfld.gt.0) then
		nsigof=iabfld
		nrefof=0
		expch1=expch1-nsigof
		refch1=refch1-nsigof
	else
		nsigof=0
		nrefof=iabfld
	endif
	call clear(tempwt(1),60)
	call clear(itmplc(1),30)
	nwts=0
	numpt1=numpt1-iabfld
	do 100 ichan=1,numpt1
		sigwt=wt(ichan+nsigof,1)
		refwt=wt(ichan+nrefof,1)
		totwt=sigwt+refwt
		stak1(ichan)=(stak1(ichan+nsigof)*sigwt
     1			-stak1(ichan+nrefof)*refwt)/totwt
		call pkwts(totwt,ichan,tempwt,itmplc,nwts)
100	continue
	call move(tempwt,wght1,60)
	call move(itmplc,locwt1,30)
	nwts1=nwts
	call clear(stak1(numpt1+1),4*iabfld)
	rms1=rms1*0.707106781
	nprc1=or(nprc1,4)
	pltd=0
	end
