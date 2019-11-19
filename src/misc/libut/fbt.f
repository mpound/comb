	integer*2 ipat(10)
 100	write(6,'("nbeg,nend,ibit")')
	read(5,*)nbeg,nend,ibit
	do 110 i=1,10000
110	call filbts(ipat,ibit,nbeg,nend)
	write(6,'(10o7)')ipat
	goto 100
	end
