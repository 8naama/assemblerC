#include "secondRun.h"
#include "firstRun.h"

char* decimalToBinary(int n, int numBits) {
    char *p;
    int i;
    p = (char*)malloc((numBits + 1) * sizeof(char)); 
    if (p == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }

    for (i = 0; i < numBits; i++) {
        p[i] = '0';
    }
    p[numBits] = '\0';

    i = numBits - 1;
    while (n > 0 && i >= 0) {
        p[i] = n % 2 + '0'; 
        n = n / 2;
        i--;
    }

    return p;
}
const char *decimalToBinary12bit(int n) /*Converts the decimal number to a binary number in 12 bits*/
{
  int i;
  char *p;
  char binary[13] = "000000000000";
  for(i=11;n>0;i--)
  {
     binary[i]=n%2 +48 ;   
     n=n/2;
  } 
p=binary;
  return p; 
} 
int opcodeDecimal(char op[]) /*Returns the opcode type in decimal base*/
{

  	if(!strcmp(op, "mov"))
    		return 0;
  	if(!strcmp(op, "cmp"))
   		return 1;
  	if(!strcmp(op, "add")) 
    		return 2;
  	if(!strcmp(op, "sub"))
		return 3;
	if(!strcmp(op, "not"))
		return 4;
	if(!strcmp(op, "clr")) 
		return 5;
 	if(!strcmp(op, "lea"))
    		return 6;
	if( !strcmp(op, "inc"))
		return 7;
	if(!strcmp(op, "dec"))
		return 8;
  	if(!strcmp(op, "jmp"))
    		return 9;
	if(!strcmp(op, "bne"))
		return 10;
  	if(!strcmp(op, "red"))
    		return 11;
  	if(!strcmp(op, "prn"))
    		return 12;
	if(!strcmp(op, "jsr"))
		return 13;
  	if(!strcmp(op, "rts"))
    		return 14;
  	if(!strcmp(op, "hlt"))
    		return 15;

return -1;
}
void opcodeBinary(char op[], struct CodeType *p) /*Returns the binary number of the instructions*/
{

  char opBin[5];
 /* struct CodeType *p=NULL;*/




 memset(opBin , '\0' , 5);

	if(strcmp(op,"mov")==0)

		strcpy(opBin,"0000");

	else
	if(!strcmp(op,"cmp"))
		strcpy(opBin,"0001");
	else
	if(!strcmp(op, "add"))
		strcpy(opBin,"0010");
	else
	if(!strcmp(op, "sub"))
		strcpy(opBin,"0011");
	else
	if(!strcmp(op, "not"))
		strcpy(opBin,"0100");
	else
	if(!strcmp(op, "clr"))
		strcpy(opBin,"0101");
	else
	if(!strcmp(op, "lea"))
		strcpy(opBin,"0110");
	else
	if(!strcmp(op, "inc"))
		strcpy(opBin,"0111");
	else
	if(!strcmp(op, "dec"))
		strcpy(opBin,"1000");
	if(!strcmp(op, "jmp"))
		strcpy(opBin,"1001");
	else
	if(!strcmp(op, "bne"))
		strcpy(opBin,"1010");
	else
	if(!strcmp(op, "red"))
		strcpy(opBin,"1011");
	else
	if(!strcmp(op, "prn"))
		strcpy(opBin,"1100");
	else
	if(!strcmp(op, "jsr"))
		strcpy(opBin,"1101");
	else
	if(!strcmp(op, "rts"))
		strcpy(opBin,"1110");
	else
	if(!strcmp(op, "hlt"))
		strcpy(opBin,"1111");
strcpy(p->opcode, opBin);


}

char* checkWord(char line[], int index )
{
	int windex = 0;
  	char word[MAX];
  	char *p;
  	if(line[index] == '\n')
     		return 0;
  memset(word, '\0' , MAX);
  	while(isspace(line[index]))
		index++;
  	while (!isspace(line[index]) && line[index] != '\n')
      	{
     		word[windex] = line[index];
		windex++;
		index++;
      	}
	/*printf("the word:%s\n", word);*/
	p=word;
	return p;

}



