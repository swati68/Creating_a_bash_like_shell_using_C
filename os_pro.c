#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <utime.h>
#include <math.h>
#define READ_LINE_BUFFER 1024
#define TOKEN_BUFFER 64
#define TOKEN_DELIMITER " \t\r\n\a"


int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_log(char **args);
int lsh_echo(char ** args);
int lsh_ls( char ** args);
int lsh_cat( char ** args);
int lsh_mv( char ** args);
int lsh_grep( char ** args);
int lsh_add (char ** args);
int ps(char ** args);
int find(char ** args);
int mtime(char ** args);
int touch(char ** args);
int lsh_wc(char **args);
int lsh_sort(char **args);
int lsh_rm(char **args);
int lsh_clear(char **args);
int lsh_cmp(char **args);
int lsh_clclog(char **args);
int lsh_viewlog(char **args);
int lsh_uniq(char **args);
 int lsh_head( char ** args);
 int lsh_tail( char ** args);
int lsh_diff(char **args);
int lsh_expt(char **args);
int lsh_fact(char **args);
int lsh_tac(char **args);

char *builtin_str[] = {
  "cd",
  "help",
  "exit", 
  "log",
  "echo",
  "ls",
  "cat",
  "mv",
  "grep",
  "add",
  "ps",
  "find",
  "mtime",
  "touch",
  "wc",
  "sort",
  "rm",
  "clear",
  "cmp",
  "clclog",
  "viewlog",
  "uniq",
  "head",
"tail",
"diff",
"expt",
"fact",
"tac"
};


int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit,
  &lsh_log,
  &lsh_echo, 
  &lsh_ls,
  &lsh_cat,
  &lsh_mv,
  &lsh_grep,
  &lsh_add,
  &ps,
  &find,
  &mtime,
  &touch,
  &lsh_wc,
  &lsh_sort,
  &lsh_rm,
  &lsh_clear,
  &lsh_cmp,
  &lsh_clclog,
  &lsh_viewlog,
  &lsh_uniq,
  &lsh_head,
&lsh_tail,
&lsh_diff,
&lsh_expt,
&lsh_fact,
&lsh_tac
};


int num_of_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int touch(char ** args){
    int fdold, count;
    fdold=open(args[1], O_RDWR | O_APPEND);
    if (fdold==-1){
        mode_t mode=S_IRUSR | S_IWUSR| S_IRGRP | S_IROTH;
        creat(args[1], mode);
        return 1;
    }    
    else{
        printf("File already exists\n");
    }
    return 1;
}

int mtime(char ** args){
    int retvalue;
    
    retvalue=utime(args[1],NULL);
    if(retvalue==0){
    printf("Timestamp modified\n");
    }
    return 1;
}

void find_file(char *basePath, char *fileName){
    char path[1000];
    struct dirent *dp;
    DIR *dir=opendir(basePath);
    if (!dir){return;}
    while((dp=readdir(dir))!=NULL){
        if(strcmp(dp->d_name,".")!=0 && strcmp(dp->d_name, "..")!=0){
            if (strcmp(dp->d_name, fileName)==0){printf("Path:%s\n",basePath);}
            
            else{
                strcpy(path,basePath);
                strcat(path, "/");
                strcat(path, dp->d_name);
                find_file(path, fileName);
            }
        }
    }
    closedir(dir);
}

int find(char ** args){
    find_file(args[1], args[2]);
    return 1;
}    

