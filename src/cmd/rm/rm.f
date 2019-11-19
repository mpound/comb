	subroutine rm
	implicit undefined (a-z)
	include '../../stacks/F.h'
	include '../../main/F.h'
	include 'rm.df.h'
	real count, rms, ssbn
	integer chnl, ibit
	automatic count, rms, ssbn, chnl
c functions
	integer stfull

	include 'rm.if.h'

	include 'rm.ef.h'
	if(stfull(1).eq.0) call error("No data in stack 1")
	count = 0.
	rms = 0.
	do 10 chnl = 1, numpt1
		if (ibit(nuse, chnl).eq.0) go to 10
		rms = rms + (stak1(chnl) ** 2)
		count = count + 1
10	continue
	if (count.le.2) call error("Not enough channels in stack 1")
	rms = sqrt(rms / (count - 2))
c put rms value in stack1
	rms1 = rms
c calculate equivalent single sideband noise temp of system
	ssbn = (rms * sqrt(time1 * fwid1 * 1.e6)) / 2
	if(dp .ne. 0) return
	write (6, 20) rms, ssbn
20	format ('rms = ', g12.4, 5x, 'system noise = ', g10.5)
	return
	end
