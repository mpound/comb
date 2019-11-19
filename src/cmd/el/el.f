	subroutine el
	implicit undefined (a-z)
	include '../../main/F.h'
	include '../../stacks/F.h'
	include "el.df.h"
	integer n1,n2
	automatic n1,n2
	integer stfull

	include 'el.if.h'

	include "el.ef.h"

	if(stfull(1).eq.0) call error("No data in stack 1")
	call pltoch(h1,h2,n1,n2)
	call elim(n1,n2)
	end

	subroutine elim(n1,n2)
	implicit undefined(a-z)

c eliminate channels n1 through n2 in stack 1 by replacing them by the avg
c of the chans on either end of the range. If n1 is 1 or n2 = numpt1, then
c use the other as the replacement value.

	include '../../stacks/F.h'
	include '../../main/F.h'
	real*8 avg,cnt
	integer n1,n2,i
	automatic avg,cnt,i

	if (n1.gt.1) then
		cnt=1.
		avg=stak1(n1-1)
	else
		cnt=0.
		avg=0.
	endif
	if(n2.lt.numpt1) then
		cnt=cnt+1.
		avg=avg+stak1(n2+1)
	endif
	if(cnt.le.0) call error("can't el all chans")
	avg=avg/cnt

	do 100 i=n1,n2
100		stak1(i)=avg
	pltd=0
	end