int ps(char ** args){

    DIR *dp ;
    struct dirent *entry;
    
    
    if ((dp=opendir("/proc"))==NULL){
        printf("Cannot open directory");
        return 0;
    }
    chdir("/proc");
    while ((entry=readdir(dp))!=NULL){
        int flag=0;
        for (int i=0;entry->d_name[i]!='\0';i++){
            if (entry->d_name[i]=='0'||entry->d_name[i]=='1'||entry->d_name[i]=='2'||entry->d_name[i]=='3'||entry->d_name[i]=='4'||entry->d_name[i]=='5'||entry->d_name[i]=='6'||entry->d_name[i]=='7'||entry->d_name[i]=='8'||entry->d_name[i]=='9'){flag++;}
        }
        if (flag>0){
            //printf("%s\n",entry->d_name);
            printf("Pid:%s\t", entry->d_name);
            int fd, r, j=0;
            char temp, line[100];
            strcat(entry->d_name, "/status");
            //printf("%s\n",entry->d_name);
            if ((fd=open(entry->d_name,O_RDONLY))!=-1){
                while((r=read(fd,&temp, sizeof(char)))!=0 && temp!='\n'){
                    
                        line[j]=temp;
                        j++;
                    
                }
            }
            printf("%s\n", line);
            
        }            
    }
    closedir(dp);
    return 1;
}        



int lsh_add(char ** args){
    int fdold1,fdold2,count;
    char buffer[2048]; //characer buffer to store the bytes
    fdold1=open(args[1], O_RDWR);
    if(fdold1==-1)
    {
    printf("cannot open file1");
    return 0;
    }

    fdold2=open(args[2], O_RDWR| O_APPEND);
    if(fdold2==-1)
    {
    printf("cannot open file2");
    return 0;
    }

    while((count=read(fdold1, buffer, sizeof(buffer)))>0){
            //printf("%s", buffer);
        size_t n=strlen(buffer);
        if ((write(fdold2, buffer , n))==-1){
            printf("Error while writing\n");
            return 1;
        }
    }
    close(fdold2);
    fdold2=open(args[2], O_RDWR| O_APPEND);
    if(fdold2==-1)
    {
    printf("cannot open file2");
    return 0;
    }

    while((count=read(fdold2, buffer, sizeof(buffer)))>0){
            printf("%s", buffer);
    }
    close(fdold1);
    close(fdold2);
    return 1;
}

int compare(const char* X, const char *Y){
    while( *X && *Y){
        if(*X!=*Y)return 0;
        X++;
        Y++;
    }
    return (*Y == '\0');
}

const char* str_in_str(const char* X, const char* Y){
    while(*X != '\0'){
    if ((*X == *Y) && compare(X, Y))return X;
    X++;
    }
    return NULL;
}

int lsh_grep( char ** args){
    int fd, r, j=0;
    char temp, line[100];
    if ((fd=open(args[2], O_RDONLY))!=-1){
        while((r=read(fd,&temp, sizeof(char)))!=0){
if (temp!='\n'){
    line[j]=temp;
    j++;
    }
    else{
        if(str_in_str(line, args[1])!=NULL)
        printf("%s\n", line);
        memset(line, 0, sizeof(line));
        j=0;
    }
}
}
return 1;
}

int lsh_mv( char ** args){
    int i, fd1, fd2;
    char *file1, *file2, buf[2];
    file1=args[1];
    file2=args[2];
    printf("file1=%s, file2=%s", file1, file2);
    fd1=open(file1, O_RDONLY, 0777);
    fd2=creat(file2, 0777);
    while(i=read(fd1, buf,1)>0)
    write (fd2, buf, 1);
    remove(file1);
    close(fd1);
    close(fd2);
    return 1;
}

int cat_read(char ** args){
    int fdold,count;
    char buffer[2];
    fdold=open(args[1], O_RDONLY);
    if (fdold==-1){
        printf("Cannot open file\n");
        return 1;
    }
    while((count=read(fdold, buffer, 1))>0){
        printf("%s", buffer);
    }    
    printf("\n");
    return 1;
}

