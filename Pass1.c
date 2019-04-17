#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct Table{
	char name[20];
	int add;
}Lit[20],Sym[20];

int search(char table[10][10], char word[20] ){
	int i = 0;
	for(i = 1; i < 10; i++)
		if(!strcmp(word,table[i]))
			return i;
	return 0;
}

int getadd(char *s, int ptr){
	int i = 0;
	for(i = 1; i < ptr; i++)
		if(!strcmp(s,Sym[i].name))
			return Sym[i].add;
	return 0;
}

int getindex(char *s, int ptr){
	int i = 0;
	for(i = 1; i < ptr; i++)
		if(!strcmp(s,Sym[i].name))
			return i;
	return 0;
}

int main()
{
	FILE *fin = fopen("input.txt", "r");
	FILE *fout = fopen("out.txt", "w");
	FILE *ST = fopen("ST.txt", "w");
	FILE *LT = fopen("LT.txt", "w");
	
	char MOT[10][10] = {"STOP", "SUB", "ADD", "MUL", "DIV", "MOVEM", "MOVER", "BC", "PRINT", "READ"};
	char REG[10][10] = {" ", "AREG", "BREG", "CREG", "DREG"};
	char COMP[10][10] = {"LE", "LT", "NE", "GE", "GT"};
		
	char word[5][20], line[100];
	int wc, loc, i, lptr = 1, sptr =1, add_lptr = 1, j, flag = 0;
	
	while(fgets(line, 100, fin)){						    /* reads a line*/	
		wc =  sscanf(line,"%s%s%s%s", word[0], word[1], word[2], word[3]);  /* read input from the string. */
		
		if(!strcmp(word[0],"START")){
			fprintf(fout, "    (AD,01) (C,%s)", word[1]); 		
			loc = atoi(word[1]);
		}
		
		else if(!strcmp(word[0],"LTORG"))
		{
			fprintf(fout, "\n    (AD,05)");
			for(i = add_lptr; i < lptr; i++)
				Lit[i].add = loc++;
			add_lptr = lptr;
		}
		
		else if(!strcmp(word[0], "ORIGIN"))
		{
			fprintf(fout, "\n    (AD,03) (S,%d)   (C,%s)", getindex(word[1], sptr), word[2]);
			loc = getadd(word[1], sptr) + atoi(word[2]);
		}
		
		else if(!strcmp(word[1],"EQU"))
		{
			i = getindex(word[0], sptr);
			Sym[i].add = getadd(word[2], sptr) + atoi(word[3]);
		}
		
		else if(!strcmp(word[0],"END"))
		{
			for(i = add_lptr; i < lptr; i++)
				Lit[i].add = loc++;
			add_lptr = lptr;
			break;
		}
		
		else if(!strcmp(word[1],"DS"))
		{
			strcpy(Sym[sptr].name, word[0]);
			Sym[sptr].add = loc;
			sptr ++ ;
			fprintf(fout, "\n%3d (DL,01) (C,%3d)",loc, atoi(word[2]));
			loc += atoi(word[2]);
		}
		
		else if(!strcmp(word[1],"DC"))
		{
			i = getindex(word[0], sptr);
			fprintf(fout, "\n%3d (DL,02) (C,%3d)",loc, atoi(word[2]));
			if(i == 0)
			{
				strcpy(Sym[sptr].name, word[0]);
				Sym[sptr].add = loc++;
			}
			else
				Sym[i].add = loc++;				
		}
		else{
			fprintf(fout, "\n%3d", loc);		
			for(i = 0; i < wc; i++){
				if(search(MOT, word[i]))
					fprintf(fout, " (IS,0%d)", search(MOT, word[i]));
			
				else if(search(REG, word[i]))
					fprintf(fout, " (REG,%d)", search(REG, word[i]));
				
				else if(search(COMP, word[i]))
					fprintf(fout, " (COMP,%d)", search(COMP, word[i]));
			
				else if(word[i][0] == '=')
				{
					flag = 0;
					for(j = add_lptr; j < lptr ;j++){
						if(!strcmp(word[i],Lit[j].name))
						{
							flag = 1;
							fprintf(fout, " (L,%d)", j);
						}
					}				
					if(flag == 0)
					{
						fprintf(fout, " (L,%d)", lptr);
						strcpy(Lit[lptr++].name, word[i]);
					}
				}
				else 
				{
					flag = 0;
					for(j = 1; j < sptr ;j++){
						if(!strcmp(word[i],Sym[j].name)){
							flag = 1;
							fprintf(fout, " (S,%d)", j);
						}
					}
					if(flag == 0)
					{
						if(i==0)
							Sym[sptr].add = loc;
						else
							fprintf(fout," (S,%d)", sptr);
						strcpy(Sym[sptr++].name, word[i]);
					}	
				}
			}
			loc++;
		}
	}	
	i = 0;
	for(i = 1; i < sptr; i++)
	{
		fprintf(ST,"\n%s\t%d", Sym[i].name, Sym[i].add);
	}
	printf("\n\n");
	for(i = 1; i < lptr; i++)
	{
		fprintf(LT,"\n%s\t%d", Lit[i].name, Lit[i].add);
	}
}
