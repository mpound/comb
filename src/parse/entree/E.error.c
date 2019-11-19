# include "E.constant.h"
# include "E.struct.h"
# include "E.extern.h"

enterr(mess)
	char *mess;
{
	printf("Error on line %d: %s\n",clin,mess);
}

yyerror()
{
	enterr("syntax error");
}
