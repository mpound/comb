c subroutine pltoch takes an unordered range of real*8 numbers in plot units and
c returns an ordered pair of integer channel numbers limited to the range of
c stack 1.

	subroutine pltoch(h1,h2,n1,n2)
	implicit undefined(a-z)
	include "../main/F.h"
	include "../stacks/F.h"
	logical flag
	integer n1,n2,irondd
	real*8 r1,r2,ra,rb,rt
	real*8 h1,h2,tran
	automatic flag,rt,ra,rb
	flag=.false.

100	if(htype.eq.4)call warn("Converting from temperature to channels")
	ra=tran(htype,1,h1)
	rb=tran(htype,1,h2)
	if(rb.lt.ra) then
		rt=ra
		ra=rb
		rb=rt
	endif
	ra=max(ra,.501d0)
	rb=min(rb,float(numpt1+.499d0))
	if(flag) goto 200
	
	n1=irondd(ra)
	n2=irondd(rb)
	return

	entry pltchr(h1,h2,r1,r2)
	flag=.true.
	goto 100
200	r1=ra
	r2=rb
	end
