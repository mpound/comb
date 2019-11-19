c rtran return the ratio between two types of horizontal plot units using
c stack 1 values of frequency and filter width.
c  For example : deltav=deltach*rtran(1,3).

	function rtran(ufrom,uto)
	implicit undefined(a-z)
	real*8 rtran
	integer ufrom,uto

	real*8 tran

	rtran=abs(tran(ufrom,uto,1.d10)-tran(ufrom,uto,0.d0))/1.d10
	end
