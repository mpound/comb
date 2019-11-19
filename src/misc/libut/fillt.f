	integer i(20)
	character c
  100	call clear(i,80)
	write(6,'(5o10)')i
	write(6,'("pos,numb,char")')
	read(5,'(2i5,a1)')j,n,c
	call fill(i(j),c,n)
	write(6,'(5o10)')i
	goto 100
	end
