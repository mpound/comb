	real*8 d
5	continue
	write(6,'("double?")')
	read(5,*)d
	write(6,'("double,rounded float, rounded double" g15.8,2i6)')
     C	d, irond(float(d)), irondd(d)
	goto 5
	end
