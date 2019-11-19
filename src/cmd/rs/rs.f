	subroutine rs
c  rescale data rs
	include '../../stacks/F.h'
	include '../../main/F.h'
	include 'rs.df.h'
	integer i
	integer stfull

	include 'rs.if.h'

	pltd = 0
	include 'rs.ef.h'
c
	if(stfull(1).eq.0) call error("No data in stack 1")
	do 20 i=1,numpt1
20	stak1(i)=set+fact*stak1(i)
c
	factt1=fact*factt1
	rms1=rms1*abs(fact)
c
	end
