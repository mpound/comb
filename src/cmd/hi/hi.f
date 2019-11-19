	subroutine hi
	implicit undefined (a-z)
	include '../../stacks/F.h'
	include '../../main/F.h'
	include "hi.df.h"
c functions
	integer stfull

	include 'hi.if.h'
	include 'hi.ef.h'

	if(stfull(1).eq.0) call error("No data in stack 1")

	call idmset

	call h1rm

	end

c Preserve various scan header info in the isdum array

     	subroutine idmset

	implicit undefined (a-z)
	include '../../stacks/F.h'
	include '../../scan/F.h'
	include '../../main/F.h'

	numpt1=124
	dl1=0
	db1=0
	isdum1(1) = ndat(73)
	isdum1(2) = ndat(74)
	isdum1(3) = ndat(75)
	isdum1(4) = ndat(100)
	isdum1(5) = ndat(101)
	isdum1(6) = ndat(102)
	isdum1(7) = ndat(103)
	isdum1(8) = ndat(115)
	isdum1(9) = ndat(107)
	isdum1(10) = ndat(108)
	isdum1(11) = ndat(109)
	isdum1(12) = ndat(110)
	isdum1(13) = ndat(111)
	isdum1(14) = ndat(112)

	end

     	subroutine h1rm

	implicit undefined (a-z)
	include '../../stacks/F.h'
	include '../../scan/F.h'
	include '../../main/F.h'

	integer zlsr, nz, i, n
	real*8 Tlsr, Tn, m

c Fix up stack header

	ibsln1 = 3

     	rsdm1(1) = odra2
     	rsdm1(2) = oddec2
     	rsdm1(3) = epoch2
     	rsdm1(4) = vpeak2
    	rsdm1(5) = rsdm2(1)
     	rsdm1(6) = rsdm2(2)

c Set zero lsr values for stack,etc

	zlsr = expch2 - (vlsr2/5.2765) + 0.01

50	continue
	do 100 i=1,124
     		nz = i - zlsr
     		stak1(i) = stak1(i) -
     1			(rsdm1(1) + rsdm1(2)*nz + rsdm1(3)*nz*nz +
     2			rsdm1(4) + rsdm1(5)*i + rsdm1(6)*i*i)
100	continue

c Check for consistency of Berkeley headers

	do 200 i=1,124
		rsdm1(7) = rsdm1(7) + (stak1(i) - stak2(i))**2.0
200	continue

	rsdm1(8) = (l1-l2)*(l1-l2)+(b1-b2)*(b1-b2)+
     1		(ra1-ra2)*(ra1-ra2)+(dec1-dec2)*(dec1-dec2)


c If an unrecorded baseline has been removed, remove and record

	if (rsdm1(7).gt.0.00001) then
		if (zlsr.lt.64) then 
			n = 119
		else 
			n = 6
		end if
		Tlsr = 0.
		Tn = 0.
		do 350 i=-4,4
			Tlsr = Tlsr + (stak1(zlsr+i) - stak2(zlsr+i))
			Tn = Tn + (stak1(n+i) - stak2(n+i))
350		continue	
		Tlsr = Tlsr/9.0
		Tn = Tn/9.0
		m = (Tn - Tlsr)/(n - zlsr)
		rsdm1(1) = rsdm1(1) + Tlsr
		rsdm1(2) = rsdm1(2) + m
		do 400 i=1,124
			nz = i - zlsr
			stak1(i) = stak1(i) - Tlsr - m*nz
400		continue

		rsdm1(7) = 0
		do 450 i=1,124
			rsdm1(7) = rsdm1(7) + (stak1(i) - stak2(i))**2.0
450		continue

	end if

c Correct temperatures and baseline coefficients for change in tau zenith
c .01118 is the new tau, .0082 the old.

	rsdm1(9) = exp(-1.0*(.01118 - .0082)*ndat(23)/16384.0)
	do 500 i=1,124
		stak1(i) = stak1(i)*rsdm1(9)
500	continue
	do 550 i=1,6
		rsdm1(i) = rsdm1(i)*rsdm1(9)
550	continue
			
	end
		