int ifOpcode(char line[], int index ,struct  CodeType* head ) /* Return the index of the opcode */
{
  int oindex = 0;
  char opName[MAX];
strcpy(opName, "");
   while(isspace(line[index]))
      index++;
   while(!isspace(line[index]))
   {
      opName[oindex++] = line[index++];
   }
   checkOpcode(head,opName);
   return index;
}
int countData(char line[] , int index) /*Summarize the number of values in the data prompt*/
{
  int dcounter = 1;
  while (line[index] != '\0')
  {
      if(line[index] == ',')
        dcounter++;
      index++;
  }
  return dcounter ;
}
int countString(char line[] , int index) /*Summarize the number of chars in the string prompt*/
{
  int dataSectionLen = 0;
char key[MAX_LABEL_NAME_LEN], *string;
  string = (char *) malloc(MAX_LINE_LEN-strlen(key)+1);

	sscanf(line, "%[^:]: .string %*[\"“]%[^\"“]%*[\"”]", key, string);

        dataSectionLen += strlen(string) + 1;	

 return dataSectionLen;
}

int DataOrString(char line[] , int index) /*Checks whether this is a data prompt or a string prompt, if so returns the space to be allocated for them*/
{
  int windex = 0;
  char word[MAX];
  if(line[index] == '\n')
     return 0;
strcpy(word, "");

  while(isspace(line[index]))
	index++;
  while (!isspace(line[index]) && line[index] != '\n')
      {
      word[windex++] = line[index++];
      }
	/*printf("%s\n", word);*/
    if(strcmp(".data",word)==0){
        return(countData(line , index));}
    if (!strcmp(".string",word)){
        return(countString(line , index));}
    return 0;
  
}

int checkAddress(char line[]) /*Checks whether it is extern prompt or entry prompt*/
{
  int aindex = 0, index = 0 ;
  char address[MAX];
  if(line[index] == '\n')
     return 0;
strcpy(address, "");
  while(isspace(line[index]))
      index ++;
  while (!isspace(line[index]) && line[index] != '\n')
      {
      address[aindex++] = line[index++];
      }
    if(!strcmp(".entry",address))
        return 1;
    if(!strcmp(".extern",address))
        return 2;
    return 0;
  
}

const char *checkNegative(char num[], int numBits) /* find the Complementary to 2 for our negavie number */
{
  int i , j = numBits-1 , n = atoi(num);
  char *p;
char *binary = (char*)malloc((numBits+1) * sizeof(char));
if (binary == NULL) {
   
    exit(EXIT_FAILURE);
}

memset(binary, '0', numBits);
    binary[numBits] = '\0';

  for (i = numBits - 1; n > 0; i--) {
        binary[i] = (n % 2) + '0';
        n = n / 2;
    }

    for (i = numBits - 1; i >= 0; i--) {
        if (binary[i] == '1') {
            j = i;
            break;
        }
    }

    for (i = j - 1; i >= 0; i--) {
        if (binary[i] == '1') {
            binary[i] = '0';
        } else {
            binary[i] = '1';
        }
    }


    p = binary;
    return p; 
}
int isNegative(char line[], int index)
{

	int minus=0;
	char num[MAX];
	memset(num, '\0' , MAX);
	if(line[index]=='#')
		index++;
        if (line[index] == '-')
        {
              minus = 1;
		return minus;
        }
return minus;
	
   
}
struct Symbol* ifSymbol(char line[], int index)
{
	struct Symbol *p=symbolTableHead;
	char word[MAX];
	int windex=0, num;
	memset(word, '\0', MAX);
	while(!isspace(line[index])&& line[index]!='['&& line[index]!=']' && line[index]!='\n')
	{	
		word[windex]=line[index];
		windex++;
		index++;
	}

