%{
	#include <stdio.h>
	#include <ctype.h>
	#include "lex.yy.c" 
	
%}
%union{
	int ival;
};
%token NUMBER VARIABLE WHILE
%right '='
%left  '+' '-'
%left  '*' '/'
%%

S		:	WHILE { label1(); } '(' E ')' { label2(); } E ';' { label3(); }
		;

E		:	V '=' { push();} E { assignment_code_generator();}
		|	E '+' { push();} E { code_generator();}
		|	E '-' { push();} E { code_generator();}
		|	E '*' { push();} E { code_generator();}
		|	E '/' { push();} E { code_generator();}
		|	'(' E ')'
		|	V
		| 	NUMBER {push();}
		;

V		:	VARIABLE {push();}
		;
%%


char stack[100][10], _id[2] = "0", temp[3] = "t";
int top = 0, start = 1, lnum = 1;

int main()
{
	printf("Enter the expression : \n");
	yyparse();
	return 0;
}
void yyerror(char *s)
{
	fprintf("%s\n", s);
}
int push()
{
	strcpy(stack[++top], yytext);
}

int code_generator()
{
	strcpy(temp, "t");
	strcat(temp, _id);
	printf("%s = %s %s %s\n", temp, stack[top-2], stack[top-1], stack[top]);
	top-=2;
	strcpy(stack[top], temp);
	_id[0]++;
}
int assignment_code_generator()
{
	printf("%s = %s \n", stack[top-2], stack[top]);
	top-=2;
}

int label1()
{
	printf("L%d : \n",lnum);
	lnum++;
}

int label2()
{
	strcpy(temp, "t");
	strcat(temp, _id);
	printf("%s = not %s\n", temp, stack[top]);
	printf("if %s goto L%d \n", temp, lnum);
	_id[0]++;
}

int label3()
{
	printf("goto L%d \n",start);
	printf("L%d : \n", lnum);
}

