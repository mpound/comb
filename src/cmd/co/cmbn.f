      subroutine cmbn(init)

c Do a weighted average of stack 1 and stack 2, leaving the result in stack
c 2. Init controls tests for stack differences:
c  Init=1 - difference storage is reset and any differences result in a
c	call to warn.
c  Init=0 - same except that there is no initial reset (continuation call).
c  Init=2 - no attention is paid to differences.
c  If 4 is ored with one of the above, c don't pay attention to position
c	differences.
c  If 8 is ored with one of the above, do a simple add rather than a
c	weighted agerage.
c  If 16 is ored in, don't combine scan number lists
c  If 32 is ored in, align the center channels independent of freq or velocity

	implicit undefined(a-z)
	include '../../stacks/F.h'
	include '../../main/F.h'
	real*4 tempwt(15),totwt,wt1,wt2,s1,s2,shift
	automatic tempwt,totwt,wt1,wt2,s1,s2
	integer*4 mmove,i,izer,irond,init
	automatic mmove,i,izer
	integer*4 nend,ioff1,i1,ioff2,i2,ii
	automatic nend,ioff1,i1,ioff2,i2,ii
	integer*2 nwts,itmplc(15)
	automatic nwts,itmplc
	integer*4 stfull
	real*8 wt
	real*4 pi
	parameter (pi=3.14159265359)

c need a comment here

c If stack #1 is empty, abort
	if(stfull(1).eq.0) call error("Empty stack 1 sub cmbn")
c If stack #2 is empty just move stack 1 in.
	if(stfull(2).eq.0) then
		call move(numpt1,numpt2,stklen)
c Call shift here so that its error storage will be initialized. The compiler
c refuses to allow a simple call of a function, thus the assignment.
		totwt=shift(init,i)
c same for catsn
		call catsn(1025)
		call mrkful(2)
		numst2 = 2
		return
	endif

c check for stack differences and get the amount to shift stack1 to the right.

	mmove=irond(shift(init,i))
	if(and(init,32).ne.0) mmove=expch2-expch1
	if(mmove.ge.0) then
c In this case leave stack 2 in place and shift stack 1 to the right, including
c as many chans as possible.
		ioff1=-mmove
		ioff2=0
	else
c here we move stack 2 to the right as much as possible to avoid loosing
c low chans in stack 1.
		ioff2=max0(mmove,numpt2-maxchn)
		ioff1=ioff2-mmove
	endif
	nend=min0(max0(numpt1-ioff1,numpt2-ioff2),maxchn)
	if(nend+ioff1.le.0.or.ioff1.ge.maxchn)
     1		call error("No overlap of stacks")

c If stack 2 is to be shifted left,
c shift the data in stack 2 to avoid overwriting it. It would be
c awkward to run the loop backward because of pkwts.
	if(ioff2.lt.0) then
		call move(stak2(1),stak2(1-ioff2),numpt2*4)
c adjust the sig and ref chans.
		expch2=expch2-ioff2
		if(refch2.ne.notfs)refch2=refch2-ioff2
	endif
	nwts=0
	izer=0
	call clear(tempwt,60)
	call clear(itmplc,30)
      do 100 i=1,nend
		i1=i+ioff1
		i2=i+ioff2
		if(i1.lt.1.or.i1.gt.numpt1)then
			wt1=0.
			s1 = 0.
		else
			s1=stak1(i1)
			if(abs(s1).gt.maxval) s1=maxval
c			if(s1.eq.dragon) then
c				wt1=0.
c			else
				wt1=wt(i1,1)
c			endif
		endif
		if(i2.lt.1.or.i2.gt.numpt2) then
			wt2=0.
			s2 = 0.
		else
			s2=stak2(i)
			if(abs(s2).gt.maxval) s2=maxval
c			if(s2.eq.dragon) then
c				wt2=0.
c			else
				wt2=wt(i2,2)
c			endif
		endif
		totwt=wt1+wt2
      		if(totwt.eq.0) then
			izer=izer+1
c	Don't propagate dragon in stacks for now.
c			stak2(i)=dragon
			stak2(i)=0
		else
			if(and(8,init).eq.0) then
				stak2(i)=(wt1*s1+wt2*s2)/totwt
			else
				if(wt2.eq.0.)stak2(i)=0.
				if(wt1.ne.0.)stak2(i)=stak2(i) + s1
			endif
		endif
		call pkwts(totwt,i,tempwt,itmplc,nwts)
  100 continue
	time2=time2+time1
	numpt2=nend
	if(izer.ne.0) write(0,'(i3," wts zero")')izer
	call move(tempwt,wght2,60)
	call move(itmplc,locwt2,30)
	nwts2=nwts
c  set up use array
	if(kusfl.ne.0) then
		do 111 ii=1,stkuse/16
			kuse2(ii)=and(kuse1(ii),kuse2(ii))
111		continue
	endif

c Calculate the new rms2
	if(rms1.ne.0.and.rms2.ne.0)rms2=1/sqrt(1/rms1**2 + 1/rms2**2)

c Concatenate the two scan number lists.
	if(and(init,16) .eq.0) call catsn(init)

	end
c
	function shift(kntrl,idif)

