/*
E->E+T |T
T->T*V|V
V->i|(E)
*/
//After removal of left recurion
/*
E->TE'
E'->+TE'/epsilon
T->VT'
T'->*VT'/epsilon
V->(E)/i
*/
#include<stdio.h>
#include<ctype.h>
#include<string.h>

void Tprime();
void Eprime();
void E();
void V(); 
void T();
 

char expression[10];
int count, error;

int main()
{
      count = 0;
      error = 0;
      printf("\nEnter a String:\t");
      scanf("%s", expression);
      E();
      if((strlen(expression) == count) && (error == 0))
      {
            printf("\nThe String %s is Accepted\n", expression);
      }
      else 
      {
            printf("\nThe String %s is Invalid\n", expression);
      }
}
                    
void E()
{
	printf("\nE->TE'\n");
      T();
      Eprime();
}

void T()
{
	printf("\nT->VT'\n");
      V();
      Tprime();
}

void Eprime()
{
	printf("\nE->+TE'\n");
      if(expression[count] == '+')
      {
            count++;
            T();
                        Eprime();
      }
}

void Tprime()
{
	printf("\nT->*VT'\n");
      if(expression[count] == '*')
      {
            count++;
            V();
            Tprime();
      }
      
      
}

void V()
{
	printf("\nV->a\n");
      if(isalnum(expression[count]))
      {
            count++;
      }
   
      else if(expression[count] == '(')
      {
         	count++;
            E();
            if(expression[count] == ')')
            {
                  count++;
            }
            else
            {
                  error = 1; 
            }
      }         
      else
      {
            error = 1;
      }
}
