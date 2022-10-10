#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct instruction
{
    char name[20];
    char opcode[20];
    char operand1[20];
    int adress;
    
};

struct optab{
    char opcode[20];
    int value;
};

char label[100][100];
int labelCount=0;

struct optab optab[22];

int labelNotFound(char a[20])
{
    int i;
    for(i=0;i<labelCount;i++)
    {
        if(strcmp(a,label[i])==0)
        {
            return 0;
        }
    }
    
    return 1;
}

int createSymbolTable(struct instruction inst,FILE *fp)
{
    
   int flag=0;
    
        if(strcmp(inst.name," "))
        {
            flag=labelNotFound(inst.name);
            if(flag){
            strcpy(label[labelCount++],inst.name);
            
            fprintf(fp,"%x %s\n",inst.adress,inst.name);
            }

            else
            {
                //fprintf(fp,"Error: Duplicate label");
                printf("Error: Duplicate Label\n");
                //exit(0);
            }
        }

        return flag;
    
}



void createOptab()
{
    FILE *fp=fopen("Optab.txt","r");
    if(fp==NULL)
    {
        printf("File not found");
        return;
    }
    int i=0;
    char a[10];
    while(!feof(fp))
    {
        fscanf(fp,"%s %s",optab[i].opcode,a);
        optab[i].value=strtol(a,NULL,16);
        i++;
    }
}

int addAdress(char b[100],char c[100]){
    int i=0;
    for(int i=0;i<22;i++)
    {
        if(strcmp(optab[i].opcode,b)==0)
        {
            return 3;
        }
    }
    if(!strcmp(b,"RESW"))
    {
        return 3*strtol(c,NULL,16);
    }

    else if(!strcmp(b,"WORD"))
    return 3;

    else if(!strcmp(b,"RESB"))
    {
        return strtol(c,NULL,16);
    }

    else if(!strcmp(b,"BYTE"))
    {
        if(c[0]=='X')
        {
            return (strlen(c)-3)/2;
        }
        else if(c[0]=='C')
        {
            return strlen(c)-3;
        }
    }   

    else if(!strcmp(b,"START"))
    return 0;

    else
    {
        printf("Invalid opcode\n");
    }
}

int main()
{
     FILE *fp2;
    fp2=fopen("SymbolTable.txt","w");
    FILE *fp=NULL;
    FILE *fp1=fopen("Output.txt","w");
    createOptab();

    fp=fopen("input.txt","r");
    if(fp==NULL)
    {
        printf("File not found");
        return 0;
    }

    struct instruction inst[100];
    int i=0;
    int baseAdress=0;
    while(!feof(fp))
    {
        char *token ;
        char a[100],b[100],c[100];
    
        
        char line[100];
        fgets(line,100,fp);
        token=strtok(line," ");
        int count=0;
        while(token)
        {
            
            if(count==0)
            {
               strcpy(a,token);
                count++;
            }
            else if(count==1)
            {
                 strcpy(b,token);
                count++;
            }
            else if(count==2)
            {
                 strcpy(c,token);
                count++;
            }

            token=strtok(NULL," ");
            
            //printf("%s\n%s\n%s\n",a,b,c);
            
            
        }
        
        if(i==0){
            if(strcmp(b,"START")==0)
            {
                printf("baseadress update\n");
                baseAdress=strtol(c,NULL,16);
                //printf("base adress = %x",baseAdress);
            }

        }


        if(count==1)
        {
        strcpy(inst[i].name," ");
        strcpy(inst[i].opcode,a);
        strcpy(inst[i].operand1," ");
        inst[i].adress=baseAdress;
        baseAdress+=0;
       
        }

        else if(count==2){
        strcpy(inst[i].name," ");
        strcpy(inst[i].opcode,a);
        strcpy(inst[i].operand1,b);
        inst[i].adress=baseAdress;
        baseAdress+=addAdress(a,b);
       
        }
        else{
        strcpy(inst[i].name,a);
        strcpy(inst[i].opcode,b);
        strcpy(inst[i].operand1,c);
        inst[i].adress=baseAdress;
        baseAdress+=addAdress(b,c);
        
        }
        createSymbolTable(inst[i],fp2);
        fprintf(fp1,"%-10x %-10s %s %s",inst[i].adress,inst[i].name,inst[i].opcode,inst[i].operand1);
        i++;

    }
    
    fclose(fp);
   
}