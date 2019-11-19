	subroutine sq
	include '../../stacks/F.h'
	include '../../main/F.h'
	include 'sq.df.h'

c Functions
	real*8 tran,wt
	integer irond,irondd
	integer stfull

c Channel about which the expansion occurs
	real exch
	automatic exch

c Channel numbers in the old spectrum corresponding to chan edges in the new.
	real c1,c2
	integer n1,n2,no
	automatic c1,c2,n1,n2,no

c Center channel about which expansion occurs.
	real exch
	automatic exch

c Channels in the new spectrum, not shifted into final place.
	integer nb,ne,nn
	automatic nb,ne,nn

c The amt to add to chans in new to get final chan #. (nfinal=new+ishift)
	integer ishift
	automatic ishift

c Channel numbers in the new shifted spectrum.
	integer n
	automatic n

c Temporary storage for the new spectrum and its weights
	real tempst(maxchn),tempwt(15),totwt,sum,weight
	integer*2 itmplc(15),nwts
	automatic tempst,tempwt,totwt,sum,weight,nwts,itmplc

	include 'sq.if.h'

c The formula for squish is: nold-exch=(new-exch)*wnool where exch is the
c channel about which the expansion occurs.

c Get the arguments
	include 'sq.ef.h'

	if(stfull(1).eq.0) call error("No data in stack 1")
	if(wnool.lt. 0.01 .or. wnool .gt. 100.)
     ^		call error("unreasonable width ratio")

c Set up things for plot
	pltd=0

c Set up the center of expansion
	if(hflg.eq.0) then
		exch=tran(htype,1,h)
	else
		exch=expch1
	endif

c Calculate the ends of the new spectrum. Don't use a chan that has <0.4 of
c the weight of a full chan.
	nb=irondd((0.5-exch)/wnool+exch+0.4)
	ne=irondd((numpt1+0.5-exch)/wnool+exch-0.4)

c Calculate ishift. First put exch at 256.5 in the final spectrum.
	ishift=irond(maxchn/2+0.5-exch)
c Now get rid of any unused chans at the end of a 512 ch spectrum
	ishift=max0(ishift,maxchn-ne)
c Finally elim unused chans at the beginning
	ishift=min0(ishift,1-nb)

c Limit nb & ne to the range [1,512]
	nb=max0(nb,1-ishift)
	ne=min0(ne,maxchn-ishift)
c	write(6,111) exch,nb,ne,ishift
c  111 format("exch",f7.2,"nb",i7,"  ne",i7,"   ishift",i7)

c Clear the wts storage
	call clear(tempwt(1),60)
	call clear(itmplc(1),30)
	nwts=0

c Step through the new chans.
	do 100 nn=nb,ne

c Find the edges of the new chan in the old spectrum. Avoid tiny edges of
c channels ( < .001ch).
		c1=(nn-0.5-exch)*wnool+exch
		c2=c1+wnool
		n1=max0(irond(c1+0.001),1)
		n2=min0(irond(c2-0.001),numpt1)

c Calculate the value and weight of the new chan
		totwt=0.0
		sum=0.0
		do 90 no=n1,n2
			weight=wt(no,1)*(amin1(no+0.5,c2)-
     1				amax1(no-0.5,c1))
			totwt=totwt+weight
			sum=sum+stak1(no)*weight
90		continue
		n=nn+ishift
		if(totwt .lt. 1.0) then
			tempst(n)=sum
		else
			tempst(n)=sum/totwt
		endif
		call pkwts(totwt,n,tempwt,itmplc,nwts)
100	continue

c Move the results into place
	call move(tempwt,wght1,60)
	call move(itmplc,locwt1,30)
	nwts1=nwts
	numpt1=n
	call move(tempst,stak1,numpt1*4)
c	if(numpt1.lt.nsize) call clear(stak1(numpt1+1),(nsize-numpt1)*4)

c Adjust the header values
	expch1=(expch1-exch)/wnool+exch+ishift
	if(refch1.ne.notfs)refch1=(refch1-exch)/wnool+exch+ishift
	fwid1=fwid1*wnool
	if(wnool.gt.1) rms1=rms1/sqrt(wnool)
	end
