	*h1 = h1_o;
	*h2 = h2_o;
	*ht = 16;
	*unit = 0;
	*stn = 1;
	*stnt = 8;
	parse_(&cmnd_.gstr,elt_,elb.c);
	h1_o = *h1;
	h2_o = *h2;