int cat_append( char ** args){
    int fdold, count;
    fdold=open(args[2], O_RDWR | O_APPEND);
    if (fdold==-1){
        mode_t mode=S_IRUSR | S_IWUSR| S_IRGRP | S_IROTH;
        creat(args[2], mode);
        fdold=open(args[2], O_RDWR | O_APPEND);
        if (fdold==-1){
            printf("Cannot open the created file");
            return 1;}
        
    }
    char buffer[1024];
    printf("Enter the data to be appended:");
    scanf("%s", buffer);
    size_t n=strlen(buffer);
    if ((write(fdold, buffer,n))==-1){
        printf("Error while writing\n");
        
    }
    else{
        close(fdold);
        
    }
    return 1;
}
    

            
int lsh_cat(char ** args){
    if (args[1]==NULL){
        printf("Please enter a file name\n");
        
    }
    else if (strcmp(args[1], "a")==0){
        cat_append(args);
    }
    else{
        cat_read(args);
    }
    
return 1;
}


char log_buffer[10][2][30];
int l=0;

  struct lines{
    char stack_lines1[100];
    char stack_lines2[100];
 };


int lsh_tac(char ** args)
{
    FILE *fp1;
     
    int cnt = 0;
    int i   = 0;
     
     
    fp1 = fopen(args[1],"r");
    if( fp1 == NULL )
    {
        printf("\n%s File can not be opened : \n",args[1]);
        return -1;
    }
     
                                    //moves the file pointer to the end.
    fseek(fp1,0,SEEK_END);
                                    //get the position of file pointer.
    cnt = ftell(fp1);
     
    while( i < cnt )
    {
        i++;
        fseek(fp1,-i,SEEK_END);
        printf("%c",fgetc(fp1));
    }
    printf("\n");
    fclose(fp1);
     
    return 1;
}


int lsh_fact(char ** args)
{
     int n;
     sscanf(args[1], "%d", &n);                                  //converting string to integer
                                                                 // Print the number of 2s that divide n
    while (n % 2 == 0) {
        printf("%d ", 2);
        n = n / 2;
    }
    printf("\n");
  int i;
                                    // n must be odd at this point.  So we can skip
                                    // one element (Note i = i +2)
    for (i = 3; i <= sqrt(n); i = i + 2) {
                                    // While i divides n, print i and divide n
        while (n % i == 0) {
            printf("%d ", i);
            n = n / i;
        }
    }
 
                                    // This condition is to handle the case when n
                                    // is a prime number greater than 2
    if (n > 2)
        printf("%d ", n);
printf("\n");    
return 1;
}


int precedence(char a,char b)
{                                                    //returns true if precedence of operator a is more or equal to than that of b
    if(((a=='+')||(a=='-'))&&((b=='*')||(b=='/')))
    return 0;
    else
    return 1;
}

int operate(int a,int b,char oper)
{
    int res=0;
switch(oper)
{
    case '+':res=a+b;break;
    case '-':res=a-b;break;
    case '*':res=a*b;break;                         //return result of evaluation
    case '/':res=a/b;break;
}
return res;
}

int lsh_expt(char ** args)
{char stack[20];                                     //store the postfix expression
int top=-1;                                          //top of postfix stack
int topOper=-1;                                       //top of operator stack
char expr[20];
strcpy(expr,args[1]);
char topsymb,operatorStack[20];
int ctr=0;
while(expr[ctr]!='\0')                                //converting to postfix
{                                                     //read till the end of input
if(expr[ctr]>='0'&&expr[ctr]<='9')
stack[++top]=expr[ctr];//add numbers straightaway
else
{
while(topOper>=0&&precedence(operatorStack[topOper],expr[ctr]))
{                                                     //check for the operators of higher precedence and then add them to stack
topsymb=operatorStack[topOper--];
stack[++top]=topsymb;
}
operatorStack[++topOper]=expr[ctr];
}
ctr++;
}
while(topOper>=0)                                     //add remaining operators to stack
stack[++top]=operatorStack[topOper--];
char oper;
int operand1,operand2;
ctr=0;
int result[2];                                        //stack to keep storing results
int rTop=-1;                                          //top of result stack
while(stack[ctr]!='\0')
{
oper=stack[ctr];
if(oper>='0'&&oper<='9')
result[++rTop]=(int)(oper-'0');                       //add numbers
else
{                                                     //if an operator is encountered then pop twice and push the result of operation to the stack
    operand1=result[rTop--];
    operand2=result[rTop--];
    result[++rTop]=operate(operand2,operand1,oper);
}
ctr++;
}

printf("%d\n",result[0]);
return 1;
}


