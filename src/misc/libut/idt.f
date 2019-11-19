	logical ifdif,l
  100	write(6,'("a,b,fuzz")')
	read(5,*)a,b,fuzz
	l=ifdif(a,b,fuzz)
	write(6,'(l1)') l
	goto 100
	end
