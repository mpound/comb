	integer*2 ipat(10)
	data ipat/z'ffff',z'0000',z'00ff',z'ff00',z'1234',z'5678',z'9abc',
&	z'def0',z'5555','zaaaa'/
  100	write(6,*)ipat
	write(6,'("bit number?")')
	read(5,*)nbit
	j=ibit(ipat,nbit)
	write(6,*)j
	write(6,'("modify bit1 bit2 value")')
	read(5,*)ib1,ib2,n
	call filbts(ipat,n,ib1,ib2)
	goto 100
	end
