	*vtp = 0;
	*vft = 16;
	*vtt = 16;
	*vst = 16;
	*voff = 0;
	*vofft = 16;
	*htp = 0;
	*hft = 16;
	*htt = 16;
	*hst = 16;
	*unit = 0;
	*fcht = 8;
	*tcht = 8;
	*mode = 0;
	*mrkn = F_CROSS;
	*mrknt = 8;
	*hlbt = 3;
	*vlbt = 3;
	*mlbt = 3;
	*fillt = 8;
	*lnt = lnt_o;
	*lntt = 8;
	*ovlt = 8;
	*tic = tic_o;
	*tict = 8;
	*usar = usar_o;
	*usart = 8;
	*gr = gr_o;
	*grt = 8;
	*fly0 = fly0_o;
	*fly0t = 8;
	*lbs = lbs_o;
	*lbst = 8;
	*hd = hd_o;
	*hdt = 8;
	parse_(&cmnd_.gstr,plt_,plb.c);
	lnt_o = *lnt;
	tic_o = *tic;
	usar_o = *usar;
	gr_o = *gr;
	fly0_o = *fly0;
	lbs_o = *lbs;
	hd_o = *hd;