	p=findInSymbolsTable(word);
	if(p!=NULL)
	{
		/*num=p->value;*/
		return p;
	}
	return 0;
}
void checkMethod(struct Symbol *s, struct CodeType *p)
{
	if(s->method==relocatable)
		strcpy(p->aer, "10");
	else
		if(s->method==external)
			strcpy(p->aer, "01");
		else
			strcpy(p->aer, "00");
	printf("aer:%s\n", p->aer);
}
char DestinationMode(char line[], int index ,struct  CodeType* head ) /*Checks which address is in the destination*/
{
     char num[MAX], ind;
     int nIndex=0, i, tempindex;


     

     while(isspace(line[index]))
        index++;
     if(line[index] == '#')  /*Immediate address */
     { 
 	 memset(num, '\0' , MAX);
        strcpy(head->daddress,"00");
	ind=otherWordsDest(head, line, index);
	return ind;
     }
     else
     if(line[index] == 'r') /*Direct register address*/
     {
        nIndex=0;
	index++;
       while (line[index] >'0' && line[index] <= '7')
        {
          num[nIndex] = line[index];
		nIndex++;
		index++;
     
        }
        i = atoi(num);
        /*strcpy(head->dregister,decimalToBinary(i, 12));*/
        strcpy(head->daddress,"11");
	strcpy(head->dother,decimalToBinary(i, 3));
	        	strcpy(head->raer,"00");
       return 'r';
     }       
   
     
     else
     {
	
          
	tempindex=index;
      				 while (line[index] != '[' && line[index] != ',' && !isspace(line[index])) /* index address */
       				{
          				num[nIndex] = line[index];
					nIndex++;
					index++;
       				}

       				if (line[index] =='[') 
       				{  
  					memset(num, '\0' , MAX);
         				nIndex = 0;
         				strcpy(head->daddress,"10");
					ind=otherWordsDest(head, line, tempindex);
     					return ind;
      				 }

       else
         strcpy(head->daddress,"01"); /* Direct address */
}

     
    return '0';

}
char SourceMode(char line[], int index ,struct  CodeType* head ) /*Checks which address is in the source*/
{

     char num[MAX], ind;
	struct Symbol *symbol=symbolTableHead;
     int nindex = 0, i, tempindex;
  memset(num, '\0' , MAX);
       
     while(isspace(line[index]))
      index++;
	while(!isspace(line[index]))
     		index++;
while(isspace(line[index]))
      index++;
        printf("poiu\n");
     		if(line[index] == '#')  /*Immediate address */
    		{ 

        		strcpy(head->saddress,"00");
        		ind=otherWordsSource(head, line, index);
			return ind;
			
     		}
    		else

     			if(line[index] == 'r') /*Direct register address*/
     			{
        			index++;
				nindex=0;

        			if(isdigit(line[index]))
					{
						nindex=0;
            						num[nindex] = line[index];

							nindex++;
        						index++;

    					}
				num[nindex]='\0';
        			i = atoi(num);

          			strcpy(head->sother,decimalToBinary(i, 9));
	          		strcpy(head->raer,"00000");
          			strcpy(head->saddress,"11");
				return 'r';
        			/*if(line[index]==',')
				{
					index++;
					DestinationMode(line, index, head);
				}*/
     			}       

       
     			else
     			{
				tempindex=index;
      				 while (line[index] != '[' && line[index] != ',' && !isspace(line[index])) /* index address */
       				{
          				num[nindex] = line[index];
					nindex++;
					index++;
       				}

       				if (line[index] =='[') 
       				{  
  					memset(num, '\0' , MAX);
         				nindex = 0;
         				strcpy(head->saddress,"10");
					/*while(line[index]!=',' && line[index]!='\n')
						index++;
					if(line[index]==',')
					{
						index++;
						DestinationMode(line, index, head);
					}*/
					ind=otherWordsSource(head, line, tempindex);
					return ind;
     					/*return tempindex;*/
      				 }
			
       				else
        					 strcpy(head->saddress,"01");       /* Direct address */
			}
		

       return '0';
}


