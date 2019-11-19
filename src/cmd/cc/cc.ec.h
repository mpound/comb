	*iactn = 0;
	*nwf = stk_[0].freq;
	*nwft = 16;
	*nwv = stk_[0].vlsr;
	*nwvt = 16;
	*oe = oe_o;
	*oet = 16;
	parse_(&cmnd_.gstr,cct_,ccb.c);
	oe_o = *oe;
