# include "include.h"

extern void ad_(),af(),bc_(),c(),ca_(),cc_(),co_(),cr(),cv(),da();
extern void dm(),dv(),docmd(),ex(),el_(),em();
extern void fl(),fo_(),ft(),gm(),gf_(),gi_(),gs_(),gt_(),hc(),/*hi_(),*/im(),in_(),is();
extern void lf_(),li_(),lk(),nf(),me(),ns(),op(),p(),pa(),cm(),cplot();
extern void pd_(),pf_(),ph(),pl(),pr(),quit(),rc(),rm_(),rs_();
extern void rt(),sc(),sl(),sp(),st(),th(),tp(),up(),us_(),lc(),sq_(),wr_(),xf();
extern void lk(),ts(),uf(),v(),vc(),vm(),wc(),wf(),wu(),rout(),rin();
/*extern void xx();*/
Icmd icmd[] =
{
	"ad",ad_,
	"af",af,
	"bc",bc_,
	"c ",c,
	"ca",ca_,
	"cc",cc_,
	"cm",cm,
	"co",co_,
	"cp",cplot,
	"cr",cr,
	"cv",cv,
	"da",da,
	"dm",dm,
	"do",docmd,
	"dv",dv,
	"e ",ex,
	"el",el_,
	"em",em,
	"fl",fl,
	"fo",fo_,
	"ft",ft,
	"gf",gf_,
	"gi",gi_,
	"gm",gm,
	"gs",gs_,
	"gt",gt_,
	"hc",hc,
/*	"hi",hi_, */
	"im",im,
	"in",in_,
	"is",is,
	"lc",lc,
	"lf",lf_,
	"li",li_,
	"lk",lk,
	"me",me,
	"nf",nf,
	"ns",ns,
	"op",op,
	"p ",p,
	"pa",pa,
	"pd",pd_,
	"pf",pf_,
	"ph",ph,
	"pl",pl,
	"pr",pr,
	"q ",quit,
	"rc",rc,
	"ri",rin,
	"rm",rm_,
	"ro",rout,
	"rs",rs_,
	"rt",rt,
	"sc",sc,
	"sl",sl,
	"sp",sp,
	"sq",sq_,
	"st",st,
	"th",th,
	"tp",tp,
	"uf",uf,
	"up",up,
	"us",us_,
	"v ",v,
	"vc",vc,
	"vm",vm,
	"wc",wc,
	"wf",wf,
	"wr",wr_,
	"wu",wu,
	"xf",xf,
/*	"xx",xx, /* MWP uses for testing new commands */
};

numcmds()
{
	return(sizeof(icmd) / sizeof(Icmd));
}
