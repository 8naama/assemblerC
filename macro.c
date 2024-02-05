#include "macro.h"


int isMacro(char line[]) /*Checks whether it is the beginning of a macro or the end of a macro*/
{
	int index = 0 , mIndex = 0 ;
	char mcr [MAX];
	memset(mcr , '\0' , MAX);
	while(isspace(line[index]))
	       index ++;
	while (line[index] != '\n' && !isspace(line[index]))
	{
	    	mcr[mIndex] = line[index];
		mIndex++;
		index++;
	}
	if (!strcmp(mcr, "mcr"))
	    	return 1;
	if (!strcmp(mcr, "endmcr"))
	    	return -1;
	return 0;
}
void insertName(struct Macro *pMcr, char line[]) /*Enter the macro name in the macros table*/
{
	int index = 0 , nIndex = 0 ;
	char mName [MAX];
	memset(mName , '\0' , MAX);
	while(isspace(line[index]))
		index ++;
	 /* while (line[index] != '\n' && !isspace(line[index]))
	   	index++;
	  while(isspace(line[index]))
	  	index ++;*/
	 while (line[index] != '\n' && !isspace(line[index]))
	 {
	    	mName[nIndex] = line[index];
	    	nIndex++;
	    	index++;
	 }
	 strcpy(pMcr->name,mName);
}
void insertContent(struct Macro *pMcr, FILE *fp) /*Inserts the macro contents into the macros table*/
{
	char line [MAX];
	char mContent [MAX];
	memset(line , '\0' , MAX);
	memset(mContent , '\0' , MAX);
	fgets(line, MAX, fp);
	while(isMacro(line) != -1)
	{
		strncat(mContent, line , MAX);   
	    	fgets(line, MAX, fp);
	}
	strcpy(pMcr->content,mContent);
}
int readFile(int i, char *argv[],struct Macro *mHead) /*Performing the first pass on the file (inserting the macros into the macro table,
                                                        copying the corresponding rows from the table to the file, etc.)*/
{
	char line [MAX];
	FILE *fpr;
	memset(line , '\0' , MAX);
	fpr = fopen(argv[i],"r");
	if(fpr == NULL)
	{
		printf("error: the file: %s cant open\n \n" , argv[i]);  
	     	return 1;
	}
	while(fgets(line, MAX, fpr))
	{
		struct  Macro* temp = NULL;
		temp = (struct Macro*)malloc(sizeof(struct Macro)); 
		if(isMacro(line) == 1)
		{ 
			insertName(temp , line);
			insertContent(temp , fpr);
			mHead -> next = temp ;
			mHead = temp;
		}
	}
	 return 0;
}
int isMacroCommand(char line[], FILE *fpw,struct Macro *mTail) /*Copy the contents of the corresponding macro to the file from the table, if it is a macro command*/
{
	int index = 0, mIndex = 0;
	char mName [MAX];
	struct  Macro *temp = NULL;
	temp = (struct Macro*)malloc(sizeof(struct Macro));
	temp = mTail;
	memset(mName, '\0' , MAX);
	while(isspace(line[index]))
		index ++;
	while (!isspace(line[index]) && line[index] != '\n')
	{
		mName[mIndex] = line[index];
	    	mIndex++;
	    	index++;
	}
	while (temp != NULL)
	{
		if (!strcmp(temp->name , mName))
	    	{
	    		fprintf(fpw, "%s", temp->content);
	    		return 1;
	    	}
	    	temp = temp ->next;
	}
	return 0;
}


void checkLine(FILE *fpw, struct Macro *tail, char *line) {
	static int mFlag = 0;

	if (!mFlag)
	{
		if (!isMacroCommand(line, fpw, tail))
		{
			if (isMacro(line) == 0)
			{
		        	fprintf(fpw, "%s", line);
		    	}
			else
			{
		        	mFlag = 1;
		    	}
		}
	}
	else
	{
		if (isMacro(line) == -1)
			mFlag = 0;
	}
}

void writeFile(int i, char *argv[], struct Macro *tail) {
	FILE *fpr, *fpw;
    	char line[MAX];
    	char fName1[MAX], fName2[MAX];

    	memset(line, '\0', MAX);

    	strcpy(fName1, argv[i]);
    	strcpy(fName2, argv[i]);
    	strncat(fName2, ".am", 3);

    	fpr = fopen(fName1, "r");
    	fpw = fopen(fName2, "w");

    	if (fpr == NULL)
	{
        	printf("error: can't open the file: %s\n\n", argv[1]);
        	
	}

	while (fgets(line, MAX, fpr))
	{
        	checkLine(fpw, tail, line);
	}

    	fclose(fpw);
    	fclose(fpr);
}
/*void writeFile(int i, char *argv[],struct Macro *tail)
{
	  int mFlag = 0; 
	  FILE *fpw;
	  FILE *fpr;
	  char line [MAX];
	  char fName1[MAX];
	  char fName2[MAX];
	  memset(line , '\0' , MAX);
	  strcpy(fName1,argv[i]);
	  strcpy(fName2,argv[i]);
	  strncat(fName2, ".am", 3);
	  fpr = fopen(fName1,"r");
	  fpw = fopen(fName2,  "w");
	  if(fpr == NULL)
	     printf("error: cant open the file: %s \n \n" , argv[1]); 
	  while(fgets(line, MAX, fpr))
	  {
	    if(!mFlag)
	    {   
	       if(!isMacroCommand(line, fpw, tail))
	       {
		 if(isMacro(line) == 0)
		 {
		    fprintf(fpw,"%s",line);
		 }
		 else 
		 {
		    mFlag = 1;
		 }
	       }
	    }
	    else
	    {
	      if(isMacro(line) == -1)
		mFlag = 0;
	    }        
	  } 
	  fclose(fpw);
	  fclose(fpr);
}
*/

/*void addMacro(struct Macro *mHead , char mName[] , char mContent[]) 
	  struct  Macro *temp = NULL;
	  temp = (struct Macro*)malloc(sizeof(struct Macro));
	  memset(temp->name, '\0' , MAX);
	  memset(temp->content , '\0' , MAX);
	  strcpy(temp->name,name);
	  strcpy(temp->content,content);
	  mHead ->next = temp;
}
void printMacrO(struct Macro *mTail)
{
	  struct  Macro *temp = NULL;
	  temp = (struct Macro*)malloc(sizeof(struct Macro));
	  temp = mTail-> next;
	  while(temp != NULL)
	  {
	    printf("\n macro name:  %s \n  content:  %s \n", temp->mname , temp->mcontent);
	    temp = temp-> next;
	  }
} */
