#include "../stacks/C.h"
#define MATCH 0
#define GET1 1
#define GET2 2
#define DIFF 3

typedef struct {
	union FILE {
		int name;
		short fn[2];
	} file;
	short number;
}SCANNUMBER;

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* scanlistsame.c */
static int ScanInStk2 P_((register SCANNUMBER *sn));
#undef P_

/***********************************************************
 * ScanListSame - compare the scan lists in stacks 1 and 2 *
 * return 0 if completely different, +n if there are n     *
 * matches, -1 if identical.				   *
 ***********************************************************/

int ScanListSame()
{
	int identical = -1;
	int numberSame = 0;
	SCANNUMBER sn1;
	register short *sp = &stk_[0].nstk[0];
	register int cnt = stk_[0].indx;
	register int state;

	sn1.file.name = 0x4b4e8055;	/* 'UNK' coded as in stk header */
	state = MATCH;
	while(--cnt >= 0) {
		if(*sp < -32000)
			state = GET1;
		switch(state) {
		case MATCH:
			sn1.number = *sp++;
			if(ScanInStk2(&sn1))
				numberSame++;
			else
				identical = 0;
			break;
		case GET1:
			sn1.file.fn[0] = *sp++;
			state = GET2;
			break;
		case GET2:
			sn1.file.fn[1] = *sp++;
			state = MATCH;
			break;
		}
		if(sp == &stk_[0].nstk[32])
			sp = &stk_[0].nstkx[0];
	}
	return((identical)? -1: numberSame);
}

static int ScanInStk2(SCANNUMBER *sn)
{
	union FILE file2;
	register short *sp = &stk_[1].nstk[0];
	register int cnt = stk_[1].indx;
	register int state;

	file2.name = 0x4b4e8055;	/* 'UNK' coded as in stk header */
	if(file2.name == sn->file.name)
		state = MATCH;
	else
		state = DIFF;
	while(--cnt >= 0) {
		if(*sp < -32000)
			state = GET1;
		switch(state) {
		case MATCH:
			if(*sp++ == sn->number)
				return(1);
			break;
		case GET1:
			file2.fn[0] = *sp++;
			state = GET2;
			break;
		case GET2:
			file2.fn[1] = *sp++;
			if(file2.name == sn->file.name)
				state = MATCH;
			else
				state = DIFF;
			break;
		case DIFF:
			sp++;
			break;
		}
		if(sp == &stk_[1].nstk[32])
			sp = &stk_[1].nstkx[0];
	}
	return(0);
}