char otherWordsDest(struct CodeType *head, char line[], int index)
{
	char num[MAX], save;
	struct Symbol *s;
	int i, nIndex, tempindex;
	while(isspace(line[index]))
        index++;
     if(line[index] == '#')  /*Immediate address */
     { 
	save=line[index];
	index++;
	nIndex=0;
 	 memset(num, '\0' , MAX);
	s=ifSymbol(line, index);
			if(s!=0)
			{

				strcpy(head->dother,decimalToBinary(s->value, 12));
				checkMethod(s, head);
				return save;
			}
			else{
				if(isNegative(line, index)==0)
				{
					while (line[index] >='0' && line[index] <= '9')
        				{
          					num[nIndex] = line[index];
						nIndex++;
						index++;
     					}
       
				strcpy(head->dother,decimalToBinary(atoi(num), 12));
			        strcpy(head->aer,"00");
				return save;
				}
				else{

					index++;
					while (line[index] >='0' && line[index] <= '9')
        				{
          					num[nIndex] = line[index];
						nIndex++;
						index++;
     
        				}
        				strcpy(head->dother,checkNegative(num, 12));
				        strcpy(head->aer,"00");
					return save;
				}
			}
	
     }
     else
     if(line[index] == 'r') /*Direct register address*/
     {
  	memset(num, '\0' , MAX);
        index++;
	nIndex=0;
        if (line[index] >'0' && line[index] <= '7')
        {
          num[nIndex] = line[index];
		nIndex++;
		index++;
     
        }
        i = atoi(num);
        strcpy(head->dother,decimalToBinary(i, 12));
        strcpy(head->aer,"00");
     }       
     else 
     {
	nIndex=0;
	memset(num, '\0',  MAX);
	s=ifSymbol(line, index);
	if(s!=0)
	{     	
		strcpy(head->dother,decimalToBinary(s->value, 12));
		checkMethod(s, head);
		head->dother[13]='\0';
		while (line[index] != '[' &&line[index]!=',' && line[index]!='\n')
			index++;
		if (line[index] =='[') 
       		{  
  			memset(num, '\0' , MAX);
         		nIndex = 0;
			save=line[index];

			index++;
			tempindex=index;
			while(line[tempindex]!=']')
			{
				num[nIndex]=line[tempindex];
				nIndex++;
				tempindex++;
			}
				s=ifSymbol(line, index);
			if(s!=0)
			{

				strcpy(head->dother1,decimalToBinary(s->value, 12));
				checkMethod(s, head);
				return save;
			}
			else
			{
				strcpy(head->dother1,decimalToBinary(atoi(num), 12));
				return save;
			}
		}
	

     }
    
     }return '0';
}
char otherWordsSource(struct CodeType *head, char line[], int index)
{
	char num[MAX], save;
	int i, nIndex, tempindex;
	struct Symbol *s;
	while(isspace(line[index]))
        index++;
     if(line[index] == '#')  /*Immediate address */
     { 
	save=line[index];
	index++;
	nIndex=0;
 	 memset(num, '\0' , MAX);
	s=ifSymbol(line, index);
			if(s!=0)
			{

				strcpy(head->sother,decimalToBinary(s->value, 12));
				checkMethod(s, head);
				return save;
			}
			else{
				if(isNegative(line, index)==0)
				{
					while (line[index] >='0' && line[index] <= '9')
        				{
          					num[nIndex] = line[index];
						nIndex++;
						index++;
     					}
       
				strcpy(head->sother,decimalToBinary(atoi(num), 12));
        			strcpy(head->aer,"00");
				return save;
				}
				else{

					index++;
					while (line[index] >='0' && line[index] <= '9')
        				{
          					num[nIndex] = line[index];
						nIndex++;
						index++;
     
        				}
        				strcpy(head->sother,checkNegative(num, 12));
		       			strcpy(head->aer,"00");
					return save;
				}
			}
	
	
     }
     else
     if(line[index] == 'r') /*Direct register address*/
     {
  	memset(num, '\0' , MAX);
        index++;
	nIndex=0;
        if (line[index] >'0' && line[index] <= '7')
        {
          num[nIndex] = line[index];
		nIndex++;
		index++;
     
        }
        i = atoi(num);
        strcpy(head->sother,decimalToBinary(i, 12));
	printf("oo:%s\n", head->sother);
        strcpy(head->aer,"00");
     }       
     else 
     {
	nIndex=0;
	memset(num, '\0',  MAX);
	s=ifSymbol(line, index);
	if(s!=0)
	{     	
		strcpy(head->sother,decimalToBinary(s->value, 12));
		checkMethod(s, head);
		head->sother2[13]='\0';
		while (line[index] != '[' &&line[index]!=',' && line[index]!='\n')
			index++;
		if (line[index] =='[') 
       		{  	save=line[index];
  			memset(num, '\0' , MAX);
         		nIndex = 0;
			index++;
			tempindex=index;
			while(line[tempindex]!=']')
			{
				num[nIndex]=line[tempindex];
				nIndex++;
				tempindex++;
			}
				s=ifSymbol(line, index);
			if(s!=0)
			{

				strcpy(head->sother2,decimalToBinary(s->value, 12));
				checkMethod(s, head);
				return save;
			}
			else
			{
				strcpy(head->sother2,decimalToBinary(atoi(num), 12));
				return save;
			}
		}
	

     }
    
} return '0';
}

