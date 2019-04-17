%{
	#include <math.h>
	#include "lex.yy.c" 
%}
%union{
	double dval;
};
%token <dval> NUMBER
%left '+' '-'
%left '*' '/'
%right '^'
%type <dval> expr
%%
S		:	expr			{ printf("%f\n", $1);}
		;
expr	:	expr '+' expr	{ $$ = $1 + $3;}
		|	expr '-' expr	{ $$ = $1 - $3;}
		|	expr '*' expr	{ $$ = $1 * $3;}
		|	expr '/' expr	{ if($3 == 0)
								yyerror("Divide by zero");
							else
								$$ = $1 / $3;
						  	}
		|	expr '^' expr	{ $$ = pow($1, $3);}
		|	'-' expr	 	{ $$ = -$2;}
		|	'(' expr ')'	{ $$ = $2;}
		|	NUMBER			//{printf("%f", $1);} 		
		;

%%
int main()
{
	//printf("Enter the expression : \n");
	yyparse();
}
void yyerror(char *s)
{
	fprintf("%s\n", s);
}
