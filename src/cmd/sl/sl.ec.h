	*x1 = x1_o;
	*x1t = 16;
	*y1 = y1_o;
	*y1t = 16;
	*x2 = x2_o;
	*x2t = 16;
	*y2 = y2_o;
	*y2t = 16;
	*im = im_o;
	*imt = 8;
	strcpy(fn,SliceName());
	*fnt = 3;
	*mft = 8;
	*fact = 1.0;
	*factt = 16;
	*yadd = 0.0;
	*yaddt = 16;
	*miny = -1e30;
	*minyt = 16;
	*maxy = 1e30;
	*maxyt = 16;
	*logyt = 8;
	*vft = 16;
	*vtt = 16;
	*hft = 16;
	*htt = 16;
	*mode = 0;
	*mrkn = F_CROSS;
	*mrknt = 8;
	*figh = figh_o;
	*fight = 16;
	*figw = figw_o;
	*figwt = 16;
	*fillt = 8;
	*hlbt = 3;
	*vlbt = 3;
	*mlbt = 3;
	*lnt = lnt_o;
	*lntt = 8;
	*ovlt = 8;
	*fly0 = fly0_o;
	*fly0t = 8;
	parse_(&cmnd_.gstr,slt_,slb.c);
	x1_o = *x1;
	y1_o = *y1;
	x2_o = *x2;
	y2_o = *y2;
	im_o = *im;
	figh_o = *figh;
	figw_o = *figw;
	lnt_o = *lnt;
	fly0_o = *fly0;
