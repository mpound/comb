	subroutine pkwts(totwt,ichan,tempwt,itmplc,nwts)
c Pack weights into the condensed weight array. This subroutine must be called
c in order of increasing chan number. The arguments are:
c	totwt	the weight for the current channel.
c	ichan	the current chan number.
c	tempwt	a working array of 15 reals to hold the new weights.
c	itmplc	15 integers to hold the new locwt`s.
c	nwts	integer*2 will be set to the number of discrete weights.
c		nwts must be initialized to 0 before the first call.
c	The working arrays should also be cleared before the first call.

	implicit undefined (a-z)
	real*4 totwt,tempwt(15),smdiff,diff
	logical ifdif
	integer*2 itmplc(15),nwts
	integer ichan,isave,i
	automatic i,isave,smdiff,diff
c Check for first call
c If totwt is within 5% of previous stored weight, no action is needed.
	if(nwts.ne.0.and..not.ifdif(totwt,tempwt(nwts),totwt*1.e-5))return
	if(nwts.lt.15)then
		nwts=nwts+1
		goto 300
	endif
c
c At this point we have run out of space in the weight array. First we will
c find the adjacent pair of weights which is closest. These will then be
c replaced by their average to make room for the new value. The first 
c thing to consider is the new weight and the last one in the array.
c
	smdiff=abs(tempwt(15)-totwt)
	isave=15
	do 210 i=1,14
		diff=abs(tempwt(i+1)-tempwt(i))
		if(diff.lt.smdiff) then
			smdiff=diff
			isave=i
		endif
210	continue
c Now combine the two closest weights.
	if(isave.eq.15) then
		tempwt(15)=(tempwt(15)+totwt)/2.
		return
	endif
	tempwt(isave)=(tempwt(isave)+tempwt(isave+1))/2.
c Close up the weight array if necessary
	do 240 i=isave+1,14
		tempwt(i)=tempwt(i+1)
		itmplc(i+1)=itmplc(i+2)
240	continue
c Finally put in the new weight.
300	tempwt(nwts)=totwt
	itmplc(nwts)=ichan
	end

	function wt(nchan,istack)
	implicit undefined(a-z)
	include'../stacks/F.h'
c Define a fake array nwts which will allow us to access the scalar nwt of
c either stack.
	integer*2 nwts(stklen/2+1)
	integer i,nchan,istack,iofset
	real*8 wt
	automatic i,iofset
	equivalence(nwts(1),nwts1)

	iofset=(istack-1)*stklen/2
	do 100 i=1,nwts(1+iofset)-1
		if(nchan.lt.locwt1(i+iofset+1))goto 200
100	continue
200	wt=wght1(i+iofset/2)
	end