c Shift checks that stack1 and stack2 are similar enough observations
c to be combined and returns the amt which stack1 should be shifted right.
c Kntrl controls the action if a difference is found. Legal values are:
c	0 If new differences are found print messages and call warn.
c	1 Reset difference storage, then same as 0.
c	2 return the bit coded difference in idif, don't call warn.
c	4 ored with the above causes position differences to be ignored.

c
	implicit undefined(a-z)
	include '../../stacks/F.h'
	real*4 fmove,shift
	integer nwr,nwrsv,kntrl,idif
	logical ifdif
	real*8 tran,rtran
	automatic fmove,nwr
	save nwrsv
	real*4 pi
	parameter (pi=3.14159265359)
	if(and(3,kntrl).ne.0) then
		nwrsv=0
	endif
	nwr=nwrsv
	fmove=0.
	if((numpt1.gt.maxchn).or.(numpt2.gt.maxchn))
     1		call error("Stack too big")
	if(nprc1.ne.nprc2) nwr=or(nwr,1)
	if(and(4,kntrl).eq.0 .and.(ifdif(dec1+ddec1,dec2+ddec2,0.003) .or.
     1		ifdif(ra1+dra1,ra2+dra2,0.0002/cos(dec1*pi/180.))))
     2		nwr=or(nwr,4)
	if(nprc2.eq.0) goto 5
	if(ifdif(fwid1,fwid2,0.1*fwid1/numpt1)) then
		nwr=or(nwr,8)
		fmove=0.
	else
		fmove=(freq2-freq1)/fwid1
		if(abs(fmove).gt.0.1) nwr=or(nwr,2)
	endif
	if(refch1.ne.notfs.and.refch2.ne.notfs.and.
     1		ifdif(expch1-refch1,expch2-refch2,0.1))nwr=or(nwr,16)
	if(ifdif(vlsr1,vlsr2,fwid1*sngl(rtran(2,3))*0.1))nwr=or(nwr,32)
    5 continue
	if((nprc1.eq.1000).and.(fwid1.ne.fwid2))nwr=or(nwr,64)

	shift=fmove+expch2-tran(3,1,dble(vlsr2))
	if(and(3,kntrl).eq.2) then
		idif=nwr
	else if(nwr.ne.nwrsv) then
		nwr=xor(nwr,nwrsv)
		if(and(nwr,1).ne.0)write(0,'("Process types differ")')
		if(and(nwr,2).ne.0)write(0,'("Frequencies differ")')
		if(and(nwr,4).ne.0)write(0,'("Positions differ")')
		if(and(nwr,8).ne.0)write(0,
     1			'("Filter widths differ - no freq shift done")')
		if(and(nwr,16).ne.0)write(0,'("Amt of freq sw differ")')
		if(and(nwr,32).ne.0)write(0,'("Lsr velocities differ")')
		if(and(nwr,64).ne.0)write(0,'("Map parameters differ")')
		nwrsv=or(nwrsv,nwr)
		call warn("")
	endif
	end

	subroutine catsn(init)

c catsn concatinates the scan number list in stacks 1&2 taking account
c of imbeded file names. The result is in stack2 (and extension).
c If and(init,1024).ne.0, return after initializing nwrsv if needed

	implicit undefined(a-z)
	include '../../stacks/F.h'
	integer init,nwrsv,i1,i2,istrt,indxsn
	integer*2 ifn(2),dumnam(2),ntemp
	automatic i1,i2,istrt,ifn,ntemp
	integer namflg
	save nwrsv
	parameter(namflg=-32000)
	data dumnam/z'8055',z'4b4e'/
c dumnam contains the file name "UNK" coded as in a stack hdr.

	if(and(1,init).ne.0) then
		nwrsv=0
	endif
	if(and(1024,init).ne.0)return
c is there any room in stack 2?
	if(indx2.ge.160.and.nwrsv.eq.1) return
c Find the file name at the end of the list in stack2. Default to UNK.
	ifn(1)=dumnam(1)
	ifn(2)=dumnam(2)
	do 100 i2=1,indx2
		if(nstk2(indxsn(i2)).lt.namflg) then
			ifn(1)=nstk2(indxsn(i2))
			ifn(2)=nstk2(indxsn(i2+1))
c Transfer the complete file name here. The next pass of the do loop will
c do no harm.
		endif
100	continue
c Now move the list from stack1 in. If the first file name in 1 equals the last
c name in 2, don't transfer the file name.
	if(ifn(1).eq.nstk1(1).and.ifn(2).eq.nstk1(2)) then
		istrt=3
	else
		istrt=1
	endif
	do 200 i1=istrt,indx1
		if(indx2.ge.160) then
			if(nwrsv.ne.1) then
				write(0,'("Scan numbers overflow")')
				nwrsv=1
			endif
c this replaces 'return' to avoid a compiler error which doesn't save nwrsv
			go to 400
		endif
		indx2=indx2+1
c		if(indx2.eq.33) call clear(nstkx2,256)
		ntemp=nstk1(indxsn(i1))
		i2 = indx2
		nstk2(indxsn(i2))=ntemp
		if(ntemp.lt.namflg) then
			nlist2=nlist2-1
		else
			nlist2=nlist2+1
		endif
200	continue
400	end
