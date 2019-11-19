	integer*2 i(1024)
5	do 10 j=1,1024
10	i(j)=j
	write(6,'("how many bytes?")')
	read(5,*)j
	call swab(i,i,j)
	write(6,'(16i5)') (i(k),k=1,j)
	goto 5
	end