void secondRun(char filename[])
{	
	int index=0, tempindex=0 ,datavalue, i,number, counter=100, nindex=0;
	FILE *fpr;
	struct CodeType* p=NULL;
    	struct Symbol *psym;
	char line [MAX_LINE_LEN], *fullFileName, num[MAX], *word, sind, dind;
	char  printWord[14];
  	/*psym = (struct Symbol*)malloc(sizeof(struct Symbol));*/
  	p = (struct CodeType*)malloc(sizeof(struct CodeType));
	psym = symbolTableHead;
    	fullFileName = (char *) malloc(strlen(filename)+strlen(READ_FILE_TYPE)+1);
    	/*fullFileName2 = (char *) malloc(strlen(filename)+strlen(READ_FILE_TYPE2)+1);*/
    	sprintf(fullFileName, "%s%s", filename, READ_FILE_TYPE);
	/*sprintf(fullFileName2, "%s%s", filename, READ_FILE_TYPE2);*/
    	fpr = fopen(fullFileName, "r");
	/*fpw = fopen(fullFileName2,"w");*/
	/*p=p->next;*/
    	/* if failed to open the file, throws error */
    	if (!fpr)
	{
        printf("error: the file: %s can't open\n" , fullFileName);
   	}

    	/* reading lines from the file */
    	while (fgets(line, MAX_LINE_LEN, fpr)) 
	{

        	printf("Reading line:%s\n", line); 
		if(psym->type==mdefine){

			while(!isspace(line[index]))			
				index++;

		}
		if(psym->method==external|| psym->method==external){

			while(!isspace(line[index]))			
				index++;

		}

		/*while(!isspace(line[index]) && line[index]!='\n')
		{
			symbol[nindex]=line[index];
			nindex++;
			index++;
		}
		printf("symbol:%s\n", symbol);
		if(_startsWith(symbol, ".define")==1)
			while(!isspace(line[index]))			
				index++;*/
		datavalue = DataOrString(line, index);
   		if ( datavalue > 0)   /* if we have a data or string order in the line */
    		{

        		while(isspace(line[index]))
				index++;

			if(line[index + 1] == 's')
			{	    	
	        		while(!isspace(line[index]))
					index++;
	        		while(isspace(line[index]))
					index++;
				index=index+3;

				for ( i = 1; i < datavalue; i++)
           			{	
               				strcpy(printWord,decimalToBinary(line[index], 14));

               				printf("%d\t%s\n", counter, printWord);
               				index++;
               				counter++;
           			} 
				printf("%d\t%s\n", counter, "00000000000000");
				counter++;
			}
			else  /* It's data order */
      			{

        			while(!isspace(line[index]))
					index++;
				while(isspace(line[index]))
					index++;

        			for ( i = 0; i < datavalue; i++)
       	 			{

					memset(num, '\0', MAX);
					nindex=0;
					if(isNegative(line, index)==0)
					{
					while(line[index] >='0' && line[index] <= '9')
       					{
            					num[nindex] = line[index];
						nindex++;
						index++;
           
      					}
               				strcpy(printWord,decimalToBinary(atoi(num), 14));
					}
					else
					{
						index++;
						while(line[index] >='0' && line[index] <= '9')
       						{
            					num[nindex] = line[index];
						nindex++;
						index++;
           
      						}
	            				strcpy(printWord,checkNegative(num, 14));
 					}
					printf("%d \t %s\n", counter, printWord);
            				counter++;
					while(line[index]!=',')
						index++;
					if(line[index]==',')
						index++;
           				while(isspace(line[index]))
           					index++;

   					if(isalpha(line[index]))
					{
						nindex=0;

    						while (isalpha(line[index]))
						{
            						num[nindex] = line[index];

							nindex++;
        						index++;

    						}
						num[nindex]='\0';
						psym=symbolTableHead;
						psym=findInSymbolsTable(num);
						if(psym)
							strcpy(printWord,decimalToBinary(psym->value, 14));
            					printf("%d \t %s\n", counter, printWord);
						counter++;
						break;
					}	
        		
          				
        			}
     			 } 

     			/*p = p->next; */
		}
		/*else   
    			if((!(checkAddress(line) == 2)))    
      			{ 
        			if(checkAddress(line) == 1)   
        			{
                					printf("lll4");
           				strcpy(symbol, "");
           				index = 0;
           				sindex = 0;
           				shead = symbolTableHead; 

           				while(isspace(line[index]))
						index++;
           				while(!isspace(line[index]))
           				{
              					symbol[sindex++] = line[index++];

           				}
           				while (shead != NULL && !extFlag)
          				{
             					if (!strcmp(shead->name,symbol))
             					{
                					shead->method ++;
                					extFlag++;
             					}
             					shead =shead->next;  
           				}
          				if(!extFlag)
             					printf("\n  error! the symbol : %s dosent exist \n",symbol);
          				extFlag = 0;
        			}*/
				else   /*If it's instruction line */
        			{
					word=NULL;
					i=-1;
					word=checkWord(line, index);	

					if(opcodeDecimal(word)!=-1)
						opcodeBinary(word, p);
					 strcpy(p->startFirst,"0000");
        				 strcpy(p->endFirst,"00");

					if(_isCode(line)==code1)
					{
  						/*psym = (struct Symbol*)malloc(sizeof(struct Symbol));*/
						
  						
						/*destSymbol(p, line, index);*/
						while(isspace(line[index]))
        						index++;
	
     						while(!isspace(line[index]))
        						index++;
						DestinationMode(line, index, p);
						strcpy(p->saddress, "00");
						otherWordsDest(p, line, index);
          					printf("%d\t%s%s%s%s%s\n",counter, p->startFirst, p->opcode, p->saddress, p->daddress, p->endFirst);
						counter++;
						printf("%d\t%s%s\n", counter, p->dother, p->aer);
						counter++;
					}
					else
						if(_isCode(line)==code0)
						{
							strcpy(p->saddress, "00");
							strcpy(p->daddress, "00");
						}
						else{

							sind=SourceMode(line, index, p);

							p->sother[13]='\0';
							p->dother[13]='\0';
							
 
							while(line[index]!=','&& line[index]!='\n')
								index++;
		 					if(line[index]==',')
							{
								index++;
								dind=DestinationMode(line, index, p);
								printf("%d\t%s%s%s%s%s\n",counter, p->startFirst, p->opcode, p->saddress, p->daddress, p->endFirst);
          							counter++;
								if(sind=='r' && dind=='r')
								{
									printf("%d\t%s%s%s\n",counter, p->sother, p->dother, p->endFirst);
              								counter++;
								}
								else{
									if(sind=='r' && dind!='r') 
									{
										printf("%d\t%s%s\n",counter, p->sother, p->raer);
              									counter++;
									}
									else
									
										if(sind!='r' && dind=='r')
										{
										printf("%d\t%s%s\n",counter, p->dother, p->raer);
              									counter++;
										}
										printf("%d\t%s%s\n",counter, p->sother, p->aer);
              									counter++; 
										printf("%d\t%s%s\n",counter, p->dother, p->aer);
              									counter++;
								
								
									if(sind=='[' && dind=='[')
									{
									printf("%d\t%s%s\n",counter, p->sother, p->aer);
              								counter++; 
									printf("%d\t%s%s\n",counter, p->sother2, p->endFirst);
              								counter++;
									printf("%d\t%s%s\n",counter, p->dother, p->aer);
									counter++;
									printf("%d\t%s%s\n",counter, p->dother1, p->endFirst);
									counter++;

									}
									else
									{


										
										if(dind=='[')
										{
							
											printf("%d\t%s%s\n",counter, p->dother1, p->aer);
											counter++;
										}		
									}
						}
								p->dother1[13]='\0';
							}
						}
					
              				
        			 
     		}

      		index = 0 ;


  	}
  	fclose(fpr);
  	/*fclose(fpw);*/

          
}

        
           
          
