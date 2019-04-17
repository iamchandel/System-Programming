#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct MNT
{
	char mname[20];
	int  mdtp;
}mnt[5];

struct MDT
{
	char opcode[15],rest[35];
}mdt[30];

char arglist[15][15],apt[10][15];
int  mdtp=0,mntp=0,arglistp=0;
char FName[20], TName[20];
char Buffer[80], temp[40],tok1[40];
int pp,kpp;// no. of positional and keyword parameters
FILE *fp1, *fp2;

int SearchMNT(char *s)
{
	int i;
 	for(i=0; i<mntp; i++)
  		if(strcmp(s,mnt[i].mname)==0)
   			return(i);
 	return(-1);
}

int SearchPNT(char *s)
{  
	int i;
 	for(i=0;i<arglistp;i++)
  		if(strcmp(arglist[i],s)==0)
   			return(i);
 	return(-1);
}

void Print_MNT()
{  
	int i;
 	printf("\n\n----------MACRO NAME TABLE---------------------");
 	printf("\n#\tMName\t#MDTP");
 	printf("\n-----------------------------------------------");
 	for(i=0;i<mntp;i++)
		printf("\n%d\t%s\t%d",i,mnt[i].mname,mnt[i].mdtp);
 	printf("\n------------------------------------------------");
}

void Print_PNT()
{  
	int i;
 	printf("\n\n------PARAMETER NAME TABLE--------");
 	printf("\n#\tPName");
 	printf("\n----------------------------------");
 	for(i=0;i<arglistp;i++)
  		printf("\n%d\t%s\t%s",i,arglist[i],apt[i]);
 	printf("\n----------------------------------");
}

void Print_MDT()
{    
	int i;
 	printf("\n\n-----------MACRO DEFINITION TABLE--------------");
 	printf("\nOpcode\tRest");
 	printf("\n-----------------------------------------------");
 	for(i=0;i<mdtp;i++)
		printf("\n%d\t%s\t%s",i,mdt[i].opcode,mdt[i].rest);
 	printf("\n-----------------------------------------------\n");
 
}
char * nexttoken(char *str , char *token)
{
	int i;
	while(*str==' ')
	str++;
	if(*str==',' || *str=='=')
	{
		*token=*str;
		token++;
		str++;
		*token='\0';
		return(str);
   	}
	while(isalnum(*str) || *str=='#' || *str=='&')
	{
		*token=*str;
  	    token++;
     	str++;
   	}
 	*token='\0';
 	return(str);
}

void make_arglist(char *s)
{
	int k;
	pp=kpp=0;//no. of postional and keyword parameters
	arglistp=0;
	while(*s)
	{
		s=nexttoken(s,temp);
  		k = SearchPNT(temp+1);
  		if(k==-1)
    		strcpy(arglist[arglistp++],temp+1);
  		else
    	{
			printf("\nError: Multiple Declaration of Symbol %s in Argument List",temp);
    		exit(0);
    	}
  		s=nexttoken(s,temp);
  		if(*temp=='=')
     	{ 
     		kpp++;
			break;  //handle keyword parameter
		}
  		pp++;
	}
	
	if(*temp=='=')//handle keyword parameter
		while(*s)
  		{
   			s=nexttoken(s,temp);
   			if(*temp!=',' )
          		strcpy(apt[arglistp-1],temp);
   			else
          		strcpy(apt[arglistp-1],"");
   			s=nexttoken(s,temp);
   			if(*temp=='\0')
      			 break;
   			k = SearchPNT(temp+1);
   			if(k==-1)
				strcpy(arglist[arglistp++],temp+1);
   			else
   			{
   				printf("\nError: Multiple Declaration of Symbol %s in Argument        List",temp);
   				exit(0);
   			}
  		}
}

void Expand(int n)
{
	int a;
 	int MEC;
	char *pointer;
 	MEC = mnt[n].mdtp+1;
 	while(strcmp(mdt[MEC].opcode,"MEND")!=0)
 	{               
 		fprintf(fp2,"+%s  ",mdt[MEC].opcode);
  		pointer=mdt[MEC].rest;
   		pointer=nexttoken(pointer,tok1);
   		while(tok1[0]!='\0')
     	{
			if(tok1[0]=='#')
    		{
       			a=atoi(tok1+1);
       			fprintf(fp2,"%s",apt[a]);
     		}
         	else
       			fprintf(fp2,"%s",tok1);
         	pointer=nexttoken(pointer,tok1);
      	}
   		fprintf(fp2,"\n");
   		MEC++;
  	}
}

