	integer n(65536)

100	do 110 i=1,65536
110		n(i)=i

	write(6,'("from,to,number")')
	read(5,*)nfrom,nto,numb
	call move(n(nfrom),n(nto),numb*4)
130	write(6,'("first,last to print(neg to stop)")')
	read(5,*)nfrom,nto
	if(nfrom.lt.0) goto 100
	write(6,'(11i6)')(j,(n(i),i=j,j+9),j=nfrom,nto,10)
	goto 130
	end
