c tran - translate a given value from one unit measure to another

c	Tran takes as arguments a source unit measure (CHAN, FREQ, or VEL), a
c	destination unit measure, and a value.  The value is assumed to be
c	expressed in the source units.  Tran returns the same value as it would
c	be represented in the destination units.  It works by changing the
c	value into CHAN and then changing it into the desired type.

	function tran(ufrom,uto,value)
	implicit undefined(a-z)

c define stack 1 header values
	include '../stacks/F.h'

	real*8 tran,value
	integer ufrom,uto

c define the speed of light
	real*4 c
	parameter(c=2.99792458d5)

c skip whole process if source is the same as destination
	if(ufrom.eq.uto)then
		tran = value
		goto 8
	end if

c skip whole process if either is temperature
	if(ufrom.eq.4.or.uto.eq.4)then
		tran = value
		goto 8
	end if

c skip whole process if tran is being used when it doesn't apply
	if(ufrom.eq.0.or.uto.eq.0)then
		tran = value
		goto 8
	end if

c change into channel
	goto (1,2,3) ufrom

c	from channel
1		tran = value
		goto 4

c	from frequency
2		tran = expch1+(freq1-value)/fwid1
		goto 4

c	from velocity
3		tran = expch1+(value-vlsr1)*freq1/fwid1/c

4	continue

c change into destination (note this is skipped if uto is CHAN)
	goto (7,5,6) uto

c	to frequency
5		tran = freq1-(tran-expch1)*fwid1
		goto 7

c	to velocity
6		tran = vlsr1+(tran-expch1)*fwid1*c/freq1

7	continue

8	continue
	end
