	subroutine cc()

	include '../../stacks/F.h'
	include '../../main/F.h'
	include "cc.df.h"

c temporary variables
	real texpch
	automatic texpch

c functions
	real*8 tran
	integer stfull

	include 'cc.if.h'

c store old value of expch for updating refch later
	texpch = expch1

c preset 'old' variables
	nwf = freq1
	nwv = vlsr1

	include "cc.ef.h"

c check for errors
	if(stfull(1).eq.0)call error("No data in stack 1")

	goto(100, 200, 300)iactn+1
	call error("Bad action")
c compute new center channel from new frequency
100	continue
		expch1 = tran(2,1,nwf)
		freq1 = nwf
	goto 1000
c compute new center channel from new velocity
200	continue
		expch1 = tran(3,1,nwv)
		vlsr1 = nwv
	goto 1000
c Correct an error in a receiver oscillator given an equivalent amount to
c shift the line in velocity.  Save the shift in expch1 in rsdm(1)
300	continue
		rsdm1(1) = rsdm1(1) - expch1
		expch1 = tran(3,1,vlsr1 - oe)
		rsdm1(1) = rsdm1(1) + expch1
	goto 1000

c update refch
1000	continue
	if(refch1.ne.notfs)refch1 = refch1 + expch1 - texpch

	return
	end
