%{
	#include <stdio.h>
	#include "y.tab.h"
%}
ALPHA [A-Za-z]
DIGIT [0-9]
%%
while						return WHILE;
{ALPHA}({ALPHA}|{DIGIT})*	return VARIABLE;
{DIGIT}+					{yylval.ival = atoi(yytext); return NUMBER;}
[ \t]						;
\n 							yyterminate();
.							return yytext[0];
%%
int yywrap()
{
	return 1;
}

