	subroutine ca()

	include '../../stacks/F.h'
	include '../../main/F.h'

	include "ca.df.h"
	include "caf.df.h"

c stack 3
	real stak3(512),expch3
c
c moved from stacks/F.h 6/9/91
	integer*2 inf(stklen/2,incore)
	equivalence(inf(1,1),numpt1)
c
	integer numpt3
	equivalence (stak3(1),inf(257,3)),(numpt3,inf(1,3)),(expch3,inf(131,3))

c global variables
	real*8 global(10)
	common /gg/global

c variables for stepping through data
	integer from,to,chan,off2,off3

c functions
	integer ibit,stfull

c initialize old default
	include "ca.if.h"
	include "caf.if.h"

c get arguments
	include "ca.ef.h"

c check for errors
	if(fn(1:2).eq."?!".or.fn(1:2).eq."??")call fparse(fn,cafd,cafb)
	if(ns.lt.1.or.ns.gt.3)call error("Bad number of stacks")
	if(stfull(1).eq.0)call error("Stack 1 is empty")

c fill used stacks, decide offsets, and bounds.
		from = 1
		to = numpt1

	goto(3,2,1)ns
1		if(stfull(3).eq.0)then
			call warn("No data in stack 3")
			call move(numpt1,numpt3,stklen)
			call mrkful(3)
		end if
		off3 = expch3 - expch1
		from = max(from,1-off3)
		to = min(to,numpt3-off3)

2		if(stfull(2).eq.0)then
			call warn("No data in stack 2")
			call move(numpt1,numpt2,stklen)
			call mrkful(2)
		end if
		off2 = expch2 - expch1
		from = max(from,1-off2)
		to = min(to,numpt2-off2)

3	continue
	if(from.ge.to)call error ("No overlapping channels")

c check that the use array is set for at least some channels in the overlap.
14	if(ibit(nuse,from).eq.0)then
		from = from + 1
		if(from.ge.to)
     *		    call error("Use array not set within range")
		goto 14
	end if


c step through data and perform calculation
	do 10 chan = from , to
		if(ibit(nuse,chan).eq.1)then
			goto(43,42,41)ns
41				global(4) = stak3(chan + off3)
42				global(3) = stak2(chan + off2)
43				global(2) = stak1(chan)
			continue
			call fparse(fn,cafd,cafb)
			goto(53,52,51)ns
51				stak3(chan + off3) = global(4)
52				stak2(chan + off2) = global(3)
53				stak1(chan) = global(2)
			continue
		end if
10	continue

c set plot flag
	pltd = 0

	return
	end