int lsh_head(char ** args)
{
   
               FILE  *file;

               char line[100];
               int count = 0;
               
              // initialise file pointer to read
               file  =  fopen(args[1],"r");

             // read line by line
              while(fscanf(file , "%[^\n]\n" , line)!=EOF)
              {
                         // break after 10 lines
                         if(count  == 10)
                         {
                                 break;
                         }
                         else
                         {
                                 printf("%s\n" ,  line);
                          }
                          count++;
             }

             fclose(file);
return(1);
}

struct stack {char strings[100];};
int lsh_tail(char ** args)
{

             // stucture initialisation    
              struct stack s[100];

              FILE *file;
              char line[100];
         
              int n,count=0, i=0;

              file  = fopen(args[1] , "r");
         
              // reading line by line and push to stack
              while(fscanf(file , "%[^\n]\n" , line)!=EOF)
              {
                             strcpy(s[i].strings , line);
                             i++;
                             n=i;
               }

               // pop line by line
               for(i=n;i>0;i--)
               {
                        // last 10 lines  
                         if(count == 10)
                               break;
                         else
                               printf("%s\n" , s[i].strings);
                         count++;
               }
return(1);
}


int lsh_diff(char ** args)
{
    struct lines st[20];
    FILE *file1,*file2;
    char line1[100],line2[100];
    int line_count1=0,flag2[10],flag1[10],line_count2=0;
    int i=0,j=0,k=0,n=0,m=0,o=0;
 
    file1 = fopen(args[1],"r");
    file2 = fopen(args[2],"r");
 
     while(1)
     {
            line_count1++;
            line_count2++;
         
           if(fscanf(file1,"%[^\n]\n",line1)!=EOF && fscanf(file2,"%[^\n]\n",line2)!=EOF)
           {
                  if(strcmp(line1,line2) == 0)
                        continue;
                  else{
                    if(line1 != NULL){
                        strcpy(st[i].stack_lines1 , line1);
                        flag1[m]=line_count1;
                        m++;
                      }
                      if(line2 != NULL){
                            strcpy(st[i].stack_lines2 , line2);
                            flag2[o]=line_count2;
                            o++;
                      }
                  }
           }
           else if(fscanf(file1,"%[^\n]\n",line1)!=EOF){
                  strcpy(st[i].stack_lines1 , line1);
                  flag1[m]=line_count1;
                  m++;
            }
           else if(fscanf(file2,"%[^\n]\n",line2)!=EOF){
                  strcpy(st[i].stack_lines2 , line2);
                  flag2[o]=line_count2;
                  o++;
            }
           else
                  break;
           i++;
           n++;
     }
   
     for(i=0;i<m;i++)
     {
        printf("%d,",flag1[i]);
     }
     printf("c");
     for(i=0;i<o;i++)
     {
        printf("%d,",flag2[i]);
     }
     printf("\n");
     for(i=0;i<n;i++)
     {
        printf("%s\n",st[i].stack_lines1);
     }
     printf("---\n");
     for(i=0;i<n;i++)
     {
        printf("%s\n",st[i].stack_lines2);
     }
     fclose(file1);
     fclose(file2);
return(1);
   
}

int lsh_ls(char ** args){
    struct dirent **namelist;
    int n;
    if (args[1]==NULL){
        n=scandir(".", &namelist, NULL, alphasort);
    }
    else{
        n=scandir(args[1], &namelist, NULL, alphasort);
    }
    if (n<0){
        perror("scandir");
        exit(EXIT_FAILURE);
    }
    else{
        while (n--){
            printf("%s\n", namelist[n]->d_name);
            free(namelist[n]);
        }
        free(namelist);
    }
    return 1;    
}


