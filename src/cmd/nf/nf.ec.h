	strcpy(nfln,scan_.datfn);
	*nflnt = 3;
	*prnt = 8;
	parse_(&cmnd_.gstr,nft_,nfb.c);
	strcpy(scan_.datfn,nfln);
