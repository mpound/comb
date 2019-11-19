	subroutine us
	implicit undefined(a-z)
	integer ivalue,nb,ne,idiff,nb2,ne2,i
	automatic ivalue,nb,ne,idiff,nb2,ne2,i
	include "../../stacks/F.h"
	include "../../main/F.h"
c functions
	integer stfull

	include "us.df.h"
	include 'us.if.h'

	include "us.ef.h"

c Convert iactn to int*4 for passing to the stuse subroutine.
	ivalue = iactn

c If retrieve was requeted, do it first.
	if(rtflg.ne.0) call move(muse,nuse,maxchn/8)

c Compliment is the next operation.
	if(coflg.ne.0) then
		do 30 i=1,maxchn/32
			nuse(i)=xor(nuse(i),-1)
30		continue
	endif

c hflg will = 0 if horiz values were given. If they weren't given, either
c don't call use at all or use the full range of stack1.
	if(hflg.ne.0) then

c If any other action was requested, don't call use.
		if(rtflg+coflg+stflg.ne.0)goto 100

c As a special convenience, the command us with no arguments sets the use array
c rather than clearing it as would be logical from the input tree.
c This test allows the user to clear the use array if specificaly requested.
		if(actfl.eq.0)ivalue=1

c Finally the default channel numbers.
		nb=1
		ne=numpt1
	else
c make sure that plot units are defined
		if(stfull(1).eq.0) call error("No data in stack 1")
C convert from plot units to chans.
		call pltoch(h1,h2,nb,ne)
	endif
	call stuse(ivalue,nb,ne)

c If requested and freq sw data repeat the action shifted from sigch to refch.
	if(rfflg.ne.0.and.refch1.ne.notfs) then
		idiff=refch1-expch1
		nb2=nb+idiff
		ne2=ne+idiff
		if((idiff.gt.0.and.nb2.lt.ne).or.(idiff.lt.0.and.ne2.gt.nb)
     1			) call error("Use for sig & ref overlap")
		call stuse(ivalue,nb2,ne2)
	endif

c Finally save the result if requested.
100	if(stflg.ne.0) call move(nuse,muse,maxchn/8)
	end

	subroutine stuse(ivalue,nb,ne)
	implicit undefined(a-z)
	include "../../stacks/F.h"
	include "../../main/F.h"
c arguments
	integer ivalue,nb,ne
c Local variables
	integer n1,n2
	automatic n1,n2

c If ivalue is 0 or 1 the use array is cleared or set between chan nb and ne.
c First limit the values to real channels.

	if(ne.le.0.or.nb.gt.numpt1.or.ne.lt.nb)return
	n1=max0(nb,1)
	n2=min0(ne,numpt1)
	call filbts(nuse,ivalue,n1,n2)
	end
