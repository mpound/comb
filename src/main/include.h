	/************************************/
	/* maximum length of command string */
	/************************************/

	/*********************/
	/* command structure */
	/*********************/

typedef struct
{
	char cmd[4];		/* name of command */
	void (*cp)();		/* routine to be called when command */
				/* is invoked */
}
Icmd;

# include <setjmp.h>
typedef struct
{
	jmp_buf sjbuf;		/* Place to jmp to when aborting */
	jmp_buf retbuf;		/* Peaceful return */
	int intflg;		/* Set non zero if an interrupt is being
				held off */
	int depth;		/* > 0 if interrupts are to be held off.
				noint_() increments depth and yesint_()
				decrements it.  Inton_() sets depth to zero. */
	char *name;		/* The name used to run comb - argv[0] */
} INTERRUPT;
extern INTERRUPT interrupt;
