#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
typedef struct {
    char name[50];
    int type;
    char filename[50];
}file;
int init(FILE **defDir)
{
    if(*defDir==NULL)
    {
        *defDir=fopen("defDir.dir","ab+");
        if(*defDir==NULL)
        {
            return 0;
        }
        else{
            return 1;
        }

    }
    return 1;
}
void getFiles(FILE *dir)
{
    file f;
    fseek(dir,0,0);
    printf("NAME\t\t\tType");
    while((fread(&f,sizeof(file),1,dir)))
    {
        printf("\n%s\t\t\t%s\n",f.name,((f.type==1)? "File" :"Directory"));
    }
}
void addFile(FILE *dir,char *fname)
{
    file f;
    strcpy(f.name,fname);
    sprintf(f.filename,"%lu",time(0));
    f.type=1;
    FILE *fl;
    fl=fopen(f.filename,"wb");
    if(fl!=NULL)
    {
        int i=fwrite(&f,sizeof(file),1,dir);
        fclose(fl);
    }
    else{
        printf("\nSome Error Has Occured\n");
    }

}
void addDir(FILE *dir,char *dirName)
{
    file f;
    strcpy(f.name,dirName);
    sprintf(f.filename,"%lu",time(0));
    f.type=0;
    FILE *fl;
    fl=fopen(f.filename,"wb");
    if(fl!=NULL)
    {
        int i=fwrite(&f,sizeof(file),1,dir);
        fclose(fl);
    }
    else{
        printf("\nSome Error Has Occured\n");
    }
}
void changeDirectory(FILE **dir,char *dirName)
{
    file f;
    int result=checkIsDirectoryExist(*dir,dirName,&f);
    if(result==1)
    {
        *dir=fopen(f.filename,"ab");
        printf("\nDirectory changed to %s\n",f.name);
    }
    else{
        printf("\nDirectory does not exist");
    }
}
int checkIsDirectoryExist(FILE *dir,char *dirname,file *result){
    file f;
    fseek(dir,0,0);
    while((fread(&f,sizeof(file),1,dir)))
    {
        if((f.type==0) && (strcmp(f.name,dirname)==0))
        {
            strcpy(result->name,f.name);
            strcpy(result->filename,f.filename);
            result->type=f.type;
            return 1;
        }
    }
    return 0;
}
void execute(char cmd[],FILE **fp)
{
    if(stricmp(cmd,"ls")==0)
    {
        getFiles(*fp);
    }
    else if(stricmp(cmd,"touch")==0)
    {
        char filename[50];

        gets(filename);
        addFile(*fp,filename);
    }
    else if(stricmp(cmd,"mkdir")==0)
    {
        char filename[50];
        gets(filename);
        addDir(*fp,filename);
    }
    else if(stricmp(cmd,"cd")==0)
    {
        char filename[50];
        gets(filename);
        printf("\n%s",filename);
        if(strcmp(filename,"~~")==0)
        {
            *fp=fopen("defDir.dir","ab+");
            return;
        }
        changeDirectory(fp,filename);
    }

}
int main()
{
    FILE *dir=NULL;
    char cmd[50];
    init(&dir);
    while(stricmp(cmd,"exit")!=0)
    {
        printf("$ ");
        scanf("%s",cmd);
        execute(cmd,&dir);
    }
}
