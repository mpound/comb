	subroutine bc
	implicit undefined(a-z)
	include '../../stacks/F.h'
	include '../../scan/F.h'
	include '../../main/F.h'
	include 'bc.df.h'
	integer i,n(2),indxsn
	automatic i,n
	integer stfull

	include 'bc.if.h'
	include 'bc.ef.h'

c Iaction=1 is a request to clear previous bad channels.
	if(iact.ne.0) call clear(numbch,20)
c Rflg=1 means that no horizontal values were given.
	if(rflag.ne.0) return
c Check that stack1 is reasonable and contains the scan which was last
c read. Indxsn is a function which compensates for the extension
c of the scan number list beyond the stack header into an overflow
c block.
	if(stfull(1).eq.0) call error("No data in stack 1")
	if(ncur.ge.76.and.nprc1.eq.kontrl) then
		do 50 i=1,indx1
			if(nstk1(indxsn(i)).eq.ndat(1)) goto 90
50		continue
	endif
	call error("Stack1 too diferent from scan")
90	continue
c pltoch converts a range of horiz plot units into an ordered pair of chan #.

	call pltoch(h1,h2,n(1),n(2))

	if(n(2)-n(1).gt.20) call warn("more than 20 bad chans")

	do 100 i=1,2
		if(numbch.gt.7)call error("Out of bad chan storage")
		numbch=numbch+1
		if(inver.gt.0) then
			nbadch(numbch)=ncur+n(i)
		else
			nbadch(numbch)=ncur+1+numpt1-n(3-i)
		endif
100	continue

c now fix stack 1
	call elim(n(1),n(2))
	end
