%{
	#include <stdio.h>
	#include "y.tab.h"
%}
DIGIT [0-9]
%%				
{DIGIT}+(\.{DIGIT}+)?		{yylval.dval = atof(yytext); return NUMBER;}
[ \t]						;
\n							yyterminate();
.							return yytext[0];
%%
int yywrap()
{
	return 1;
}