void log_entry(char* line, time_t t){
    strcpy(log_buffer[l][0], line);
    strcpy(log_buffer[l][1], ctime(&t));
    l++;

}


int lsh_log(char ** args){
    printf("Displaying the log:\n");
    for (int i=0; i<=l;i++){
        printf("%s\t %s\n",log_buffer[i][0], log_buffer[i][1]);
    }
return 1;
}


int lsh_echo(char ** args){
    int k=1;
    while (args[k]!=NULL){
        printf("%s ", args[k]);
        k++;
    }
printf("\n");
return 1;
}


int lsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "OS_Shell: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("OS_Shell");
    }
  }
  return 1;
}


int lsh_help(char **args)
{
  int i;
  printf("OS Project Shell\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < num_of_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int lsh_wc(char **args)  //Calculate the character,word,line,byte count in file
{
  int character_count=0;
  int space_count=0;
  int word_count=0;
  int line_count=0;
  int byte_count=0;
  char ch;
    
    int character_count1 = 0;
  int space_count1 = 0;
  int word_count1 = 0;
  int line_count1 = 0;
  int byte_count1=0;
  char ch1;
 
  FILE *fp;
  FILE *fp1;
    
  if(args[1] == NULL)
  {
       fprintf(stderr, "OS_Shell: expected argument to \"wc\"\n");
  }
  else
  {
        if(strstr(args[1],"txt")!=NULL)  //wc "filename".txt (wc m1.txt)
        {
             fp = fopen(args[1],"r");

          while((ch=fgetc(fp))!=EOF)
          {
            character_count++;
            if(ch == ' ')
            {
                 space_count++;
              word_count++;
            }
              if(ch == '\n')
            {
                    line_count++;
            }
          }
          fclose(fp);
          if(args[2]==NULL)
          {
              printf("Line_count = %d\n",line_count);
              printf("Word_count = %d\n",word_count+2);
              printf("Space_count = %d\n",space_count);
              printf("Character_count = %d\n",character_count);
              printf("Byte_count=%d\n",(character_count+space_count+1));
                 printf("File_name= %s\n",args[1]);
             }
             else if(strstr(args[2],"txt")!=NULL)  //wc "file1".txt "file2".txt (wc m1.txt m.txt)
             {
                 fp1 = fopen(args[2],"r");

              while((ch1=fgetc(fp1))!=EOF)
              {
                character_count1++;
                if(ch1 == ' ')
                {
                     space_count1++;
                  word_count1++;
                }
                  if(ch1 == '\n')
                {
                        line_count1++;
                }
              }
              fclose(fp1);
              
              printf("Line_count = %d\t",line_count);
              printf("Word_count = %d\t",word_count+2);
              printf("Space_count = %d\t",space_count);
              printf("Character_count = %d\t",character_count);
              printf("Byte_count=%d\t",(character_count+space_count+1));
                 printf("File_name= %s\n",args[1]);
                 printf("Line_count = %d\t",line_count1);
              printf("Word_count = %d\t",word_count1+2);
              printf("Space_count = %d\t",space_count1);
              printf("Character_count = %d\t",character_count1);
              printf("Byte_count=%d\t",(character_count1+space_count1+1));
                 printf("File_name= %s\n",args[2]);
                 printf("Total:\n");
                 printf("Line_count = %d\t",line_count+line_count1);
              printf("Word_count = %d\t",word_count+4+word_count1);
              printf("Space_count = %d\t",space_count+space_count1);
              printf("Character_count = %d\t",character_count+character_count1);
              printf("Byte_count=%d\n",(character_count+space_count+1)+(character_count1+space_count1+1));
          }
      }
      else if(strstr(args[1],"txt")==NULL)
      {
          fp = fopen(args[2],"r");

          while((ch=fgetc(fp))!=EOF)
          {
            character_count++;
            if(ch == ' ')
            {
                 space_count++;
              word_count++;
            }
              if(ch == '\n')
            {
                    line_count++;
            }
        }
        fclose(fp);
        
        if(strcmp(args[1],"-l")==0)  //wc -l "file".txt (Print line count)
        {
            printf("line_count = %d\n",line_count);
                 printf("File_name= %s\n",args[2]);
             }
             else if(strcmp(args[1],"-w")==0)  //wc -w "file".txt (Print word count)
             {
                 printf("word_count = %d\n",word_count+2);
                 printf("File_name= %s\n",args[2]);
             }
             else if(strcmp(args[1],"-m")==0)//wc -m "file".txt (Print character count)
             {
                 printf("character_count = %d\n",character_count);
                 printf("File_name= %s\n",args[2]);
             }
             else if(strcmp(args[1],"-s")==0) //wc -s "file".txt (Print space count)
             {
                 printf("space_count = %d\n",space_count);
                 printf("File_name= %s\n",args[2]);
             }
             else if(strcmp(args[1],"-L")==0)  //wc -L "file".txt (Print maximum length line)
          {
              if(args[2] == NULL)
              {
                   fprintf(stderr, "OS_Shell: expected argument to \"wc\"\n");
              }
              else
              {
                  char line[1000];
                  int maxc=-999;
                  char ref[1000];
                  fp = fopen(args[2],"r");
                  while(fgets(line,sizeof(line),fp))
                  {
                      int len=strlen(line);
                      if(len>maxc){
                      maxc=len;
                      strcpy(ref,line);
                      }
                  }
                  fclose(fp);
                  printf("Biggest_line = %s",ref);
                     printf("File_name= %s\n",args[2]);
              }
             }
         }
     }
      return 1;
}

int lsh_cmp(char **args)  //compare the content of two files
{
    FILE *fx,*fx1;
    FILE *fz,*fz1;
    int count=0,flag=0,linec=1;
    char c;
    char ch;
    if(strstr(args[1],"txt")!=NULL)  //cmp "file1".txt "file2".txt (Print byte and line number if files don't match) (cmp m1.txt m.txt)
    {
        fx=fopen(args[1],"r");
        fx1=fopen(args[2],"r");
        while((c=getc(fx))!=EOF)
        {
            count++;
            ch=getc(fx1);
            if(c=='\n')
            {
                linec++;
            }
            if(ch!=c)
            {
                flag=1;
                break;
            }
        }
        if(flag==1)
        {
            printf("%s and %s do not match at byte %d line     %d\n",args[1],args[2],count,linec);
            return 1;
        }
        else
        {
            printf("%s and %s perfectly match\n",args[1],args[2]);
            return 1;
        }
        fclose(fx);
        fclose(fx1);
    }
    else if(strcmp(args[1],"-b")==0)  //cmp -b "file1".txt "file2".txt (Print character along with byte and line number at which file differ)
    {
        fz=fopen(args[2],"r");
        fz1=fopen(args[3],"r");
        if(fz==NULL || fz1==NULL)
        {
            perror("Failed:");
            return 1;
        }
        while((c=getc(fz))!=EOF)
        {
            count++;
            ch=getc(fz1);
            if(c=='\n')
            {
                linec++;
            }
            if(ch!=c)
            {
                flag=1;
                break;
            }
        }
        if(flag==1)
        {
            printf("%s and %s differ at byte %d and line %d where %c in %s and %c in %s differ\n",args[2],args[3],count,linec,c,args[2],ch,args[3]);
            return 1;
        }
        else
        {
            printf("%s and %s perfectly match\n",args[2],args[3]);
            return 1;
        }
        fclose(fz);
        fclose(fz1);
    }
    
}    

int lsh_sort(char **args)  //sort the data in a file
{
    FILE *f1;
    char sorted[1000][1000];
    char value[1000];
    int c=0;
    int i,j;
    if(strstr(args[1],"txt")!=NULL) //sorting in ascending order (sort s1.txt)
    {
        f1=fopen(args[1],"r");
        while(fgets(value,sizeof(value),f1)!=NULL)
        {
            if(strchr(value,'\n'))
            {
                value[strlen(value)-1]='\0';
            }
            strcpy(sorted[c],value);
            c++;
        }
        for(i=0;i<c-1;++i)
        {
            for(j=0;j<(c-i-1);++j)
            {
                if(strcmp(sorted[j],sorted[j+1])>0)
                {
                    strcpy(value,sorted[j]);
                    strcpy(sorted[j],sorted[j+1]);
                    strcpy(sorted[j+1],value);
                }
            }
        }
		fclose(f1);
        if(args[2]==NULL)  //Printing the sorted array
        {
            printf("Sorted Data:");
            for(i=0;i<c;i++)
            {
                printf("%s\n",sorted[i]);
            }
        }
        else if(strstr(args[2],"txt")!=NULL)  //Storing sorted array in new file
        {
            FILE *f2;
            f2=fopen(args[2],"w");
            for(i=0;i<c;i++)
            {
                fprintf(f2,"%s\n",sorted[i]);
            }
            fclose(f2);
        }
    }
    else if(strcmp(args[1],"-r")==0)  //sorting in reverse order
    {
        f1=fopen(args[2],"r");
        while(fgets(value,sizeof(value),f1)!=NULL)
        {
            if(strchr(value,'\n'))
            {
                value[strlen(value)-1]='\0';
            }
            strcpy(sorted[c],value);
            c++;
        }
        for(i=0;i<c-1;++i)
        {
            for(j=0;j<(c-i-1);++j)
            {
                if(strcmp(sorted[j+1],sorted[j])>0)
                {
                    strcpy(value,sorted[j]);
                    strcpy(sorted[j],sorted[j+1]);
                    strcpy(sorted[j+1],value);
                }
            }
        }
        fclose(f1);
        if(args[3]==NULL)
        {
            printf("Sorted Data:");
            for(i=0;i<c;i++)
            {
                printf("%s\n",sorted[i]);
            }
        }
        else if(strstr(args[3],"txt")!=NULL)
        {
            FILE *f2;
            f2=fopen(args[3],"w");
            for(i=0;i<c;i++)
            {
                fprintf(f2,"%s\n",sorted[i]);
            }
            fclose(f2);
        }
    }
    return 1;
}

int lsh_rm(char **args)  //Delete a file from directory(rm s2.txt)
{
    int del=remove(args[1]);
    if(!del)
    {
        printf("File deleted successfully\n");
    }
    else
    {
        printf("Deletion failed\n");
    }
    return 1;
}

int lsh_uniq(char **args)  //Print line which is duplicated(uniq m.txt)
{
    if(args[1]==NULL)
    {
        fprintf(stderr, "OS_Shell: expected argument to \"uniq\"\n");
    }
    else
    {
    FILE *fp,*fp1;
    char line[100],line1[100];
    fp=fopen(args[1],"r");
    fp1=fopen("test.txt","w");
    fgets(line,sizeof(line),fp);
    fputs(line,fp1);
    while(fgets(line1,sizeof(line1),fp))
    {
        if(strcmp(line,line1)==0)
        {
            continue;
        }
        else
        {
            fputs(line1,fp1);
            strcpy(line,line1);
        }
    }
    fclose(fp);
    fclose(fp1);
    remove(args[1]);
    rename("test.txt",args[1]);
    }
    return 1;
}
        

int lsh_clclog(char **args)  //Clear the log
{
    char dat[20];
    char line[100];
    if(args[2]==NULL)  //clear entire log
    {
        fclose(fopen("log2.txt","w"));
        printf("Log cleared successfully.\n");
        return 1;
    }
    else       //clear log on the specified date
    {
        FILE *fp,*fp1;
        int flag=0;
        fp=fopen("log2.txt","r");
        fp1=fopen("replica.txt","w");
        strcpy(dat,args[2]);
        while(fgets(line,sizeof(line),fp))
        {
            for(int i=0;i<strlen(dat);i++)
            {
                if(dat[i]!=line[i])
                {
                    flag=1;
                    break;
                }
            }
            if(flag==1)
            {
                fputs(line,fp1);
                flag=0;
            }
        }
        fclose(fp);
        fclose(fp1);
        remove("log2.txt");
        rename("replica.txt","log2.txt");
        return 1;
    }
}

int lsh_viewlog(char **args)  //Used to view log of specific date
{
    if(args[1]==NULL)
    {
        fprintf(stderr, "OS_Shell: expected argument to \"viewlog\"\n");
    }
    else
    {
    char dat[20];
    char line[100];
    FILE *fp;
    int flag=0;
    fp=fopen("log2.txt","r");
    strcpy(dat,args[1]);
    while(fgets(line,sizeof(line),fp))
    {
        for(int i=0;i<strlen(dat);i++)
        {
            if(dat[i]!=line[i])
            {
                flag=1;
                break;
            }
        }
        if(flag==0)
        {
            printf("%s",line);
        }
        else
        {
            flag=0;
        }
    }
    fclose(fp);
    }
    return 1;
}

int lsh_clear(char **args)    //Clear the console window
{
    const char* blank="\e[1;1H\e[2J";
    write(STDOUT_FILENO,blank,12);
    return 1;
}



int lsh_exit(char **args)
{
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  FILE *f = fopen("log2.txt", "a");
  fprintf(f,"%d-%02d-%02d %02d:%02d:%02d : ",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  for (int i=0; i<=l;i++){
      char x[1000];
      strcpy(x,log_buffer[i][0]);
      int k=strlen(x);
      for(int j=0;j<k;j++)
      {
          if(x[j]=='\0' || x[j]=='\00')
          {
              continue;
          }
          else
          {
              fputc(x[j],f);
          }
      }
      fputs(",",f);
  }
  fputs("\n",f);
    fclose(f);
  return 0;
}


int lsh_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("OS_Shell");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("OS_Shell");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;    
}


int lsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {        //Finish if an empty command is entered
    return 1;
  }

  for (i = 0; i < num_of_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {        //Compare the arguments with builtin cmds
      return (*builtin_func[i])(args);            //Call the appropriate function to execute
    }
  }

  return lsh_launch(args);
}



char *lsh_read_line(void)
{
  int buffer_size = READ_LINE_BUFFER;
  int position = 0;
  char *buffer = malloc(sizeof(char) * buffer_size);
  int c;

  if (!buffer) {
    fprintf(stderr, "OS_Shell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();

    if (c == EOF || c == '\n') {    //Replace EOF with a null character
      buffer[position] = '\0';
      return buffer;            //Return the buffer or the line after reaching EOF
    } else {
      buffer[position] = c;
    }
    position++;

    // Reallocate buffer, if the buffer size has exceeded
    if (position >= buffer_size) {
      buffer_size += READ_LINE_BUFFER;
      buffer = realloc(buffer, buffer_size);
      if (!buffer) {
        fprintf(stderr, "OS_Shell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}



char **lsh_split_line(char *line)
{
  int buffer_size = TOKEN_BUFFER, position = 0;
  char **tokens = malloc(buffer_size * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "OS_Shell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOKEN_DELIMITER);    //Parses the string & converts to tokens
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= buffer_size) {        //Reallocate memory if buffer size has exceeded
      buffer_size += TOKEN_BUFFER;
      tokens = realloc(tokens, buffer_size * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "OS_Shell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOKEN_DELIMITER);
  }
  tokens[position] = NULL;
  return tokens;                //Return the tokens list 
}


void lsh_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("OS_Project> ");
    line = lsh_read_line();        //Read the input line
    time_t t;
    time(&t);
    log_entry(line, t);
    args = lsh_split_line(line);    //Split the input line
    status = lsh_execute(args);        //Execute the arguments

    free(line);
    free(args);
  } while (status);
}


int main(int argc, char **argv)
{
  printf("Welcome to this OS Project Shell\n");
  lsh_loop();
  return EXIT_SUCCESS;
}