void main()

{
 	int i=0,j=0,k=0,n;
 	char *pointer;//pointer for the array buffer
 	
 	printf("\nEnter Source File Name: ");
 	scanf("%s",FName);
 	printf("\nEnter Target File Name: ");
 	scanf("%s",TName);
 	if((fp1=fopen(FName,"r"))==NULL)
	{
 		printf("\nCannot Open Source File...%s",FName);
  		exit(0);
 	}
 	if((fp2=fopen(TName,"w"))==NULL)
 	{
  		printf("\nCannot Create Intermediate File...%s",TName);
  		exit(0);
 	}
 		
 	while(fgets(Buffer,80,fp1))
 	{
  		pointer=Buffer;
  		nexttoken(pointer,tok1);
  		if(strcmp(tok1,"MACRO")==0)
  		{
   			fgets(Buffer,80,fp1); //read the parameter line
   			pointer=nexttoken(pointer,tok1);
   			strcpy(mnt[mntp].mname,tok1);
   			mnt[mntp].mdtp = mdtp;
   			strcpy(mdt[mdtp].opcode,tok1);
   			strcpy(mdt[mdtp].rest,pointer);
   			mdtp++;
   			make_arglist(pointer);
   			mntp++;

     		while(fgets(Buffer,80,fp1)) //store the body of the macro
        	{
   				pointer=Buffer;
   				pointer=nexttoken(pointer,tok1);
   				if(strcmp(tok1,"MEND")==0)
      			{
    				strcpy(mdt[mdtp].opcode,"MEND");
    				strcpy(mdt[mdtp++].rest,"");
    				arglistp=0;;
    				break;
       			}
   				else
      			{
       				strcpy(mdt[mdtp].opcode,tok1);
       				strcpy(mdt[mdtp].rest,"");
       				pointer=nexttoken(pointer,tok1);
       				while(tok1[0]!='\0')
         			{
         		 		if(tok1[0]=='&')
     					{
        					k = SearchPNT(tok1+1);
      						if(k==-1)
        					{
      								printf("\nError: Parameter %s not found",tok1+1);
      								exit(0);
         					}
       						temp[0]='#';
       						temp[1]=k+48;//convert to ASCII
       						temp[2]='\0';
       						strcat(mdt[mdtp].rest,temp);
       					}
    					else
       						strcat(mdt[mdtp].rest,tok1);
    					pointer=nexttoken(pointer,tok1);
         			}
     			}
        		mdtp++;
     		}
     	}
		else
		{
			k = SearchMNT(tok1);
      		if(k==-1)
   				fprintf(fp2,"%s",Buffer);
      		else
        	{
   				arglistp=0;
   				pointer=mdt[mnt[k].mdtp].rest;
   				make_arglist(pointer);
   				pointer=nexttoken(pointer,tok1);
   				//Handle positional parameters
   				pointer=Buffer;
   				pointer=nexttoken(pointer,tok1);//skip macro name
   				for(i=0;i<pp;i++)
     			{
        			pointer=nexttoken(pointer,tok1);
        			strcpy(apt[i],tok1);
        			pointer=nexttoken(pointer,tok1);//skip ,
     			}
 				//Handle keyword parameters
   				pointer=nexttoken(pointer,tok1);
   				
   				while(tok1[0]!='\0')
     			{
        			j=SearchPNT(tok1);//get location of the keyword parameter
        			//get the new value of the keyword parameter
        			pointer=nexttoken(pointer,tok1);//skip =
        			pointer=nexttoken(pointer,tok1);
        			strcpy(apt[j],tok1);
        			pointer=nexttoken(pointer,tok1);//skip ,
        			pointer=nexttoken(pointer,tok1);//read next parameter
     			}
     			Print_PNT();
    			 Expand(k);
  			}//macro expansion

  		}
	}
	Print_MNT();
	Print_MDT();
	fcloseall();
}

