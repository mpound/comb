	function indxsn(i)

c Indxsn returns the proper index into the nstk array to take care of the
c scan numbers that are stored in the extension array - nstkx.

	implicit undefined(a-z)
	include '../stacks/F.h'
	integer indxsn,i
	if(i.le.32) then
		indxsn=i
	else
		indxsn=i+maxchn*2+256-86
	endif
	end
